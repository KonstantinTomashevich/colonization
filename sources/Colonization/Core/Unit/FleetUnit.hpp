#pragma once
#include <Urho3D/Core/Object.h>
#include <Urho3D/Core/Attribute.h>
#include <Colonization/Core/Unit/Unit.hpp>

namespace Colonization
{
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

    int GetWarShipsCount () const;
    void SetWarShipsCount (int warShipsCount);

    /// Any war ship health is in (0.0f, 100.0f].
    Urho3D::PODVector <float> GetWarShipsHealthPoints () const;
    void SetWarShipsHealthPoints (const Urho3D::PODVector <float> &warShipsHealthPoints);
    Urho3D::VariantVector GetWarShipsHealthPointsAttribute () const;
    void SetWarShipsHealthPointsAttribute (const Urho3D::VariantVector &warShipsHealthPoints);

    virtual float GetBattleAttackForce (GameConfiguration *configuration, bool isNaval) const;
    /// Return true if unit is destroyed.
    virtual bool ApplyDamage (GameConfiguration *configuration, float damage);
    virtual Urho3D::String GetUnitTypeTag () const;
};
}
