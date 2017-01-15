#include <Colonization/BuildConfiguration.hpp>
#include "Unit.hpp"
#include <Urho3D/IO/Log.h>
#include <Urho3D/Core/Context.h>
#include <Urho3D/Graphics/DebugRenderer.h>
#include <Urho3D/Scene/Scene.h>

#include <Colonization/Backend/UnitsManager.hpp>
#include <Colonization/Core/Map.hpp>
#include <Colonization/Utils/Categories.hpp>
#include <Colonization/Utils/AttributeMacro.hpp>

namespace Colonization
{
const char *wayStructureElementsNames [] =
{
    "Waypoints Count",
    "   District Hash",
    0
};

const char *unitTypesNames [] =
{
    "Fleet",
    "Traders",
    "Colonizators",
    "Army",
    0
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

    URHO3D_ACCESSOR_ATTRIBUTE ("Is Enabled", IsEnabled, SetEnabled, bool, true, Urho3D::AM_DEFAULT);
    URHO3D_MIXED_ACCESSOR_ATTRIBUTE ("Hash", GetHash, SetHash, Urho3D::StringHash, Urho3D::StringHash ("nothing"), Urho3D::AM_DEFAULT);
    URHO3D_MIXED_ACCESSOR_ATTRIBUTE ("Owner Player Name", GetOwnerPlayerName, SetOwnerPlayerName, Urho3D::String, Urho3D::String ("Unit without owner"), Urho3D::AM_DEFAULT);
    URHO3D_ENUM_ACCESSOR_ATTRIBUTE ("Unit Type", GetUnitType, SetUnitType, UnitType, unitTypesNames, UNIT_FLEET, Urho3D::AM_DEFAULT);
    URHO3D_MIXED_ACCESSOR_ATTRIBUTE ("Position Hash", GetPositionHash, SetPositionHash, Urho3D::StringHash, Urho3D::StringHash ("nothing"), Urho3D::AM_DEFAULT);
    URHO3D_MIXED_ACCESSOR_VARIANT_VECTOR_STRUCTURE_ATTRIBUTE ("Way", GetWayAttribute, SetWayAttribute, Urho3D::VariantVector, Urho3D::Variant::emptyVariantVector, wayStructureElementsNames, Urho3D::AM_DEFAULT);
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

void Unit::DrawDebugGeometry (Urho3D::DebugRenderer *debug, bool depthTest)
{
    Map *map = node_->GetScene ()->GetChild ("map")->GetComponent <Map> ();
    assert (map);

    District *district = map->GetDistrictByHash (positionHash_);
    if (district)
    {
        Urho3D::Sphere sphere (district->GetUnitPosition (), 0.15f);
        debug->AddSphere (sphere, Urho3D::Color::GREEN, depthTest);
    }
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
    variantVector.Push (Urho3D::Variant (way_.Size ()));
    for (int index = 0; index < way_.Size (); index++)
        variantVector.Push (Urho3D::Variant (way_.At (index)));
    return variantVector;
}

void Unit::SetWayAttribute (const Urho3D::VariantVector &way)
{
    way_.Clear ();
    if (!way.Empty ())
    {
        int requestedSize = way.At (0).GetInt ();
        if (requestedSize > 0)
            for (int index = 0; index < requestedSize; index++)
            {
                if (index + 1 < way.Size ())
                    way_.Push (way.At (index + 1).GetStringHash ());
                else
                    way_.Push (Urho3D::StringHash ());
            }
    }
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
