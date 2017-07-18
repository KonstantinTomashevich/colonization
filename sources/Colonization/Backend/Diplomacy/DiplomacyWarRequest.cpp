#include <Colonization/BuildConfiguration.hpp>
#include "DiplomacyWarRequest.hpp"
#include <Urho3D/Core/Context.h>

#include <Colonization/Core/Diplomacy/DiplomacyWar.hpp>
#include <Colonization/Core/Diplomacy/DiplomacyTags.hpp>
#include <Colonization/Backend/Diplomacy/DiplomacyRequestsUtils.hpp>
#include <Colonization/Backend/Diplomacy/DiplomacyInfoType.hpp>
#include <Colonization/Backend/Diplomacy/DiplomacyEvents.hpp>
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

void DiplomacyWarRequest::OnAddition ()
{
    DiplomacyWar *war = DiplomacyRequestsUtils::CreateWar (node_->GetScene ());
    war->AddAttackerNameHash (attacker_);
    war->AddDefenderNameHash (defender_);

    DiplomacyRequestsUtils::UpdatePlayerEnemies (node_->GetScene (), attacker_);
    DiplomacyRequestsUtils::UpdatePlayerEnemies (node_->GetScene (), defender_);

    Urho3D::VariantMap infoData;
    infoData [DiplomacyInfoWarStarted::ATTACKER] = Urho3D::Variant (attacker_);
    infoData [DiplomacyInfoWarStarted::DEFENDER] = Urho3D::Variant (defender_);
    DiplomacyRequestsUtils::SendDiplomacyInfoMessageToAllPlayers (node_->GetScene (),
                                                                  DIPLOMACY_INFO_WAR_STARTED,
                                                                  infoData);

    Urho3D::VariantMap eventData;
    eventData [WarStarted::WAR] = Urho3D::Variant (war);
    SendEvent (EVENT_WAR_STARTED, eventData);
}

bool DiplomacyWarRequest::UpdatePlayerStatus (Urho3D::StringHash playerNameHash, DiplomacyRequestPlayerStatus status)
{
    return false;
}

bool DiplomacyWarRequest::TimeUpdate (float timeStep)
{
    return true;
}

Urho3D::String DiplomacyWarRequest::GetRequestTypeTag () const
{
    return TAG_WAR;
}
}
