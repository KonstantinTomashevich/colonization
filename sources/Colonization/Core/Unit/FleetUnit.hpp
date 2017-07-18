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
public:
    explicit FleetUnit (Urho3D::Context *context);
    virtual ~FleetUnit ();

    static void RegisterObject (Urho3D::Context *context);
    virtual void DrawDebugGeometry (Urho3D::DebugRenderer *debug, bool depthTest);

    //@ASBindGen Function OverrideName=get_warShipsCount
    int GetWarShipsCount () const;
    //@ASBindGen Function OverrideName=set_warShipsCount
    void SetWarShipsCount (int warShipsCount);

    //@ASBindGen Function OverrideName=get_warShipsHealthPoints
    Urho3D::PODVector <float> GetWarShipsHealthPoints () const;
    //@ASBindGen Function OverrideName=set_warShipsHealthPoints ReplaceInType_arg0=&| ReplaceInType_arg0=const|
    void SetWarShipsHealthPoints (const Urho3D::PODVector <float> &warShipsHealthPoints);
    Urho3D::VariantVector GetWarShipsHealthPointsAttribute () const;
    void SetWarShipsHealthPointsAttribute (const Urho3D::VariantVector &warShipsHealthPoints);

    virtual bool IsCanGoTo (const District *district, const Map *map, Urho3D::StringHash imaginePosition = Urho3D::StringHash::ZERO) const;
    virtual float GetBattleAttackForce (GameConfiguration *configuration, bool isNaval) const;
    /// Return true if unit is destroyed.
    virtual bool ApplyDamage (GameConfiguration *configuration, float damage);
    virtual bool IsAlive () const;
    virtual Urho3D::String GetUnitTypeTag () const;

private:
    int warShipsCount_;
    /// Any war ship health is in (0.0f, 100.0f].
    Urho3D::PODVector <float> warShipsHealthPoints_;
};
}
