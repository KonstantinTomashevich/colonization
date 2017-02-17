#include <Colonization/BuildConfiguration.hpp>
#include "MapMaskUpdater.hpp"
#include <Urho3D/IO/Log.h>

#include <Urho3D/Scene/SceneEvents.h>
#include <Urho3D/Core/Context.h>
#include <Urho3D/Math/MathDefs.h>

#include <Urho3D/Resource/ResourceCache.h>
#include <Urho3D/Scene/Node.h>
#include <Urho3D/Scene/Scene.h>
#include <Urho3D/Graphics/Graphics.h>

#include <Colonization/Core/District.hpp>
#include <Colonization/Core/Map.hpp>
#include <Colonization/Frontend/FogOfWarCalculator.hpp>

#include <Colonization/Utils/Categories.hpp>
#include <Colonization/Utils/ImageUtils.hpp>
#include <Colonization/Utils/VectorUtils.hpp>
#include <Colonization/Utils/AttributeMacro.hpp>

namespace Colonization
{
Urho3D::IntVector2 MapMaskUpdater::WorldPointToMapPoint (Urho3D::Vector3 worldPoint) const
{
    Urho3D::Vector3 delta = worldPoint - mapMinPoint_;
    float mapWidth = mapMaxPoint_.x_ - mapMinPoint_.x_;
    float mapHeight = mapMaxPoint_.z_ - mapMinPoint_.z_;

    Urho3D::IntVector2 positionOnMap;
    positionOnMap.x_ = static_cast <int> (std::floor (MAP_MASK_WIDTH * delta.x_ / mapWidth));
    positionOnMap.y_ = MAP_MASK_HEIGHT - static_cast <int> (std::floor (MAP_MASK_HEIGHT * delta.z_ / mapHeight));
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
        {
            secondPoint = polygonPoints.At (index + 1);
        }
        else
        {
            secondPoint = polygonPoints.At (0);
        }

        Urho3D::IntVector2 firstOnMap = WorldPointToMapPoint (firstPoint);
        Urho3D::IntVector2 secondOnMap = WorldPointToMapPoint (secondPoint);

        ImageUtils::DrawLine (maskImage_, MAP_MASK_DISTRICT_BORDER_LINE_COLOR,
                              firstOnMap.x_, firstOnMap.y_,
                              secondOnMap.x_, secondOnMap.y_,
                              MAP_MASK_DISTRICT_BORDER_LINE_WIDTH);
    }
}

void MapMaskUpdater::OnSceneSet (Urho3D::Scene *scene)
{
    UnsubscribeFromAllEvents ();
    Urho3D::Component::OnSceneSet (scene);
    SubscribeToEvent (scene, Urho3D::E_SCENEUPDATE, URHO3D_HANDLER (MapMaskUpdater, Update));
}

MapMaskUpdater::MapMaskUpdater (Urho3D::Context *context) : Urho3D::Component (context),
    fogOfWarMaskImage_ (new Urho3D::Image (context)),
    fogOfWarMaskTexture_ (new Urho3D::Texture2D (context)),

    maskImage_ (new Urho3D::Image (context)),
    maskTexture_ (new Urho3D::Texture2D (context)),

    mapMinPoint_ (),
    mapMaxPoint_ (),
    districtColorToDistrictHash_ (),
    selectedDistrictHash_ ()
{
    maskImage_->SetSize (MAP_MASK_WIDTH, MAP_MASK_HEIGHT, MAP_MASK_COMPONENTS);
    maskTexture_->SetName ("maskTexture");
    maskTexture_->SetData (maskImage_, false);

    fogOfWarMaskImage_->SetSize (MAP_MASK_WIDTH, MAP_MASK_HEIGHT, MAP_MASK_COMPONENTS);
    fogOfWarMaskTexture_->SetName ("fogOfWarMaskTexture");
    fogOfWarMaskTexture_->SetData (fogOfWarMaskImage_, false);

    Urho3D::ResourceCache *resourceCache = context_->GetSubsystem <Urho3D::ResourceCache> ();
    resourceCache->AddManualResource (maskTexture_);
    resourceCache->AddManualResource (fogOfWarMaskTexture_);
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
    URHO3D_MIXED_ACCESSOR_ATTRIBUTE ("Selected District Hash", GetSelectedDistrictHash, SetSelectedDistrictHash, Urho3D::StringHash, Urho3D::StringHash::ZERO, Urho3D::AM_DEFAULT);
}

