#pragma once
#include <Urho3D/Core/Object.h>
#include <Urho3D/Core/Attribute.h>
#include <Colonization/Core/Unit/Unit.hpp>

namespace Colonization
{
//@ASBindGen Class ObjectType=Ref
class TradersUnit : public Unit
{
URHO3D_OBJECT (TradersUnit, Unit)
protected:
    float tradeGoodsCost_;

public:
    explicit TradersUnit (Urho3D::Context *context);
    virtual ~TradersUnit ();

    static void RegisterObject (Urho3D::Context *context);
    virtual void DrawDebugGeometry (Urho3D::DebugRenderer *debug, bool depthTest);

    //@ASBindGen Function OverrideName=get_tradeGoodsCost
    float GetTradeGoodsCost () const;
    //@ASBindGen Function OverrideName=set_tradeGoodsCost
    void SetTradeGoodsCost (float tradeGoodsCost);

    virtual float GetBattleAttackForce (GameConfiguration *configuration, bool isNaval) const;
    /// Return true if unit is destroyed.
    virtual bool ApplyDamage (GameConfiguration *configuration, float damage);
    virtual bool IsAlive () const;
    virtual Urho3D::String GetUnitTypeTag () const;
};
}
