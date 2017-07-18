#pragma once
#include <Urho3D/Scene/Component.h>

namespace Colonization
{
class DiplomacyProcessor;
//@ASBindGen Class ObjectType=Ref
class DiplomacyWar : public Urho3D::Component
{
URHO3D_OBJECT (DiplomacyWar, Component)
public:
    explicit DiplomacyWar (Urho3D::Context* context);
    virtual ~DiplomacyWar ();
    static void RegisterObject (Urho3D::Context *context);

    void UpdateHash (DiplomacyProcessor *owner);
    //@ASBindGen Function OverrideName=get_hash
    Urho3D::StringHash GetHash () const;
    //@ASBindGen Function OverrideName=set_hash
    void SetHash (const Urho3D::StringHash &hash);

    //@ASBindGen Function OverrideName=get_conflictEscalation
    float GetConflictEscalation () const;
    //@ASBindGen Function OverrideName=set_conflictEscalation
    void SetConflictEscalation (float conflictEscalation);

    //@ASBindGen Function OverrideName=get_attackersCount
    unsigned GetAttackersCount () const;
    //@ASBindGen Function
    Urho3D::StringHash GetAttackerNameHashByIndex (int index) const;
    //@ASBindGen Function
    bool AddAttackerNameHash (Urho3D::StringHash attackerNameHash);
    //@ASBindGen Function
    bool RemoveAttackerNameHash (Urho3D::StringHash attackerNameHash);
    //@ASBindGen Function
    bool IsAttacker (Urho3D::StringHash nameHash) const;
    //@ASBindGen Function
    Urho3D::PODVector <Urho3D::StringHash> GetAttackersList () const;

    //@ASBindGen Function OverrideName=get_defendersCount
    unsigned GetDefendersCount () const;
    //@ASBindGen Function
    Urho3D::StringHash GetDefenderNameHashByIndex (int index) const;
    //@ASBindGen Function
    bool AddDefenderNameHash (Urho3D::StringHash defenderNameHash);
    //@ASBindGen Function
    bool RemoveDefenderNameHash (Urho3D::StringHash defenderNameHash);
    //@ASBindGen Function
    bool IsDefender (Urho3D::StringHash nameHash) const;
    //@ASBindGen Function
    Urho3D::PODVector <Urho3D::StringHash> GetDefendersList () const;

    Urho3D::VariantVector GetAttackersListAttribute () const;
    void SetAttackersListAttribute (const Urho3D::VariantVector &attackersList);

    Urho3D::VariantVector GetDefendersListAttribute () const;
    void SetDefendersListAttribute (const Urho3D::VariantVector &defendersList);

private:
    Urho3D::StringHash hash_;
    Urho3D::PODVector <Urho3D::StringHash> attackersList_;
    Urho3D::PODVector <Urho3D::StringHash> defendersList_;
    /// [0.0f, 100.0f]
    float conflictEscalation_;
};
}
