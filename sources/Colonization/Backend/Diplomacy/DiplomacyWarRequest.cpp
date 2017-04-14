#include <Colonization/BuildConfiguration.hpp>
#include "DiplomacyWarRequest.hpp"
#include <Urho3D/Core/Context.h>

#include <Colonization/Core/Diplomacy/DiplomacyWar.hpp>
#include <Colonization/Utils/Serialization/Categories.hpp>
#include <Colonization/Utils/Serialization/AttributeMacro.hpp>

namespace Colonization
{
DiplomacyWarRequest::DiplomacyWarRequest (Urho3D::Context *context) : DiplomacyRequest (context),
    attacker_ (),
    defender_ ()
{

}

DiplomacyWarRequest::~DiplomacyWarRequest ()
{

}

void DiplomacyWarRequest::RegisterObject (Urho3D::Context *context)
{
    context->RegisterFactory <DiplomacyWarRequest> (COLONIZATION_SERVER_ONLY_CATEGORY);
    URHO3D_COPY_BASE_ATTRIBUTES (DiplomacyRequest);

    URHO3D_MIXED_ACCESSOR_ATTRIBUTE ("Attacker", GetAttacker, SetAttacker, Urho3D::StringHash, Urho3D::StringHash::ZERO, Urho3D::AM_DEFAULT);
    URHO3D_MIXED_ACCESSOR_ATTRIBUTE ("Defender", GetDefender, SetDefender, Urho3D::StringHash, Urho3D::StringHash::ZERO, Urho3D::AM_DEFAULT);
}

Urho3D::StringHash DiplomacyWarRequest::GetAttacker () const
{
    return attacker_;
}

void DiplomacyWarRequest::SetAttacker (const Urho3D::StringHash &attacker)
{
    attacker_ = attacker;
}

Urho3D::StringHash DiplomacyWarRequest::GetDefender () const
{
    return defender_;
}

void DiplomacyWarRequest::SetDefender (const Urho3D::StringHash &defender)
{
    defender_ = defender;
}

bool DiplomacyWarRequest::UpdatePlayerStatus (DiplomacyActionsExecutor *diplomacyActionsExecutor, Urho3D::StringHash playerNameHash, DiplomacyRequestPlayerStatus status)
{
    return false;
}

bool DiplomacyWarRequest::TimeUpdate (DiplomacyActionsExecutor *diplomacyActionsExecutor, float timeStep)
{
    diplomacyActionsExecutor->AddPlayerEnemy (attacker_, defender_);
    diplomacyActionsExecutor->AddPlayerEnemy (defender_, attacker_);

    DiplomacyWar *war = diplomacyActionsExecutor->CreateWar ();
    war->AddAttackerNameHash (attacker_);
    war->AddDefenderNameHash (defender_);
    return true;
}
}