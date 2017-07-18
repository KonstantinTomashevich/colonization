#pragma once
#include <Urho3D/Scene/Component.h>
namespace Colonization
{
class District;
//@ASBindGen Class ObjectType=Ref
class FogOfWarCalculator : public Urho3D::Component
{
URHO3D_OBJECT (FogOfWarCalculator, Component)
public:
    explicit FogOfWarCalculator (Urho3D::Context *context);
    virtual ~FogOfWarCalculator ();

    static void RegisterObject (Urho3D::Context *context);
    void Update (Urho3D::StringHash eventType, Urho3D::VariantMap &eventData);

    //@ASBindGen Function OverrideName=get_playerName
    Urho3D::String GetPlayerName () const;
    //@ASBindGen Function OverrideName=set_playerName
    void SetPlayerName (const Urho3D::String &playerName);

    //@ASBindGen Function
    bool IsDistrictVisible (Urho3D::StringHash districtHash);
    Urho3D::HashMap <Urho3D::StringHash, bool> GetFogOfWarMap ();

protected:
    virtual void OnSceneSet (Urho3D::Scene* scene);

private:
    Urho3D::String playerName_;
    // If true -- district visible.
    Urho3D::HashMap <Urho3D::StringHash, bool> fogOfWarMap_;
    void OpenDistrictAndNeighbors (District *district);
};
}
