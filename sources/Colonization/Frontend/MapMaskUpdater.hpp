#pragma once
#include <Urho3D/Scene/Component.h>
#include <Urho3D/Resource/Image.h>
#include <Urho3D/Graphics/Texture2D.h>

namespace Colonization
{
class District;
const int MAP_MASK_WIDTH = 2048;
const int MAP_MASK_HEIGHT = 2048;
const int MAP_MASK_DISTRICT_BORDER_LINE_WIDTH = 3;

const Urho3D::Color MAP_MASK_DISTRICT_BORDER_LINE_COLOR (0.1f, 0.1f, 0.1f, 1.0f);
const Urho3D::Color MAP_MASK_VISIBLE_DISTRICT_COLOR (1.0f, 1.0f, 1.0f, 1.0f);
const Urho3D::Color MAP_MASK_DISTRICT_UNDER_FOG_COLOR (0.35f, 0.35f, 0.35f, 1.0f);

class MapMaskUpdater : public Urho3D::Component
{
URHO3D_OBJECT (MapMaskUpdater, Component)
protected:
    Urho3D::SharedPtr <Urho3D::Image> maskImage_;
    Urho3D::SharedPtr <Urho3D::Texture2D> maskTexture_;

    Urho3D::Vector3 mapMinPoint_;
    Urho3D::Vector3 mapMaxPoint_;
    Urho3D::HashMap <Urho3D::StringHash, Urho3D::StringHash> districtColorToDistrictHash_;

    Urho3D::IntVector2 WorldPointToMapPoint (Urho3D::Vector3 worldPoint);
    void DrawDistrictBorders (District *district);

public:
    MapMaskUpdater (Urho3D::Context *context);
    virtual ~MapMaskUpdater ();

    static void RegisterObject (Urho3D::Context *context);
    void Update (Urho3D::StringHash eventType, Urho3D::VariantMap &eventData);
    Urho3D::StringHash GetDistrictByPoint (Urho3D::Vector3 point);

    Urho3D::Image *GetMaskImage () const;
    Urho3D::Texture2D *GetMaskTexture () const;

    Urho3D::Vector3 GetMapMinPoint () const;
    void SetMapMinPoint (const Urho3D::Vector3 &mapMinPoint);

    Urho3D::Vector3 GetMapMaxPoint () const;
    void SetMapMaxPoint (const Urho3D::Vector3 &mapMaxPoint);
};
}
