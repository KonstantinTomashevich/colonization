#pragma once
#include <Urho3D/Core/Object.h>
#include <Urho3D/Core/Attribute.h>
#include <Colonization/Core/Unit/Unit.hpp>

namespace Colonization
{
//@ASBindGen Class ObjectType=Ref
// TODO: Maybe add isSettleAtCurrentMoveTarget_? And add ability to rule colonizators from client.
class ColonizatorsUnit : public Unit
{
URHO3D_OBJECT (ColonizatorsUnit, Unit)
protected:
    float colonizatorsCount_;

public:
    ColonizatorsUnit (Urho3D::Context *context);
    virtual ~ColonizatorsUnit ();

    static void RegisterObject (Urho3D::Context *context);
    virtual void DrawDebugGeometry (Urho3D::DebugRenderer *debug, bool depthTest);

    //@ASBindGen Function OverrideName=get_colonizatorsCount
    float GetColonizatorsCount () const;
    //@ASBindGen Function OverrideName=set_colonizatorsCount
    void SetColonizatorsCount (float colonizatorsCount);

    virtual float GetBattleAttackForce (GameConfiguration *configuration, bool isNaval) const;
    /// Return true if unit is destroyed.
    virtual bool ApplyDamage (GameConfiguration *configuration, float damage);
    virtual Urho3D::String GetUnitTypeTag () const;
};
}
