#include <Colonization/BuildConfiguration.hpp>
#include "MapMaskUpdater.hpp"

#include <Urho3D/Core/CoreEvents.h>
#include <Urho3D/Core/Context.h>
#include <Urho3D/Math/MathDefs.h>

#include <Urho3D/Scene/Node.h>
#include <Urho3D/Scene/Scene.h>
#include <Urho3D/Graphics/Graphics.h>

#include <Colonization/Core/District.hpp>
#include <Colonization/Core/Map.hpp>
#include <Colonization/Frontend/FogOfWarCalculator.hpp>

#include <Colonization/Utils/Categories.hpp>
#include <Colonization/Utils/ImageUtils.hpp>
#include <Colonization/Utils/AttributeMacro.hpp>

namespace Colonization
{
Urho3D::IntVector2 MapMaskUpdater::WorldPointToMapPoint (Urho3D::Vector3 worldPoint)
{
    Urho3D::Vector3 delta = worldPoint - mapMinPoint_;
    float mapWidth = mapMaxPoint_.x_ - mapMinPoint_.x_;
    float mapHeight = mapMaxPoint_.z_ - mapMinPoint_.z_;

    Urho3D::IntVector2 positionOnMap;
    positionOnMap.x_ = static_cast <int> (std::floor (MAP_MASK_WIDTH * delta.x_ / mapWidth));
    positionOnMap.y_ = static_cast <int> (std::floor (MAP_MASK_HEIGHT * delta.z_ / mapHeight));
    return positionOnMap;
}

void MapMaskUpdater::DrawDistrictBorders (District *district)
{
    Urho3D::PODVector <Urho3D::Vector3> polygonPoints = district->GetPolygonPoints ();
    for (int index = 0; index < polygonPoints.Size (); index++)
    {
        Urho3D::Vector3 firstPoint = polygonPoints.At (index);
        Urho3D::Vector3 secondPoint;

        if (index + 1 < polygonPoints.Size ())
            secondPoint = polygonPoints.At (index + 1);
        else
            secondPoint = polygonPoints.At (0);

        Urho3D::IntVector2 firstOnMap = WorldPointToMapPoint (firstPoint);
        Urho3D::IntVector2 secondOnMap = WorldPointToMapPoint (secondPoint);

        ImageUtils::DrawLine (maskImage_, MAP_MASK_DISTRICT_BORDER_LINE_COLOR,
                              firstOnMap.x_, firstOnMap.y_,
                              secondOnMap.x_, secondOnMap.y_,
                              MAP_MASK_DISTRICT_BORDER_LINE_WIDTH);
    }
}

MapMaskUpdater::MapMaskUpdater (Urho3D::Context *context) : Urho3D::Component (context),
    maskImage_ (new Urho3D::Image (context)),
    maskTexture_ (new Urho3D::Texture2D (context)),

