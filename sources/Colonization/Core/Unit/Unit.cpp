#include <Colonization/BuildConfiguration.hpp>
#include "Unit.hpp"
#include <Urho3D/IO/Log.h>
#include <Urho3D/Core/Context.h>
#include <Urho3D/Graphics/DebugRenderer.h>
#include <Urho3D/Scene/Scene.h>

#include <Colonization/Core/Map.hpp>
#include <Colonization/Core/District/District.hpp>
#include <Colonization/Core/GameConfiguration.hpp>
#include <Colonization/Core/PlayerInfo/PlayerInfo.hpp>

#include <Colonization/Backend/UnitsManager.hpp>
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

const char *unitTypesNames [] =
{
    "Fleet",
    "Traders",
    "Colonizators",
    "Army",
    0
};

Unit::Unit (Urho3D::Context *context) : Urho3D::Component (context),
    hash_ (Urho3D::StringHash::ZERO),
    ownerPlayerName_ (Urho3D::StringHash::ZERO),
    unitType_ (UNIT_FLEET),
    positionHash_ (),
    way_ (),
    wayToNextDistrictProgressInPercents_ (0.0f),
    isInBattle_ (false),
    battleHash_ (Urho3D::StringHash::ZERO)
{

}

Unit::~Unit ()
{

}

void Unit::RegisterObject (Urho3D::Context *context)
{
    URHO3D_ACCESSOR_ATTRIBUTE ("Is Enabled", IsEnabled, SetEnabled, bool, true, Urho3D::AM_DEFAULT);
    URHO3D_MIXED_ACCESSOR_ATTRIBUTE ("Hash", GetHash, SetHash, Urho3D::StringHash, Urho3D::StringHash::ZERO, Urho3D::AM_DEFAULT);
    URHO3D_MIXED_ACCESSOR_ATTRIBUTE ("Owner Player Name", GetOwnerPlayerName, SetOwnerPlayerName, Urho3D::String, Urho3D::String ("Unit without owner"), Urho3D::AM_DEFAULT);
    URHO3D_ENUM_ACCESSOR_ATTRIBUTE ("Unit Type", GetUnitType, SetUnitType, UnitType, unitTypesNames, UNIT_FLEET, Urho3D::AM_DEFAULT);
    URHO3D_MIXED_ACCESSOR_ATTRIBUTE ("Position Hash", GetPositionHash, SetPositionHash, Urho3D::StringHash, Urho3D::StringHash ("nothing"), Urho3D::AM_DEFAULT);
    URHO3D_MIXED_ACCESSOR_VARIANT_VECTOR_STRUCTURE_ATTRIBUTE ("Way", GetWayAttribute, SetWayAttribute, Urho3D::VariantVector, Urho3D::Variant::emptyVariantVector, wayStructureElementsNames, Urho3D::AM_DEFAULT);
    URHO3D_ACCESSOR_ATTRIBUTE ("Way To Next District Progress In Percents",
                               GetWayToNextDistrictProgressInPercents,
                               SetWayToNextDistrictProgressInPercents,
                               float, 0.0f, Urho3D::AM_DEFAULT);
    URHO3D_ACCESSOR_ATTRIBUTE ("Is In Battle", GetIsInBattle, SetIsInBattle, bool, false, Urho3D::AM_DEFAULT);
    URHO3D_MIXED_ACCESSOR_ATTRIBUTE ("Battle Hash", GetBattleHash, SetBattleHash, Urho3D::StringHash, Urho3D::StringHash::ZERO, Urho3D::AM_DEFAULT);
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

    return (district->IsNeighborsWith (position) && !district->GetIsImpassable ());
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

bool Unit::GetIsInBattle () const
{
    return isInBattle_;
}

void Unit::SetIsInBattle (bool isInBattle)
{
    isInBattle_ = isInBattle;
}

Urho3D::StringHash Unit::GetBattleHash () const
{
    return battleHash_;
}

void Unit::SetBattleHash (const Urho3D::StringHash &battleHash)
{
    battleHash_ = battleHash;
}
}
