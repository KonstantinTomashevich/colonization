#include <Colonization/BuildConfiguration.hpp>
#include "Unit.hpp"
#include <Urho3D/IO/Log.h>
#include <Urho3D/Core/Context.h>
#include <Colonization/Backend/UnitsManager.hpp>
#include <Colonization/Utils/Categories.hpp>

namespace Colonization
{
const char *unitTypesNames [] =
{
    "Fleet",
    "Traders",
    "Colonizators",
    "Army",
};

Unit::Unit (Urho3D::Context *context) : Urho3D::Component (context),
    hash_ ("nothing"),
    ownerPlayerName_ ("???"),
    unitType_ (UNIT_FLEET),
    positionHash_ (),
    way_ (),
    wayToNextDistrictProgressInPercents_ (0.0f),
    unitTypeSpecificVars_ ()
{

}

Unit::~Unit ()
{

}

void Unit::RegisterObject (Urho3D::Context *context)
{
    context->RegisterFactory <Unit> (COLONIZATION_CORE_CATEGORY);
    using namespace Urho3D;

    URHO3D_MIXED_ACCESSOR_ATTRIBUTE ("Hash", GetHash, SetHash, StringHash, StringHash ("nothing"), AM_DEFAULT);
    URHO3D_MIXED_ACCESSOR_ATTRIBUTE ("Owner Player Name", GetOwnerPlayerName, SetOwnerPlayerName, String, String ("Unit without owner"), AM_DEFAULT);
    URHO3D_ENUM_ACCESSOR_ATTRIBUTE ("Unit Type", GetUnitType, SetUnitType, UnitType, unitTypesNames, UNIT_FLEET, AM_DEFAULT);
    URHO3D_MIXED_ACCESSOR_ATTRIBUTE ("Position Hash", GetPositionHash, SetPositionHash, StringHash, StringHash ("nothing"), AM_DEFAULT);
    URHO3D_MIXED_ACCESSOR_ATTRIBUTE ("Way", GetWayAttribute, SetWayAttribute, VariantVector, Variant::emptyVariantVector, AM_DEFAULT);
    URHO3D_ACCESSOR_ATTRIBUTE ("Way To Next District Progress In Percents",
                               GetWayToNextDistrictProgressInPercents,
                               SetWayToNextDistrictProgressInPercents,
                               float, 0.0f, AM_DEFAULT);

    URHO3D_ACCESSOR_ATTRIBUTE ("[Fleet Only] War Ships Count",
                               FleetUnitGetWarShipsCount,
                               FleetUnitSetWarShipsCount, int, 0, AM_DEFAULT);

    URHO3D_ACCESSOR_ATTRIBUTE ("[Traders Only] Trade Goods Cost",
                               TradersUnitGetTradeGoodsCost,
                               TradersUnitSetTradeGoodsCost, float, 0.0f, AM_DEFAULT);

    URHO3D_ACCESSOR_ATTRIBUTE ("[Colonizators Only] Colonizators Count",
                               ColonizatorsUnitGetColonizatorsCount,
                               ColonizatorsUnitSetColonizatorsCount, int, 0, AM_DEFAULT);

    URHO3D_ACCESSOR_ATTRIBUTE ("[Army Only] War Ships Count",
                               ArmyUnitGetSoldiersCount,
                               ArmyUnitSetSoldiersCount, int, 0, AM_DEFAULT);
}

void Unit::UpdateHash (UnitsManager *owner)
{
    do
        hash_ = Urho3D::StringHash (ownerPlayerName_ + Urho3D::String (static_cast <int> (unitType_)) +
                                    Urho3D::String (Urho3D::Random (0, 100000)));
    while (owner->GetUnitByHash (hash_) != this);
}

Urho3D::StringHash Unit::GetHash () const
{
    return hash_;
}

void Unit::SetHash (const Urho3D::StringHash &hash)
{
    hash_ = hash;
}

UnitType Unit::GetUnitType () const
{
    return unitType_;
}

void Unit::SetUnitType (UnitType unitType)
{
    unitType_ = unitType;
}

Urho3D::String Unit::GetOwnerPlayerName () const
{
    return ownerPlayerName_;
}

void Unit::SetOwnerPlayerName (const Urho3D::String &ownerPlayerName)
{
    ownerPlayerName_ = ownerPlayerName;
}

Urho3D::StringHash Unit::GetPositionHash () const
{
    return positionHash_;
}

void Unit::SetPositionHash (const Urho3D::StringHash &positionHash)
{
    positionHash_ = positionHash;
}

Urho3D::PODVector <Urho3D::StringHash> Unit::GetWay () const
{
    return way_;
}

void Unit::SetWay (Urho3D::PODVector <Urho3D::StringHash> way)
{
    way_ = way;
}

Urho3D::VariantVector Unit::GetWayAttribute () const
{
    Urho3D::VariantVector variantVector;
    for (int index = 0; index < way_.Size (); index++)
        variantVector.Push (Urho3D::Variant (way_.At (index)));
    return variantVector;
}

void Unit::SetWayAttribute (const Urho3D::VariantVector &way)
{
    way_.Clear ();
    if (!way.Empty ())
        for (int index = 0; index < way.Size (); index++)
            way_.Push (way.At (index).GetStringHash ());
}

float Unit::GetWayToNextDistrictProgressInPercents () const
{
    return wayToNextDistrictProgressInPercents_;
}

void Unit::SetWayToNextDistrictProgressInPercents (float wayToNextDistrictProgressInPercents)
{
    wayToNextDistrictProgressInPercents_ = wayToNextDistrictProgressInPercents;
}

int Unit::FleetUnitGetWarShipsCount () const
{
    Urho3D::Variant *value = unitTypeSpecificVars_ ["WarShipsCount"];
    if (value)
        return value->GetInt ();
    else
        return 0;
}

void Unit::FleetUnitSetWarShipsCount (int warShipsCount)
{
    unitTypeSpecificVars_ ["WarShipsCount"] = warShipsCount;
}

float Unit::TradersUnitGetTradeGoodsCost () const
{
    Urho3D::Variant *value = unitTypeSpecificVars_ ["TradeGoodsCost"];
    if (value)
        return value->GetFloat ();
    else
        return 0.0f;
}

void Unit::TradersUnitSetTradeGoodsCost (float tradeGoodsCost)
{
    unitTypeSpecificVars_ ["TradeGoodsCost"] = tradeGoodsCost;
}

int Unit::ColonizatorsUnitGetColonizatorsCount () const
{
    Urho3D::Variant *value = unitTypeSpecificVars_ ["ColonizatorsCount"];
    if (value)
        return value->GetInt ();
    else
        return 0;
}

void Unit::ColonizatorsUnitSetColonizatorsCount (int colonizatorsCount)
{
    unitTypeSpecificVars_ ["ColonizatorsCount"] = colonizatorsCount;
}

int Unit::ArmyUnitGetSoldiersCount () const
{
    Urho3D::Variant *value = unitTypeSpecificVars_ ["SoldiersCount"];
    if (value)
        return value->GetInt ();
    else
        return 0;
}

void Unit::ArmyUnitSetSoldiersCount (int soldiersCount)
{
    unitTypeSpecificVars_ ["SoldiersCount"] = soldiersCount;
}
}