    mapMinPoint_ (),
    mapMaxPoint_ (),
    districtColorToDistrictHash_ ()
{
    maskImage_->SetSize (MAP_MASK_WIDTH, MAP_MASK_HEIGHT, Urho3D::Graphics::GetRGBFormat ());
    maskTexture_->SetData (maskImage_, false);
    SubscribeToEvent (Urho3D::E_UPDATE, URHO3D_HANDLER (MapMaskUpdater, Update));
}

MapMaskUpdater::~MapMaskUpdater ()
{

}

void MapMaskUpdater::RegisterObject (Urho3D::Context *context)
{
    context->RegisterFactory <MapMaskUpdater> (COLONIZATION_CLIENT_ONLY_CATEGORY);

    URHO3D_ACCESSOR_ATTRIBUTE ("Is Enabled", IsEnabled, SetEnabled, bool, true, Urho3D::AM_DEFAULT);
    URHO3D_MIXED_ACCESSOR_ATTRIBUTE ("Map Min Point", GetMapMinPoint, SetMapMinPoint, Urho3D::Vector3, Urho3D::Vector3::ZERO, Urho3D::AM_DEFAULT);
    URHO3D_MIXED_ACCESSOR_ATTRIBUTE ("Map Max Point", GetMapMaxPoint, SetMapMaxPoint, Urho3D::Vector3, Urho3D::Vector3::ZERO, Urho3D::AM_DEFAULT);
}

void MapMaskUpdater::Update (Urho3D::StringHash eventType, Urho3D::VariantMap &eventData)
{
    if (enabled_ && node_ && node_->GetScene ()->IsUpdateEnabled ())
    {
        maskImage_->Clear (Urho3D::Color (1.0f, 1.0f, 1.0f, 1.0f));
        Map *map = node_->GetScene ()->GetChild ("map")->GetComponent <Map> ();
        assert (map);

        FogOfWarCalculator *fogOfWarCalculator = node_->GetScene ()->GetComponent <FogOfWarCalculator> ();
        assert (fogOfWarCalculator);

        // Create mask image with district borders and information about fog of war.
        for (int index = 0; index < map->GetDistrictsCount (); index++)
        {
            District *district = map->GetDistrictByIndex (index);
            DrawDistrictBorders (district);
            Urho3D::IntVector2 colonyPositionOnMap = WorldPointToMapPoint (district->GetColonyPosition ());
            // TODO: Later implement flood fill for political map.
            ImageUtils::FloodFill (maskImage_,
                                   fogOfWarCalculator->IsDistrictVisible (district->GetHash ()) ?
                                       MAP_MASK_VISIBLE_DISTRICT_COLOR : MAP_MASK_DISTRICT_UNDER_FOG_COLOR,
                                   colonyPositionOnMap.x_, colonyPositionOnMap.y_);
        }

        // Update mask texture.
        maskTexture_->SetData (maskImage_, false);

        // Flood image with district-specific colors for later determination which district is pressed by user.
        districtColorToDistrictHash_.Clear ();
        for (int index = 0; index < map->GetDistrictsCount (); index++)
        {
            District *district = map->GetDistrictByIndex (index);
            Urho3D::Color districtColor;
            do
                districtColor = Urho3D::Color (1.0f / (1.0f * Urho3D::Random (0, 256)),
                                               1.0f / (1.0f * Urho3D::Random (0, 256)),
                                               1.0f / (1.0f * Urho3D::Random (0, 256)),
                                               1.0f);
            while (districtColorToDistrictHash_.Keys ().Contains (Urho3D::StringHash (districtColor.ToUInt ())) ||
                   districtColor == MAP_MASK_DISTRICT_BORDER_LINE_COLOR ||
                   districtColor == MAP_MASK_VISIBLE_DISTRICT_COLOR ||
                   districtColor == MAP_MASK_DISTRICT_UNDER_FOG_COLOR);

            districtColorToDistrictHash_ [Urho3D::StringHash (districtColor.ToUInt ())] = district->GetHash ();
            Urho3D::IntVector2 colonyPositionOnMap = WorldPointToMapPoint (district->GetColonyPosition ());
            ImageUtils::FloodFill (maskImage_, districtColor, colonyPositionOnMap.x_, colonyPositionOnMap.y_);
        }
    }
}

Urho3D::StringHash MapMaskUpdater::GetDistrictByPoint (Urho3D::Vector3 point)
{
    Urho3D::IntVector2 mapPoint = WorldPointToMapPoint (point);
    Urho3D::Color color = maskImage_->GetPixel (mapPoint.x_, mapPoint.y_);

    if (color == MAP_MASK_DISTRICT_BORDER_LINE_COLOR)
        return Urho3D::StringHash::ZERO;
    else
        return districtColorToDistrictHash_ [Urho3D::StringHash (color.ToUInt ())];
}

Urho3D::Image *MapMaskUpdater::GetMaskImage () const
{
    return maskImage_.Get ();
}

Urho3D::Texture2D *MapMaskUpdater::GetMaskTexture () const
{
    return maskTexture_.Get ();
}

Urho3D::Vector3 MapMaskUpdater::GetMapMinPoint () const
{
    return mapMinPoint_;
}

void MapMaskUpdater::SetMapMinPoint (const Urho3D::Vector3 &mapMinPoint)
{
    assert (mapMinPoint.x_ < mapMaxPoint_.x_);
    assert (mapMinPoint.y_ < mapMaxPoint_.y_);
    assert (mapMinPoint.z_ < mapMaxPoint_.z_);
    mapMinPoint_ = mapMinPoint;
}

Urho3D::Vector3 MapMaskUpdater::GetMapMaxPoint () const
{
    return mapMaxPoint_;
}

void MapMaskUpdater::SetMapMaxPoint (const Urho3D::Vector3 &mapMaxPoint)
{
    assert (mapMaxPoint.x_ > mapMinPoint_.x_);
    assert (mapMaxPoint.y_ > mapMinPoint_.y_);
    assert (mapMaxPoint.z_ > mapMinPoint_.z_);
    mapMaxPoint_ = mapMaxPoint;
}
}

