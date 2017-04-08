#pragma once
#include <Urho3D/Scene/Component.h>
#include <Urho3D/Resource/Image.h>
#include <Urho3D/Graphics/Texture2D.h>
#include <Colonization/Frontend/FogOfWarCalculator.hpp>

namespace Colonization
{
class District;
class Map;

class MapMaskUpdater : public Urho3D::Component
{
URHO3D_OBJECT (MapMaskUpdater, Component)
protected:
    Urho3D::IntVector2 mapMaskSize_;
    int mapMaskComponents_;
    int districtBorderLineWidth_;
    float selectedDistrictColorModifer_;
    Urho3D::Color districtBorderLineColor_;
    Urho3D::Color visibleDistrictColor_;
    Urho3D::Color districtUnderFogColor_;

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

    Urho3D::StringHash lastMaskUpdateSelectedHash_;
    Urho3D::HashMap <Urho3D::StringHash, bool> lastMaskUpdateFogOfWarMap_;

    void DrawDistrictBorders (District *district, Urho3D::Image *image);
    void UpdateFogOfWarMask (Map *map, FogOfWarCalculator *fogOfWarCalculator);
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

    Urho3D::IntVector2 GetMapMaskSize () const;
    void SetMapMaskSize (const Urho3D::IntVector2 &mapMaskSize);

    int GetMapMaskComponents () const;
    void SetMapMaskComponents (int mapMaskComponents);

    int GetDistrictBorderLineWidth () const;
    void SetDistrictBorderLineWidth (int districtBorderLineWidth);

    float GetSelectedDistrictColorModifer () const;
    void SetSelectedDistrictColorModifer (float selectedDistrictColorModifer);

    Urho3D::Color GetDistrictBorderLineColor () const;
    void SetDistrictBorderLineColor (const Urho3D::Color &districtBorderLineColor);

    Urho3D::Color GetVisibleDistrictColor () const;
    void SetVisibleDistrictColor (const Urho3D::Color &visibleDistrictColor);

    Urho3D::Color GetDistrictUnderFogColor () const;
    void SetDistrictUnderFogColor (const Urho3D::Color &districtUnderFogColor);

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
