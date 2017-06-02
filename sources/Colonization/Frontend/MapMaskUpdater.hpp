#pragma once
#include <Urho3D/Scene/Component.h>
#include <Urho3D/Resource/Image.h>
#include <Urho3D/Graphics/Texture2D.h>
#include <Colonization/Frontend/FogOfWarCalculator.hpp>

namespace Colonization
{
class District;
class Map;

//@ASBindGen Class ObjectType=Ref
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

    //@ASBindGen Function
    void RecalculateMaskImage ();
    //@ASBindGen Function
    Urho3D::StringHash GetDistrictByPoint (Urho3D::Vector3 point) const;
    //@ASBindGen Function
    Urho3D::StringHash GetDistrictByColor (Urho3D::Color color) const;
    //@ASBindGen Function
    Urho3D::StringHash GetDistrictByColorInt (unsigned color) const;

    //@ASBindGen Function
    Urho3D::IntVector2 WorldPointToMapPoint (Urho3D::Vector3 worldPoint) const;
    //@ASBindGen Function
    unsigned GetDistrictColorInt (Urho3D::StringHash districtHash);

    //@ASBindGen Function OverrideName=get_mapMaskSize
    Urho3D::IntVector2 GetMapMaskSize () const;
    //@ASBindGen Function OverrideName=set_mapMaskSize
    void SetMapMaskSize (const Urho3D::IntVector2 &mapMaskSize);

    //@ASBindGen Function OverrideName=get_mapMaskComponents
    int GetMapMaskComponents () const;
    //@ASBindGen Function OverrideName=set_mapMaskComponents
    void SetMapMaskComponents (int mapMaskComponents);

    //@ASBindGen Function OverrideName=get_districtBorderLineWidth
    int GetDistrictBorderLineWidth () const;
    //@ASBindGen Function OverrideName=set_districtBorderLineWidth
    void SetDistrictBorderLineWidth (int districtBorderLineWidth);

    //@ASBindGen Function OverrideName=get_selectedDistrictColorModifer
    float GetSelectedDistrictColorModifer () const;
    //@ASBindGen Function OverrideName=set_selectedDistrictColorModifer
    void SetSelectedDistrictColorModifer (float selectedDistrictColorModifer);

    //@ASBindGen Function OverrideName=get_districtBorderLineColor
    Urho3D::Color GetDistrictBorderLineColor () const;
    //@ASBindGen Function OverrideName=set_districtBorderLineColor
    void SetDistrictBorderLineColor (const Urho3D::Color &districtBorderLineColor);

    //@ASBindGen Function OverrideName=get_visibleDistrictColor
    Urho3D::Color GetVisibleDistrictColor () const;
    //@ASBindGen Function OverrideName=set_visibleDistrictColor
    void SetVisibleDistrictColor (const Urho3D::Color &visibleDistrictColor);

    //@ASBindGen Function OverrideName=get_districtUnderFogColor
    Urho3D::Color GetDistrictUnderFogColor () const;
    //@ASBindGen Function OverrideName=set_districtUnderFogColor
    void SetDistrictUnderFogColor (const Urho3D::Color &districtUnderFogColor);

    //@ASBindGen Function AddRef_arg-1
    Urho3D::Image *GetFogOfWarMaskImage () const;
    //@ASBindGen Function AddRef_arg-1
    Urho3D::Texture2D *GetFogOfWarMaskTexture () const;

    //@ASBindGen Function AddRef_arg-1
    Urho3D::Image *GetMaskImage () const;
    //@ASBindGen Function AddRef_arg-1
    Urho3D::Texture2D *GetMaskTexture () const;

    //@ASBindGen Function OverrideName=get_mapMinPoint
    Urho3D::Vector3 GetMapMinPoint () const;
    //@ASBindGen Function OverrideName=set_mapMinPoint
    void SetMapMinPoint (const Urho3D::Vector3 &mapMinPoint);

    //@ASBindGen Function OverrideName=get_mapMaxPoint
    Urho3D::Vector3 GetMapMaxPoint () const;
    //@ASBindGen Function OverrideName=set_mapMaxPoint
    void SetMapMaxPoint (const Urho3D::Vector3 &mapMaxPoint);

    //@ASBindGen Function OverrideName=get_selectedDistrictHash
    Urho3D::StringHash GetSelectedDistrictHash () const;
    //@ASBindGen Function OverrideName=set_selectedDistrictHash
    void SetSelectedDistrictHash (const Urho3D::StringHash &selectedDistrictHash);
};
}