void MapMaskUpdater::Update (Urho3D::StringHash eventType, Urho3D::VariantMap &eventData)
{
    if (enabled_)
    {
        // Update packed fog of war data.
        Map *map = node_->GetScene ()->GetChild ("map")->GetComponent <Map> ();
        assert (map);
        fogOfWarMaskImage_->Clear (Urho3D::Color::WHITE);

        FogOfWarCalculator *fogOfWarCalculator = node_->GetScene ()->GetComponent <FogOfWarCalculator> ();
        assert (fogOfWarCalculator);

        Urho3D::HashMap <Urho3D::StringHash, unsigned> translateMap;
        translateMap [Urho3D::StringHash (MAP_MASK_DISTRICT_BORDER_LINE_COLOR.ToUInt ())] = MAP_MASK_DISTRICT_BORDER_LINE_COLOR.ToUInt ();
        for (int index = 0; index < districtColorToDistrictHash_.Size (); index++)
        {
            Urho3D::StringHash colorHash = districtColorToDistrictHash_.Keys ().At (index);
            Urho3D::StringHash districtHash = districtColorToDistrictHash_.Values ().At (index);
            Urho3D::Color color = fogOfWarCalculator->IsDistrictVisible (districtHash) ?
                        MAP_MASK_VISIBLE_DISTRICT_COLOR : MAP_MASK_DISTRICT_UNDER_FOG_COLOR;

            if (selectedDistrictHash_ == districtHash)
            {
                color.r_ *= MAP_MASK_SELECTED_DISTRICT_COLOR_MODIFER;
                color.g_ *= MAP_MASK_SELECTED_DISTRICT_COLOR_MODIFER;
                color.b_ *= MAP_MASK_SELECTED_DISTRICT_COLOR_MODIFER;
            }

            translateMap [colorHash] = color.ToUInt ();
        }

        for (int x = 0; x < fogOfWarMaskImage_->GetWidth (); x++)
        {
            for (int y = 0; y < fogOfWarMaskImage_->GetHeight (); y++)
            {
                unsigned sourceColor = maskImage_->GetPixelInt (x, y);
                unsigned resultColor = translateMap [Urho3D::StringHash (sourceColor)];
                fogOfWarMaskImage_->SetPixelInt (x, y, resultColor);
            }
        }

        fogOfWarMaskTexture_->SetData (fogOfWarMaskImage_, false);
    }
}

void MapMaskUpdater::RecalculateMaskImage ()
{
    maskImage_->Clear (Urho3D::Color (1.0f, 1.0f, 1.0f, 1.0f));
    Map *map = node_->GetScene ()->GetChild ("map")->GetComponent <Map> ();
    assert (map);

    FogOfWarCalculator *fogOfWarCalculator = node_->GetScene ()->GetComponent <FogOfWarCalculator> ();
    assert (fogOfWarCalculator);

    districtColorToDistrictHash_.Clear ();
    // Create mask image with district borders and flood districts areas with their special color.
    for (int index = 0; index < map->GetDistrictsCount (); index++)
    {
        District *district = map->GetDistrictByIndex (index);
        DrawDistrictBorders (district);
        Urho3D::Color districtColor;
        do
        {
            districtColor = Urho3D::Color (1.0f / (1.0f * Urho3D::Random (1, 256)),
                                           1.0f / (1.0f * Urho3D::Random (1, 256)),
                                           1.0f / (1.0f * Urho3D::Random (1, 256)),
                                           1.0f);
        }
        while (districtColorToDistrictHash_.Keys ().Contains (Urho3D::StringHash (districtColor.ToUInt ())) ||
               districtColor == MAP_MASK_DISTRICT_BORDER_LINE_COLOR ||
               districtColor == MAP_MASK_VISIBLE_DISTRICT_COLOR ||
               districtColor == MAP_MASK_DISTRICT_UNDER_FOG_COLOR);

        districtColorToDistrictHash_ [Urho3D::StringHash (districtColor.ToUInt ())] = district->GetHash ();
        Urho3D::IntVector2 unitPositionOnMap = WorldPointToMapPoint (district->GetUnitPosition ());
        ImageUtils::FloodFill (maskImage_, districtColor, unitPositionOnMap.x_, unitPositionOnMap.y_);
    }

    // Apply changes to mask texture.
    maskTexture_->SetData (maskImage_, false);
}

