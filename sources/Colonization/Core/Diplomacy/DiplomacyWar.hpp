#pragma once
#include <Urho3D/Scene/Component.h>

namespace Colonization
{
class DiplomacyProcessor;
class DiplomacyWar : public Urho3D::Component
{
URHO3D_OBJECT (DiplomacyWar, Component)
protected:
    Urho3D::StringHash hash_;
    Urho3D::PODVector <Urho3D::StringHash> attackersList_;
    Urho3D::PODVector <Urho3D::StringHash> defendersList_;
    float conflictEscalation_;

public:
    DiplomacyWar (Urho3D::Context* context);
    virtual ~DiplomacyWar ();
    static void RegisterObject (Urho3D::Context *context);

    void UpdateHash (DiplomacyProcessor *owner);
    Urho3D::StringHash GetHash () const;
    void SetHash (const Urho3D::StringHash &hash);

    float GetConflictEscalation () const;
    void SetConflictEscalation (float conflictEscalation);

    int GetAttackersCount () const;
    Urho3D::StringHash GetAttackerNameHashByIndex (int index) const;
    bool AddAttackerNameHash (Urho3D::StringHash attackerNameHash);
    bool RemoveAttackerNameHash (Urho3D::StringHash attackerNameHash);
    Urho3D::PODVector <Urho3D::StringHash> GetAttackersList () const;

    int GetDefendersCount () const;
    Urho3D::StringHash GetDefenderNameHashByIndex (int index) const;
    bool AddDefenderNameHash (Urho3D::StringHash defenderNameHash);
    bool RemoveDefenderNameHash (Urho3D::StringHash defenderNameHash);
    Urho3D::PODVector <Urho3D::StringHash> GetDefendersList () const;

    Urho3D::VariantVector GetAttackersListAttribute () const;
    void SetAttackersListAttribute (const Urho3D::VariantVector &attackersList);

    Urho3D::VariantVector GetDefendersListAttribute () const;
    void SetDefendersListAttribute (const Urho3D::VariantVector &defendersList);
};
}

