#pragma once
#include <Urho3D/Core/Object.h>
#include <Urho3D/Core/Attribute.h>
#include <Colonization/Core/Unit/Unit.hpp>

namespace Colonization
{
class ArmyUnit : public Unit
{
URHO3D_OBJECT (ArmyUnit, Unit)
protected:
    int soldiersCount_;

public:
    ArmyUnit (Urho3D::Context *context);
    virtual ~ArmyUnit ();

    static void RegisterObject (Urho3D::Context *context);
    virtual void DrawDebugGeometry (Urho3D::DebugRenderer *debug, bool depthTest);

    int GetSoldiersCount () const;
    void SetSoldiersCount (int soldiersCount);

    virtual float GetBattleAttackForce (GameConfiguration *configuration, bool isNaval) const;
    /// Return true if unit is destroyed.
    virtual bool ApplyDamage (GameConfiguration *configuration, float damage);
    virtual Urho3D::String GetUnitTypeTag () const;
};
}
