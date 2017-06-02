#pragma once
#include <Urho3D/Core/Object.h>
#include <Urho3D/Core/Attribute.h>
#include <Colonization/Core/Unit/Unit.hpp>

namespace Colonization
{
//@ASBindGen Class ObjectType=Ref
class FleetUnit : public Unit
{
URHO3D_OBJECT (FleetUnit, Unit)
protected:
    int warShipsCount_;
    Urho3D::PODVector <float> warShipsHealthPoints_;

public:
    FleetUnit (Urho3D::Context *context);
    virtual ~FleetUnit ();

    static void RegisterObject (Urho3D::Context *context);
    virtual void DrawDebugGeometry (Urho3D::DebugRenderer *debug, bool depthTest);

    //@ASBindGen Function OverrideName=get_warShipsCount
    int GetWarShipsCount () const;
    //@ASBindGen Function OverrideName=set_warShipsCount
    void SetWarShipsCount (int warShipsCount);

    //@ASBindGen Function OverrideName=get_warShipsHealthPoints
    /// Any war ship health is in (0.0f, 100.0f].
    Urho3D::PODVector <float> GetWarShipsHealthPoints () const;
    //@ASBindGen Function OverrideName=set_warShipsHealthPoints
    void SetWarShipsHealthPoints (const Urho3D::PODVector <float> &warShipsHealthPoints);
    Urho3D::VariantVector GetWarShipsHealthPointsAttribute () const;
    void SetWarShipsHealthPointsAttribute (const Urho3D::VariantVector &warShipsHealthPoints);

    virtual float GetBattleAttackForce (GameConfiguration *configuration, bool isNaval) const;
    /// Return true if unit is destroyed.
    virtual bool ApplyDamage (GameConfiguration *configuration, float damage);
    virtual Urho3D::String GetUnitTypeTag () const;
};
}
