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
public:
    explicit ColonizatorsUnit (Urho3D::Context *context);
    virtual ~ColonizatorsUnit ();

    static void RegisterObject (Urho3D::Context *context);
    virtual void DrawDebugGeometry (Urho3D::DebugRenderer *debug, bool depthTest);

    //@ASBindGen Function OverrideName=get_colonizatorsCount
    float GetColonizatorsCount () const;
    //@ASBindGen Function OverrideName=set_colonizatorsCount
    void SetColonizatorsCount (float colonizatorsCount);

    virtual bool IsCanGoTo (const District *district, const Map *map, Urho3D::StringHash imaginePosition = Urho3D::StringHash::ZERO) const;
    virtual float GetBattleAttackForce (GameConfiguration *configuration, bool isNaval) const;
    /// Return true if unit is destroyed.
    virtual bool ApplyDamage (GameConfiguration *configuration, float damage);
    virtual bool IsAlive () const;
    virtual Urho3D::String GetUnitTypeTag () const;

private:
    float colonizatorsCount_;
};
}
