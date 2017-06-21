#include <Colonization/BuildConfiguration.hpp>
#include "Battle.hpp"
#include <Urho3D/Core/Context.h>

#include <Colonization/Backend/BattlesProcessor.hpp>
#include <Colonization/Utils/Serialization/Categories.hpp>
#include <Colonization/Utils/Serialization/AttributeMacro.hpp>

namespace Colonization
{
const char *attackersUnitsListStructureElementsNames [] =
{
    "Attackers Units Count",
    "   Unit Hash",
    0
};

const char *defendersUnitsListStructureElementsNames [] =
{
    "Defenders Units Count",
    "   Unit Hash",
    0
};

Battle::Battle (Urho3D::Context *context) : Urho3D::Component (context),
    hash_ (),
    districtHash_ (),
    warHash_ (),
    attackersUnits_ (),
    defendersUnits_ ()
{

}

Battle::~Battle ()
{

}

void Battle::RegisterObject (Urho3D::Context *context)
{
    context->RegisterFactory <Battle> (COLONIZATION_CORE_CATEGORY);
    URHO3D_ACCESSOR_ATTRIBUTE ("Is Enabled", IsEnabled, SetEnabled, bool, true, Urho3D::AM_DEFAULT);
    URHO3D_MIXED_ACCESSOR_ATTRIBUTE ("Hash", GetHash, SetHash, Urho3D::StringHash, Urho3D::StringHash::ZERO, Urho3D::AM_DEFAULT);
    URHO3D_MIXED_ACCESSOR_ATTRIBUTE ("District Hash", GetDistrictHash, SetDistrictHash, Urho3D::StringHash, Urho3D::StringHash::ZERO, Urho3D::AM_DEFAULT);
    URHO3D_MIXED_ACCESSOR_ATTRIBUTE ("War Hash", GetWarHash, SetWarHash, Urho3D::StringHash, Urho3D::StringHash::ZERO, Urho3D::AM_DEFAULT);
    URHO3D_MIXED_ACCESSOR_VARIANT_VECTOR_STRUCTURE_ATTRIBUTE ("Attackers Units List", GetAttackersUnitsListAttribute, SetAttackersUnitsListAttribute, Urho3D::VariantVector, Urho3D::Variant::emptyVariantVector, attackersUnitsListStructureElementsNames, Urho3D::AM_DEFAULT);
    URHO3D_MIXED_ACCESSOR_VARIANT_VECTOR_STRUCTURE_ATTRIBUTE ("Defenders Units List", GetDefendersUnitsListAttribute, SetDefendersUnitsListAttribute, Urho3D::VariantVector, Urho3D::Variant::emptyVariantVector, defendersUnitsListStructureElementsNames, Urho3D::AM_DEFAULT);
}

void Battle::UpdateHash (BattlesProcessor *owner)
{
    do
    {
        hash_ = Urho3D::Random (0, 100) * districtHash_ * warHash_;
    }
    while (owner->GetBattleByHash (hash_) != this);
}

Urho3D::StringHash Battle::GetHash () const
{
    return hash_;
}

void Battle::SetHash (const Urho3D::StringHash &hash)
{
    hash_ = hash;
}

Urho3D::StringHash Battle::GetDistrictHash () const
{
    return districtHash_;
}

void Battle::SetDistrictHash (const Urho3D::StringHash &districtHash)
{
    districtHash_ = districtHash;
}

Urho3D::StringHash Battle::GetWarHash () const
{
    return warHash_;
}

void Battle::SetWarHash (const Urho3D::StringHash &warHash)
{
    warHash_ = warHash;
}

unsigned Battle::GetAttackersUnitsCount () const
{
    return attackersUnits_.Size ();
}

Urho3D::StringHash Battle::GetAttackerUnitHashByIndex (int index) const
{
    assert (index < attackersUnits_.Size ());
    return attackersUnits_.At (index);
}

bool Battle::AddAttackerUnitHash (Urho3D::StringHash attackerUnitHash)
{
    if (!attackersUnits_.Contains (attackerUnitHash))
    {
        attackersUnits_.Push (attackerUnitHash);
        return true;
    }
    else
    {
        return false;
    }
}

bool Battle::RemoveAttackerUnitHash (Urho3D::StringHash attackerUnitHash)
{
    return attackersUnits_.Remove (attackerUnitHash);
}

bool Battle::IsAttackerUnit (Urho3D::StringHash unitHash) const
{
    return attackersUnits_.Contains (unitHash);
}

Urho3D::PODVector <Urho3D::StringHash> Battle::GetAttackersUnitsList () const
{
    return attackersUnits_;
}

unsigned Battle::GetDefendersUnitsCount () const
{
    return defendersUnits_.Size ();
}

Urho3D::StringHash Battle::GetDefenderUnitHashByIndex (int index) const
{
    assert (index < defendersUnits_.Size ());
    return defendersUnits_.At (index);
}

bool Battle::AddDefenderUnitHash (Urho3D::StringHash defenderUnitHash)
{
    if (!defendersUnits_.Contains (defenderUnitHash))
    {
        defendersUnits_.Push (defenderUnitHash);
        return true;
    }
    else
    {
        return false;
    }
}

bool Battle::RemoveDefenderUnitHash (Urho3D::StringHash defenderUnitHash)
{
    return defendersUnits_.Remove (defenderUnitHash);
}

bool Battle::IsDefenderUnit (Urho3D::StringHash unitHash) const
{
    return defendersUnits_.Contains (unitHash);
}

Urho3D::PODVector <Urho3D::StringHash> Battle::GetDefendersUnitsList () const
{
    return defendersUnits_;
}

Urho3D::VariantVector Battle::GetAttackersUnitsListAttribute () const
{
    Urho3D::VariantVector variantVector;
    variantVector.Push (Urho3D::Variant (attackersUnits_.Size ()));
    for (int index = 0; index < attackersUnits_.Size (); index++)
    {
        variantVector.Push (Urho3D::Variant (attackersUnits_.At (index)));
    }
    return variantVector;
}

void Battle::SetAttackersUnitsListAttribute (const Urho3D::VariantVector &attackersUnitsList)
{
    attackersUnits_.Clear ();
    if (!attackersUnitsList.Empty ())
    {
        int requestedSize = attackersUnitsList.At (0).GetInt ();
        if (requestedSize > 0)
        {
            for (int index = 0; index < requestedSize; index++)
            {
                if (index + 1 < attackersUnitsList.Size ())
                {
                    attackersUnits_.Push (attackersUnitsList.At (index + 1).GetStringHash ());
                }
                else
                {
                    attackersUnits_.Push (Urho3D::StringHash ());
                }
            }
        }
    }
}

Urho3D::VariantVector Battle::GetDefendersUnitsListAttribute () const
{
    Urho3D::VariantVector variantVector;
    variantVector.Push (Urho3D::Variant (defendersUnits_.Size ()));
    for (int index = 0; index < defendersUnits_.Size (); index++)
    {
        variantVector.Push (Urho3D::Variant (defendersUnits_.At (index)));
    }
    return variantVector;
}

void Battle::SetDefendersUnitsListAttribute (const Urho3D::VariantVector &defendersUnitsList)
{
    defendersUnits_.Clear ();
    if (!defendersUnitsList.Empty ())
    {
        int requestedSize = defendersUnitsList.At (0).GetInt ();
        if (requestedSize > 0)
        {
            for (int index = 0; index < requestedSize; index++)
            {
                if (index + 1 < defendersUnitsList.Size ())
                {
                    defendersUnits_.Push (defendersUnitsList.At (index + 1).GetStringHash ());
                }
                else
                {
                    defendersUnits_.Push (Urho3D::StringHash ());
                }
            }
        }
    }
}
}
