#include <Colonization/BuildConfiguration.hpp>
#include "Unit.hpp"
#include <Urho3D/IO/Log.h>
#include <Urho3D/Core/Context.h>
#include <Colonization/Backend/UnitsManager.hpp>
#include <Colonization/Utils/Categories.hpp>

namespace Colonization
{
static const char *unitTypesNames [] =
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
    position_ (0),
    way_ (0),
    wayToNextDistrictProgressInPercents_ (0),
    unitTypeSpecificVars_ ()
{

}

Unit::~Unit ()
{

}

void Unit::RegisterObject (Urho3D::Context *context)
{
    context->RegisterFactory <Unit> (COLONIZATION_CORE_CATEGORY);

    URHO3D_ACCESSOR_ATTRIBUTE ("Hash", GetHash, SetHash, Urho3D::StringHash, Urho3D::StringHash ("nothing"), Urho3D::AM_DEFAULT);
    URHO3D_ACCESSOR_ATTRIBUTE ("Owner Player Name", GetOwnerPlayerName, SetOwnerPlayerName, Urho3D::String, Urho3D::String ("Unit without owner"), Urho3D::AM_DEFAULT);
    URHO3D_ENUM_ACCESSOR_ATTRIBUTE ("Unit Type", GetUnitType, SetUnitType, UnitType, unitTypesNames, UNIT_FLEET, Urho3D::AM_DEFAULT);
    URHO3D_ACCESSOR_ATTRIBUTE ("Position Hash", GetPositionHash, SetPositionHash, Urho3D::StringHash, Urho3D::StringHash ("nothing"), Urho3D::AM_DEFAULT);
    URHO3D_ACCESSOR_ATTRIBUTE ("Way", GetWayAttribute, SetWayAttribute, Urho3D::VariantVector, Urho3D::Variant::emptyVariantVector, Urho3D::AM_DEFAULT);
    URHO3D_ACCESSOR_ATTRIBUTE ("Way To Next District Progress In Percents",
                               GetWayToNextDistrictProgressInPercents,
                               SetWayToNextDistrictProgressInPercents,
                               float, 0.0f, Urho3D::AM_DEFAULT);

    URHO3D_ACCESSOR_ATTRIBUTE ("[Fleet Only] War Ships Count",
                               FleetUnitGetWarShipsCount,
                               FleetUnitSetWarShipsCount, int, 0, Urho3D::AM_DEFAULT);

    URHO3D_ACCESSOR_ATTRIBUTE ("[Traders Only] Trade Goods Cost",
                               TradersUnitGetTradeGoodsCost,
                               TradersUnitSetTradeGoodsCost, float, 0.0f, Urho3D::AM_DEFAULT);

    URHO3D_ACCESSOR_ATTRIBUTE ("[Colonizators Only] Colonizators Count",
                               ColonizatorsUnitGetColonizatorsCount,
                               ColonizatorsUnitSetColonizatorsCount, int, 0, Urho3D::AM_DEFAULT);

    URHO3D_ACCESSOR_ATTRIBUTE ("[Army Only] War Ships Count",
                               ArmyUnitGetSoldiersCount,
                               ArmyUnitSetSoldiersCount, int, 0, Urho3D::AM_DEFAULT);
}

const Urho3D::Vector <Urho3D::AttributeInfo> *Unit::GetAttributes () const
{
    return currentAttributesList_;
}

void Unit::UpdateHash (UnitsManager *owner)
{
    do
        hash_ = Urho3D::StringHash (ownerPlayerName_ + Urho3D::String (static_cast <int> (unitType_)) +
                                    Urho3D::String (Urho3D::Random (0, 100000)));
    while (owner->GetUnitByHash (hash_) != this);
}

Urho3D::StringHash Unit::GetHash ()
{
    return hash_;
}

void Unit::SetHash (Urho3D::StringHash hash)
{
    hash_ = hash;
}

UnitType Unit::GetUnitType ()
{
    return unitType_;
}

void Unit::SetUnitType (UnitType unitType)
{
    unitType_ = unitType;
}

Urho3D::String Unit::GetOwnerPlayerName ()
{
    return ownerPlayerName_;
}

void Unit::SetOwnerPlayerName (Urho3D::String ownerPlayerName)
{
    ownerPlayerName_ = ownerPlayerName;
}

Urho3D::StringHash Unit::GetPositionHash ()
{
    return positionHash_;
}

void Unit::SetPositionHash (Urho3D::StringHash positionHash)
{
    positionHash_ = positionHash;
}

Urho3D::PODVector <Urho3D::StringHash> Unit::GetWay ()
{
    return way_;
}

void Unit::SetWay (Urho3D::PODVector <Urho3D::StringHash> way)
{
    way_ = way;
}

Urho3D::VariantVector Unit::GetWayAttribute ()
{
    Urho3D::VariantVector variantVector;
    if (!way_.Empty ())
        for (int index = 0; index < way_.Size (); index++)
            variantVector.Push (Urho3D::Variant (way_.At (index)));
    return variantVector;
}

void Unit::SetWayAttribute (Urho3D::VariantVector way)
{
    way_.Clear ();
    if (way.Size ())
        for (int index = 0; index < way.Size (); index++)
            way_.Push (way.At (index).GetStringHash ());
}

float Unit::GetWayToNextDistrictProgressInPercents ()
{
    return wayToNextDistrictProgressInPercents_;
}

void Unit::SetWayToNextDistrictProgressInPercents (float wayToNextDistrictProgressInPercents)
{
    wayToNextDistrictProgressInPercents_ = wayToNextDistrictProgressInPercents;
}

int Unit::FleetUnitGetWarShipsCount ()
{
    return unitTypeSpecificVars_ ["WarShipsCount"].GetInt ();
}

void Unit::FleetUnitSetWarShipsCount (int warShipsCount)
{
    unitTypeSpecificVars_ ["WarShipsCount"] = warShipsCount;
}

float Unit::TradersUnitGetTradeGoodsCost ()
{
    return unitTypeSpecificVars_ ["TradeGoodsCost"].GetFloat ();
}

void Unit::TradersUnitSetTradeGoodsCost (float tradeGoodsCost)
{
    unitTypeSpecificVars_ ["TradeGoodsCost"] = tradeGoodsCost;
}

int Unit::ColonizatorsUnitGetColonizatorsCount ()
{
    return unitTypeSpecificVars_ ["ColonizatorsCount"].GetInt ();
}

void Unit::ColonizatorsUnitSetColonizatorsCount (int colonizatorsCount)
{
    unitTypeSpecificVars_ ["ColonizatorsCount"] = colonizatorsCount;
}

int Unit::ArmyUnitGetSoldiersCount ()
{
    return unitTypeSpecificVars_ ["SoldiersCount"].GetInt ();
}

void Unit::ArmyUnitSetSoldiersCount (int soldiersCount)
{
    unitTypeSpecificVars_ ["SoldiersCount"] = soldiersCount;
}
}
