#pragma once
#include <Urho3D/Scene/Node.h>
#include <Urho3D/Scene/Component.h>
#include <Urho3D/Container/Vector.h>

namespace Colonization
{
class BattlesProcessor;
//@ASBindGen Constant
const Urho3D::String TAG_BATTLE ("Battle");
//@ASBindGen Class ObjectType=Ref
class Battle: public Urho3D::Component
{
URHO3D_OBJECT (Battle, Component)
protected:
    Urho3D::StringHash hash_;
    Urho3D::StringHash districtHash_;
    Urho3D::StringHash warHash_;
    Urho3D::PODVector <Urho3D::StringHash> defendersUnits_;
    Urho3D::PODVector <Urho3D::StringHash> attackersUnits_;

public:
    explicit Battle (Urho3D::Context *context);
    virtual ~Battle ();
    static void RegisterObject (Urho3D::Context *context);

    void UpdateHash (BattlesProcessor *owner);
    //@ASBindGen Function OverrideName=get_hash
    Urho3D::StringHash GetHash () const;
    //@ASBindGen Function OverrideName=set_hash
    void SetHash (const Urho3D::StringHash &hash);

    //@ASBindGen Function OverrideName=get_districtHash
    Urho3D::StringHash GetDistrictHash () const;
    //@ASBindGen Function OverrideName=set_districtHash
    void SetDistrictHash (const Urho3D::StringHash &districtHash);

    //@ASBindGen Function OverrideName=get_warHash
    Urho3D::StringHash GetWarHash () const;
    //@ASBindGen Function OverrideName=set_warHash
    void SetWarHash (const Urho3D::StringHash &warHash);

    //@ASBindGen Function OverrideName=get_attackersUnitsCount
    unsigned GetAttackersUnitsCount () const;
    //@ASBindGen Function
    Urho3D::StringHash GetAttackerUnitHashByIndex (int index) const;
    //@ASBindGen Function
    bool AddAttackerUnitHash (Urho3D::StringHash attackerUnitHash);
    //@ASBindGen Function
    bool RemoveAttackerUnitHash (Urho3D::StringHash attackerUnitHash);
    //@ASBindGen Function
    bool IsAttackerUnit (Urho3D::StringHash unitHash) const;
    //@ASBindGen Function
    Urho3D::PODVector <Urho3D::StringHash> GetAttackersUnitsList () const;

    //@ASBindGen Function OverrideName=get_defendersUnitsCount
    unsigned GetDefendersUnitsCount () const;
    //@ASBindGen Function
    Urho3D::StringHash GetDefenderUnitHashByIndex (int index) const;
    //@ASBindGen Function
    bool AddDefenderUnitHash (Urho3D::StringHash defenderUnitHash);
    //@ASBindGen Function
    bool RemoveDefenderUnitHash (Urho3D::StringHash defenderUnitHash);
    //@ASBindGen Function
    bool IsDefenderUnit (Urho3D::StringHash unitHash) const;
    //@ASBindGen Function
    Urho3D::PODVector <Urho3D::StringHash> GetDefendersUnitsList () const;

    Urho3D::VariantVector GetAttackersUnitsListAttribute () const;
    void SetAttackersUnitsListAttribute (const Urho3D::VariantVector &attackersUnitsList);

    Urho3D::VariantVector GetDefendersUnitsListAttribute () const;
    void SetDefendersUnitsListAttribute (const Urho3D::VariantVector &defendersUnitsList);
};
}
