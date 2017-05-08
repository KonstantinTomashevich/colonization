#pragma once
#include <Urho3D/Core/Object.h>
#include <Urho3D/Core/Attribute.h>
#include <Colonization/Core/Unit/Unit.hpp>

namespace Colonization
{
class TradersUnit : public Unit
{
URHO3D_OBJECT (TradersUnit, Unit)
protected:
    float tradeGoodsCost_;

public:
    TradersUnit (Urho3D::Context *context);
    virtual ~TradersUnit ();

    static void RegisterObject (Urho3D::Context *context);
    virtual void DrawDebugGeometry (Urho3D::DebugRenderer *debug, bool depthTest);

    float GetTradeGoodsCost () const;
    void SetTradeGoodsCost (float tradeGoodsCost);

    virtual float GetBattleAttackForce (GameConfiguration *configuration, bool isNaval) const;
    /// Return true if unit is destroyed.
    virtual bool ApplyDamage (GameConfiguration *configuration, float damage);
    virtual Urho3D::String GetUnitTypeTag () const;
};
}
