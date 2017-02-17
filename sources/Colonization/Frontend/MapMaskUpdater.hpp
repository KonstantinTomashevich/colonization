#pragma once
#include <Urho3D/Scene/Component.h>
#include <Urho3D/Resource/Image.h>
#include <Urho3D/Graphics/Texture2D.h>

namespace Colonization
{
class District;
const int MAP_MASK_WIDTH = 512;
const int MAP_MASK_HEIGHT = 512;
const int MAP_MASK_COMPONENTS = 4;
const int MAP_MASK_DISTRICT_BORDER_LINE_WIDTH = 1;
const float MAP_MASK_SELECTED_DISTRICT_COLOR_MODIFER = 1.0f / 0.75f;

const Urho3D::Color MAP_MASK_DISTRICT_BORDER_LINE_COLOR (0.10f, 0.10f, 0.10f, 1.00f);
const Urho3D::Color MAP_MASK_VISIBLE_DISTRICT_COLOR (0.75f, 0.75f, 0.75f, 1.00f);
const Urho3D::Color MAP_MASK_DISTRICT_UNDER_FOG_COLOR (0.30f, 0.30f, 0.30f, 1.00f);

class MapMaskUpdater : public Urho3D::Component
{
URHO3D_OBJECT (MapMaskUpdater, Component)
protected:
    /// Rendered fog of war mask.
    Urho3D::SharedPtr <Urho3D::Image> fogOfWarMaskImage_;
    Urho3D::SharedPtr <Urho3D::Texture2D> fogOfWarMaskTexture_;

    /// Used for determinating which district is on this point. Also contains districts borders lines.
    Urho3D::SharedPtr <Urho3D::Image> maskImage_;
    Urho3D::SharedPtr <Urho3D::Texture2D> maskTexture_;

    Urho3D::Vector3 mapMinPoint_;
    Urho3D::Vector3 mapMaxPoint_;
    Urho3D::HashMap <Urho3D::StringHash, Urho3D::StringHash> districtColorToDistrictHash_;
    Urho3D::StringHash selectedDistrictHash_;

    void DrawDistrictBorders (District *district);
    virtual void OnSceneSet (Urho3D::Scene* scene);

public:
    MapMaskUpdater (Urho3D::Context *context);
    virtual ~MapMaskUpdater ();

    static void RegisterObject (Urho3D::Context *context);
    void Update (Urho3D::StringHash eventType, Urho3D::VariantMap &eventData);

    void RecalculateMaskImage ();
    Urho3D::StringHash GetDistrictByPoint (Urho3D::Vector3 point) const;
    Urho3D::StringHash GetDistrictByColor (Urho3D::Color color) const;
    Urho3D::StringHash GetDistrictByColorInt (unsigned color) const;

    Urho3D::IntVector2 WorldPointToMapPoint (Urho3D::Vector3 worldPoint) const;
    unsigned GetDistrictColorInt (Urho3D::StringHash districtHash);

    Urho3D::Image *GetFogOfWarMaskImage () const;
    Urho3D::Texture2D *GetFogOfWarMaskTexture () const;

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
