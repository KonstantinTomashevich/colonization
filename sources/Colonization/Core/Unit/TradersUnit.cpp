#include <Colonization/BuildConfiguration.hpp>
#include "TradersUnit.hpp"
#include <Urho3D/IO/Log.h>
#include <Urho3D/Core/Context.h>
#include <Urho3D/Graphics/DebugRenderer.h>
#include <Urho3D/Scene/Scene.h>

#include <Colonization/Core/Map.hpp>
#include <Colonization/Core/District/District.hpp>
#include <Colonization/Core/Unit/UnitTags.hpp>
#include <Colonization/Core/Unit/UnitEvents.hpp>
#include <Colonization/Core/GameConfiguration.hpp>

#include <Colonization/Backend/UnitsManager.hpp>
#include <Colonization/Utils/Serialization/Categories.hpp>
#include <Colonization/Utils/Serialization/AttributeMacro.hpp>

namespace Colonization
{
TradersUnit::TradersUnit (Urho3D::Context *context) : Unit (context),
    tradeGoodsCost_ (0.0f)
{
    unitType_ = UNIT_TRADERS;
}

TradersUnit::~TradersUnit ()
{

}

void TradersUnit::RegisterObject (Urho3D::Context *context)
{
    context->RegisterFactory <TradersUnit> (COLONIZATION_CORE_CATEGORY);
    URHO3D_COPY_BASE_ATTRIBUTES (Unit);
    URHO3D_ACCESSOR_ATTRIBUTE ("Trade Goods Cost", GetTradeGoodsCost, SetTradeGoodsCost, float, 0.0f, Urho3D::AM_DEFAULT);
}

void TradersUnit::DrawDebugGeometry (Urho3D::DebugRenderer *debug, bool depthTest)
{
    Map *map = node_->GetScene ()->GetChild ("map")->GetComponent <Map> ();
    assert (map);

    District *district = map->GetDistrictByHash (GetPositionHash ());
    if (district)
    {
        Urho3D::Sphere sphere (district->GetUnitPosition (), 0.15f);
        debug->AddSphere (sphere, Urho3D::Color::YELLOW, depthTest);
    }
}

float TradersUnit::GetTradeGoodsCost () const
{
    return tradeGoodsCost_;
}

void TradersUnit::SetTradeGoodsCost (float tradeGoodsCost)
{
    tradeGoodsCost_ = tradeGoodsCost;
}

bool TradersUnit::IsCanGoTo (const District *district, const Map *map, Urho3D::StringHash imaginePosition) const
{
    if (Unit::IsCanGoTo (district, map, imaginePosition))
    {
        return (district->GetIsSea () || (district->GetHasColony () && district->GetColonyOwnerName () == GetOwnerPlayerName ()));
    }
    else
    {
        return false;
    }
}

float TradersUnit::GetBattleAttackForce (GameConfiguration *configuration, bool isNaval) const
{
    if (isNaval)
    {
        return configuration->GetTraderBasicNavalAttackForce () * tradeGoodsCost_;
    }
    else
    {
        return configuration->GetTraderBasicLandAttackForce () * tradeGoodsCost_;
    }
}

bool TradersUnit::ApplyDamage (GameConfiguration *configuration, float damage)
{
    float losses = damage / configuration->GetTraderBasicHealth ();
    tradeGoodsCost_ -= losses;

    Urho3D::VariantMap eventData;
    eventData [TradersUnitLossesGold::UNIT_HASH] = GetHash ();
    eventData [TradersUnitLossesGold::BATTLE_HASH] = GetBattleHash ();
    eventData [TradersUnitLossesGold::GOLD_AMOUNT] = losses;

    SendEvent (EVENT_TRADERS_UNIT_LOSSES_GOLD, eventData);
    return IsAlive ();
}

bool TradersUnit::IsAlive () const
{
    return tradeGoodsCost_ > 0.0f;
}

Urho3D::String TradersUnit::GetUnitTypeTag () const
{
    return TAG_TRADERS;
}
}
