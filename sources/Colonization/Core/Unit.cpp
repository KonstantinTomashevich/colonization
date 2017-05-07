#include <Colonization/BuildConfiguration.hpp>
#include "Unit.hpp"
#include <Urho3D/IO/Log.h>
#include <Urho3D/Core/Context.h>
#include <Urho3D/Graphics/DebugRenderer.h>
#include <Urho3D/Scene/Scene.h>

#include <Colonization/Backend/UnitsManager.hpp>
#include <Colonization/Core/Map.hpp>
#include <Colonization/Utils/Serialization/Categories.hpp>
#include <Colonization/Utils/Serialization/AttributeMacro.hpp>

namespace Colonization
{
const char *wayStructureElementsNames [] =
{
    "Waypoints Count",
    "   District Hash",
    0
};

const char *warShipsHealthPointsElementsNames [] =
{
    "War Ships Count:",
    "   War Ship HP",
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

    URHO3D_MIXED_ACCESSOR_VARIANT_VECTOR_STRUCTURE_ATTRIBUTE  ("[Fleet Only] War Ships Health Points",
                               FleetUnitGetWarShipsHealthPoints,
                               FleetUnitSetWarShipsHealthPoints, Urho3D::VariantVector, Urho3D::Variant::emptyVariantVector,
                               warShipsHealthPointsElementsNames, Urho3D::AM_DEFAULT);

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

bool Unit::IsCanGoTo (const District *district, const Map *map, Urho3D::StringHash imaginePosition) const
{
    Urho3D::StringHash position;
    if (imaginePosition == Urho3D::StringHash::ZERO)
    {
        position = positionHash_;
    }
    else
    {
        position = imaginePosition;
    }

    if (!district->IsNeighborsWith (position))
    {
        return false;
    }
    else if (unitType_ == UNIT_FLEET)
    {
        return district->IsSea () && !district->IsImpassable ();
    }
    // TODO: Current rule for army units is temporary!
    else if (unitType_ == UNIT_TRADERS || unitType_ == UNIT_ARMY)
    {
        return (district->IsSea () || (district->HasColony () && district->GetColonyOwnerName () == ownerPlayerName_))
                && !district->IsImpassable ();
    }
    else if (unitType_ == UNIT_COLONIZATORS)
    {
        if (district->IsSea () || (district->HasColony () && district->GetColonyOwnerName () == ownerPlayerName_))
        {
            return !district->IsImpassable ();
        }
        else if (!district->IsImpassable () && !district->HasColony ())
        {
            Urho3D::PODVector <Urho3D::StringHash> targetNeighbors = district->GetNeighborsHashes ();
            for (int index = 0; index < targetNeighbors.Size (); index++)
            {
                District *targetNeighbor = map->GetDistrictByHash (targetNeighbors.At (index));
                if ((targetNeighbor->IsSea () || (targetNeighbor->HasColony () &&
                                                  targetNeighbor->GetColonyOwnerName () == ownerPlayerName_))
                        && !targetNeighbor->IsImpassable ())
                {
                    return true;
                }
            }
            return false;
        }
        else
        {
            return false;
        }
    }
}

void Unit::UpdateHash (const UnitsManager *owner)
{
    do
    {
        hash_ = Urho3D::StringHash (ownerPlayerName_ + Urho3D::String (static_cast <int> (unitType_)) +
                                    Urho3D::String (Urho3D::Random (0, 100000)));
    }
    while (owner->GetUnitByHash (hash_) != this && hash_ != Urho3D::StringHash::ZERO);
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
    Urho3D::StringHash oldNextTarget;
    if (!way_.Empty ())
    {
        if (way_.At (0) == positionHash_)
        {
            if (way_.Size () > 1)
            {
                oldNextTarget = way_.At (1);
            }
            else
            {
                oldNextTarget = Urho3D::StringHash::ZERO;
            }
        }
        else
        {
            oldNextTarget = way_.At (0);
        }
    }

    Urho3D::StringHash newNextTarget;
    if (!way.Empty ())
    {
        if (way.At (0) == positionHash_)
        {
            if (way.Size () > 1)
            {
                newNextTarget = way.At (1);
            }
            else
            {
                newNextTarget = Urho3D::StringHash::ZERO;
            }
        }
        else
        {
            newNextTarget = way.At (0);
        }
    }

    if (oldNextTarget != newNextTarget ||
            oldNextTarget == Urho3D::StringHash::ZERO || newNextTarget == Urho3D::StringHash::ZERO)
    {
        wayToNextDistrictProgressInPercents_ = 0.0f;
    }
    way_ = way;
}

Urho3D::VariantVector Unit::GetWayAttribute () const
{
    Urho3D::VariantVector variantVector;
    variantVector.Push (Urho3D::Variant (way_.Size ()));
    for (int index = 0; index < way_.Size (); index++)
    {
        variantVector.Push (Urho3D::Variant (way_.At (index)));
    }
    return variantVector;
}

void Unit::SetWayAttribute (const Urho3D::VariantVector &way)
{
    Urho3D::PODVector <Urho3D::StringHash> newWay;
    if (!way.Empty ())
    {
        int requestedSize = way.At (0).GetInt ();
        if (requestedSize > 0)
        {
            for (int index = 0; index < requestedSize; index++)
            {
                if (index + 1 < way.Size ())
                {
                    newWay.Push (way.At (index + 1).GetStringHash ());
                }
                else
                {
                    newWay.Push (Urho3D::StringHash ());
                }
            }
        }
    }
    SetWay (newWay);
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
    {
        return value->GetInt ();
    }
    else
    {
        return 0;
    }
}

void Unit::FleetUnitSetWarShipsCount (int warShipsCount)
{
    unitTypeSpecificVars_ ["WarShipsCount"] = warShipsCount;
    Urho3D::VariantVector shipsHealthPoints = unitTypeSpecificVars_ ["WarShipsHealthPoints"].GetVariantVector ();
    if (shipsHealthPoints.Size () != warShipsCount)
    {
        if (shipsHealthPoints.Size () > warShipsCount)
        {
            shipsHealthPoints.Erase (warShipsCount, shipsHealthPoints.Size () - warShipsCount);
        }

        while (shipsHealthPoints.Size () < warShipsCount)
        {
            shipsHealthPoints.Push (100.0f);
        }
        unitTypeSpecificVars_ ["WarShipsHealthPoints"] = shipsHealthPoints;
    }
}

Urho3D::VariantVector Unit::FleetUnitGetWarShipsHealthPoints () const
{
    Urho3D::VariantVector shipsHealthPoints;
    shipsHealthPoints.Push (FleetUnitGetWarShipsCount ());
    Urho3D::Variant *savedArray = unitTypeSpecificVars_ ["WarShipsHealthPoints"];
    if (savedArray)
    {
        shipsHealthPoints.Push (savedArray->GetVariantVector ());
    }
    return shipsHealthPoints;
}

void Unit::FleetUnitSetWarShipsHealthPoints (const Urho3D::VariantVector &warShipsHealthPoints)
{
    Urho3D::VariantVector shipsHealthPoints;
    int shipsCount = FleetUnitGetWarShipsCount ();
    for (int index = 0; index < shipsCount; index++)
    {
        if (index + 1 < warShipsHealthPoints.Size ())
        {
            shipsHealthPoints.Push (warShipsHealthPoints.At (index + 1));
        }
        else
        {
            shipsHealthPoints.Push (100.0f);
        }
    }
    unitTypeSpecificVars_ ["WarShipsHealthPoints"] = shipsHealthPoints;
}

float Unit::TradersUnitGetTradeGoodsCost () const
{
    Urho3D::Variant *value = unitTypeSpecificVars_ ["TradeGoodsCost"];
    if (value)
    {
        return value->GetFloat ();
    }
    else
    {
        return 0.0f;
    }
}

void Unit::TradersUnitSetTradeGoodsCost (float tradeGoodsCost)
{
    unitTypeSpecificVars_ ["TradeGoodsCost"] = tradeGoodsCost;
}

int Unit::ColonizatorsUnitGetColonizatorsCount () const
{
    Urho3D::Variant *value = unitTypeSpecificVars_ ["ColonizatorsCount"];
    if (value)
    {
        return value->GetInt ();
    }
    else
    {
        return 0;
    }
}

void Unit::ColonizatorsUnitSetColonizatorsCount (int colonizatorsCount)
{
    unitTypeSpecificVars_ ["ColonizatorsCount"] = colonizatorsCount;
}

int Unit::ArmyUnitGetSoldiersCount () const
{
    Urho3D::Variant *value = unitTypeSpecificVars_ ["SoldiersCount"];
    if (value)
    {
        return value->GetInt ();
    }
    else
    {
        return 0;
    }
}

void Unit::ArmyUnitSetSoldiersCount (int soldiersCount)
{
    unitTypeSpecificVars_ ["SoldiersCount"] = soldiersCount;
}
}
