#include <Colonization/BuildConfiguration.hpp>
#include "DiplomacyWarRequest.hpp"
#include <Colonization/Core/Diplomacy/DiplomacyWar.hpp>

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

void DiplomacyWarRequest::Init (Urho3D::StringHash attacker, Urho3D::StringHash defender)
{
    attacker_ = attacker;
    defender_ = defender;
}

void DiplomacyWarRequest::UpdatePlayerStatus (DiplomacyActionsExecutor *diplomacyActionsExecutor, Urho3D::StringHash playerNameHash, DiplomacyRequestPlayerStatus status)
{

}

void DiplomacyWarRequest::TimeUpdate (DiplomacyActionsExecutor *diplomacyActionsExecutor, float timeStep)
{
    diplomacyActionsExecutor->AddPlayerEnemy (attacker_, defender_);
    diplomacyActionsExecutor->AddPlayerEnemy (defender_, attacker_);

    DiplomacyWar *war = diplomacyActionsExecutor->CreateWar ();
    war->AddAttackerNameHash (attacker_);
    war->AddDefenderNameHash (defender_);
    isFinished_ = true;
}
}
