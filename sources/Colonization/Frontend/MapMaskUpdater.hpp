#pragma once
#include <Urho3D/Scene/Component.h>
#include <Urho3D/Resource/Image.h>
#include <Urho3D/Graphics/Texture2D.h>

namespace Colonization
{
class District;
const int MAP_MASK_WIDTH = 256;
const int MAP_MASK_HEIGHT = 256;
const int MAP_MASK_COMPONENTS = 4;
const int MAP_MASK_DISTRICT_BORDER_LINE_WIDTH = 1;
const float MAP_MASK_SELECTED_DISTRICT_COLOR_MODIFER = 1.0f / 0.9f;

const Urho3D::Color MAP_MASK_DISTRICT_BORDER_LINE_COLOR (0.1f, 0.1f, 0.1f, 1.0f);
const Urho3D::Color MAP_MASK_VISIBLE_DISTRICT_COLOR (0.9f, 0.9f, 0.9f, 1.0f);
const Urho3D::Color MAP_MASK_DISTRICT_UNDER_FOG_COLOR (0.35f, 0.35f, 0.35f, 1.0f);

class MapMaskUpdater : public Urho3D::Component
{
URHO3D_OBJECT (MapMaskUpdater, Component)
protected:
    /// Used for transporting fog of war data into shader.
    /// Size will be (districts-count * 2, 1).
    /// Content: [pixel 2n] = district-color (from special map); [pixel 2n + 1] = color which modifies districts' colors.
    Urho3D::SharedPtr <Urho3D::Image> packedFogOfWarImage_;
    Urho3D::SharedPtr <Urho3D::Texture2D> packedFogOfWarTexture_;

    /// Used for determinating which district is on this point. Also contains districts borders lines.
    Urho3D::SharedPtr <Urho3D::Image> maskImage_;
    Urho3D::SharedPtr <Urho3D::Texture2D> maskTexture_;

    Urho3D::Vector3 mapMinPoint_;
    Urho3D::Vector3 mapMaxPoint_;
    Urho3D::HashMap <Urho3D::StringHash, Urho3D::StringHash> districtColorToDistrictHash_;
    Urho3D::StringHash selectedDistrictHash_;

    Urho3D::IntVector2 WorldPointToMapPoint (Urho3D::Vector3 worldPoint);
    void DrawDistrictBorders (District *district);

public:
    MapMaskUpdater (Urho3D::Context *context);
    virtual ~MapMaskUpdater ();

    static void RegisterObject (Urho3D::Context *context);
    void Update (Urho3D::StringHash eventType, Urho3D::VariantMap &eventData);

    void RecalculateMaskImage ();
    Urho3D::StringHash GetDistrictByPoint (Urho3D::Vector3 point);

    Urho3D::Image *GetPackedFogOfWarImage () const;
    Urho3D::Texture2D *GetPackedFogOfWarTexture () const;

    Urho3D::Image *GetMaskImage () const;
    Urho3D::Texture2D *GetMaskTexture () const;

    Urho3D::Vector3 GetMapMinPoint () const;
    void SetMapMinPoint (const Urho3D::Vector3 &mapMinPoint);

    Urho3D::Vector3 GetMapMaxPoint () const;
    void SetMapMaxPoint (const Urho3D::Vector3 &mapMaxPoint);

    Urho3D::StringHash GetSelectedDistrictHash () const;
    void SetSelectedDistrictHash (const Urho3D::StringHash &selectedDistrictHash);
};
}
