#include <Colonization/BuildConfiguration.hpp>
#include "DiplomacyWar.hpp"
#include <Urho3D/Core/Context.h>

#include <Colonization/Backend/Diplomacy/DiplomacyProcessor.hpp>
#include <Colonization/Utils/Serialization/Categories.hpp>
#include <Colonization/Utils/Serialization/AttributeMacro.hpp>

namespace Colonization
{
const char *attackersListStructureElementsNames [] =
{
    "Attackers Count",
    "   Player Name Hash",
    0
};

const char *defendersListStructureElementsNames [] =
{
    "Defenders Count",
    "   Player Name Hash",
    0
};

DiplomacyWar::DiplomacyWar (Urho3D::Context* context) : Urho3D::Component(context),
    hash_ (),
    attackersList_ (),
    defendersList_ (),
    conflictEscalation_ (0.0f)
{

}

DiplomacyWar::~DiplomacyWar ()
{

}

void DiplomacyWar::RegisterObject (Urho3D::Context *context)
{
    context->RegisterFactory <DiplomacyWar> (COLONIZATION_CORE_CATEGORY);

    URHO3D_ACCESSOR_ATTRIBUTE ("Is Enabled", IsEnabled, SetEnabled, bool, true, Urho3D::AM_DEFAULT);
    URHO3D_MIXED_ACCESSOR_ATTRIBUTE ("Hash", GetHash, SetHash, Urho3D::StringHash, Urho3D::StringHash::ZERO, Urho3D::AM_DEFAULT);

    URHO3D_ACCESSOR_ATTRIBUTE ("Conflict Escalation", GetConflictEscalation, SetConflictEscalation, float, 0.0f, Urho3D::AM_DEFAULT);
    URHO3D_MIXED_ACCESSOR_VARIANT_VECTOR_STRUCTURE_ATTRIBUTE ("Attackers List", GetAttackersListAttribute, SetAttackersListAttribute, Urho3D::VariantVector, Urho3D::Variant::emptyVariantVector, attackersListStructureElementsNames, Urho3D::AM_DEFAULT);
    URHO3D_MIXED_ACCESSOR_VARIANT_VECTOR_STRUCTURE_ATTRIBUTE ("Defenders List", GetDefendersListAttribute, SetDefendersListAttribute, Urho3D::VariantVector, Urho3D::Variant::emptyVariantVector, defendersListStructureElementsNames, Urho3D::AM_DEFAULT);
}

void DiplomacyWar::UpdateHash (DiplomacyProcessor *owner)
{
    do
    {
        hash_ = Urho3D::StringHash (Urho3D::Random (0, 100000) * (attackersList_.Size () + 1) * (defendersList_.Size () + 1));
    }
    while (owner->GetWarByHash (hash_) != this && hash_ != Urho3D::StringHash::ZERO);
}

Urho3D::StringHash DiplomacyWar::GetHash () const
{
    return hash_;
}

void DiplomacyWar::SetHash (const Urho3D::StringHash &hash)
{
    hash_ = hash;
}

float DiplomacyWar::GetConflictEscalation () const
{
    return conflictEscalation_;
}

void DiplomacyWar::SetConflictEscalation (float conflictEscalation)
{
    conflictEscalation_ = conflictEscalation;
}

unsigned DiplomacyWar::GetAttackersCount () const
{
    return attackersList_.Size ();
}

Urho3D::StringHash DiplomacyWar::GetAttackerNameHashByIndex (int index) const
{
    assert (index < attackersList_.Size ());
    return attackersList_.At (index);
}

bool DiplomacyWar::AddAttackerNameHash (Urho3D::StringHash attackerNameHash)
{
    if (!attackersList_.Contains (attackerNameHash))
    {
        attackersList_.Push (attackerNameHash);
        return true;
    }
    else
    {
        return false;
    }
}

bool DiplomacyWar::RemoveAttackerNameHash (Urho3D::StringHash attackerNameHash)
{
    return attackersList_.Remove (attackerNameHash);
}

bool DiplomacyWar::IsAttacker (Urho3D::StringHash nameHash) const
{
    return attackersList_.Contains (nameHash);
}

Urho3D::PODVector <Urho3D::StringHash> DiplomacyWar::GetAttackersList () const
{
    return attackersList_;
}

unsigned DiplomacyWar::GetDefendersCount () const
{
    return defendersList_.Size ();
}

Urho3D::StringHash DiplomacyWar::GetDefenderNameHashByIndex (int index) const
{
    assert (index < defendersList_.Size ());
    return defendersList_.At (index);
}

bool DiplomacyWar::AddDefenderNameHash (Urho3D::StringHash defenderNameHash)
{
    if (!defendersList_.Contains (defenderNameHash))
    {
        defendersList_.Push (defenderNameHash);
        return true;
    }
    else
    {
        return false;
    }
}

bool DiplomacyWar::RemoveDefenderNameHash (Urho3D::StringHash defenderNameHash)
{
    return defendersList_.Remove (defenderNameHash);
}

bool DiplomacyWar::IsDefender (Urho3D::StringHash nameHash) const
{
    return defendersList_.Contains (nameHash);
}

Urho3D::PODVector <Urho3D::StringHash> DiplomacyWar::GetDefendersList () const
{
    return defendersList_;
}

Urho3D::VariantVector DiplomacyWar::GetAttackersListAttribute () const
{
    Urho3D::VariantVector variantVector;
    variantVector.Push (Urho3D::Variant (attackersList_.Size ()));
    for (int index = 0; index < attackersList_.Size (); index++)
    {
        variantVector.Push (Urho3D::Variant (attackersList_.At (index)));
    }
    return variantVector;
}

void DiplomacyWar::SetAttackersListAttribute (const Urho3D::VariantVector &attackersList)
{
    attackersList_.Clear ();
    if (!attackersList.Empty ())
    {
        int requestedSize = attackersList.At (0).GetInt ();
        if (requestedSize > 0)
        {
            for (int index = 0; index < requestedSize; index++)
            {
                if (index + 1 < attackersList.Size ())
                {
                    attackersList_.Push (attackersList.At (index + 1).GetStringHash ());
                }
                else
                {
                    attackersList_.Push (Urho3D::StringHash ());
                }
            }
        }
    }
}

Urho3D::VariantVector DiplomacyWar::GetDefendersListAttribute () const
{
    Urho3D::VariantVector variantVector;
    variantVector.Push (Urho3D::Variant (defendersList_.Size ()));
    for (int index = 0; index < defendersList_.Size (); index++)
    {
        variantVector.Push (Urho3D::Variant (defendersList_.At (index)));
    }
    return variantVector;
}

void DiplomacyWar::SetDefendersListAttribute (const Urho3D::VariantVector &defendersList)
{
    defendersList_.Clear ();
    if (!defendersList.Empty ())
    {
        int requestedSize = defendersList.At (0).GetInt ();
        if (requestedSize > 0)
        {
            for (int index = 0; index < requestedSize; index++)
            {
                if (index + 1 < defendersList.Size ())
                {
                    defendersList_.Push (defendersList.At (index + 1).GetStringHash ());
                }
                else
                {
                    defendersList_.Push (Urho3D::StringHash ());
                }
            }
        }
    }
}
}