Urho3D::StringHash MapMaskUpdater::GetDistrictByPoint (Urho3D::Vector3 point) const
{
    Urho3D::IntVector2 mapPoint = WorldPointToMapPoint (point);
    int color = maskImage_->GetPixelInt (mapPoint.x_, mapPoint.y_);

    if (color == MAP_MASK_DISTRICT_BORDER_LINE_COLOR.ToUInt ())
    {
        return Urho3D::StringHash::ZERO;
    }
    else
    {
        return GetDistrictByColorInt (color);
    }
}

Urho3D::StringHash MapMaskUpdater::GetDistrictByColor (Urho3D::Color color) const
{
    return GetDistrictByColorInt (color.ToUInt ());
}

Urho3D::StringHash MapMaskUpdater::GetDistrictByColorInt (unsigned color) const
{
    Urho3D::StringHash *value = districtColorToDistrictHash_ [Urho3D::StringHash (color)];
    if (value)
    {
        return *value;
    }
    else
    {
        return Urho3D::StringHash::ZERO;
    }
}

unsigned MapMaskUpdater::GetDistrictColorInt (Urho3D::StringHash districtHash)
{
    Urho3D::Vector <Urho3D::StringHash> districtColorToDistrictHashValues = districtColorToDistrictHash_.Values ();
    int districtIndexInColorToDistrictMap = VectorUtils::GetElementIndex (districtColorToDistrictHashValues, districtHash);
    if (districtIndexInColorToDistrictMap != -1)
    {
        return districtColorToDistrictHash_.Keys ().At (districtIndexInColorToDistrictMap).Value ();
    }
    else
    {
        return 0;
    }
}

Urho3D::Image *MapMaskUpdater::GetFogOfWarMaskImage () const
{
    return fogOfWarMaskImage_;
}

Urho3D::Texture2D *MapMaskUpdater::GetFogOfWarMaskTexture () const
{
    return fogOfWarMaskTexture_;
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
    assert (mapMinPoint.x_ <= mapMaxPoint_.x_);
    assert (mapMinPoint.y_ <= mapMaxPoint_.y_);
    assert (mapMinPoint.z_ <= mapMaxPoint_.z_);
    mapMinPoint_ = mapMinPoint;
}

Urho3D::Vector3 MapMaskUpdater::GetMapMaxPoint () const
{
    return mapMaxPoint_;
}

void MapMaskUpdater::SetMapMaxPoint (const Urho3D::Vector3 &mapMaxPoint)
{
    assert (mapMaxPoint.x_ >= mapMinPoint_.x_);
    assert (mapMaxPoint.y_ >= mapMinPoint_.y_);
    assert (mapMaxPoint.z_ >= mapMinPoint_.z_);
    mapMaxPoint_ = mapMaxPoint;
}

Urho3D::StringHash MapMaskUpdater::GetSelectedDistrictHash () const
{
    return selectedDistrictHash_;
}

void MapMaskUpdater::SetSelectedDistrictHash (const Urho3D::StringHash &selectedDistrictHash)
{
    selectedDistrictHash_ = selectedDistrictHash;
}
}

