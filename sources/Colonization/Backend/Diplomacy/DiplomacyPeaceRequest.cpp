#include <Colonization/BuildConfiguration.hpp>
#include "DiplomacyPeaceRequest.hpp"
#include <Urho3D/Core/Context.h>

#include <Colonization/Core/Diplomacy/DiplomacyWar.hpp>
#include <Colonization/Backend/Diplomacy/DiplomacyRequestsUtils.hpp>
#include <Colonization/Backend/Diplomacy/DiplomacyOfferType.hpp>
#include <Colonization/Backend/Diplomacy/DiplomacyInfoType.hpp>
#include <Colonization/Utils/Serialization/Categories.hpp>
#include <Colonization/Utils/Serialization/AttributeMacro.hpp>

namespace Colonization
{
void DiplomacyPeaceRequest::ProcessAcceptedResult ()
{
    Urho3D::VariantMap infoData;
    infoData [DiplomacyInfoPeaceTreatyAccepted::SENDER] = Urho3D::Variant (peaceRequester_);
    infoData [DiplomacyInfoPeaceTreatyAccepted::ANOTHER_PLAYER] = Urho3D::Variant (enemy_);
    DiplomacyRequestsUtils::SendDiplomacyInfoMessageToAllPlayers (node_->GetScene (),
                                                                  DIPLOMACY_INFO_PEACE_TREATY_ACCEPTED,
                                                                  infoData);

    DiplomacyRequestsUtils::RemoveWarByHash (node_->GetScene (), warHash_);
    DiplomacyRequestsUtils::UpdatePlayerEnemies (node_->GetScene (), peaceRequester_);
    DiplomacyRequestsUtils::UpdatePlayerEnemies (node_->GetScene (), enemy_);
}

void DiplomacyPeaceRequest::ProcessDeclinedResult ()
{
    Urho3D::VariantMap infoData;
    infoData [DiplomacyInfoPeaceTreatyDeclined::SENDER] = Urho3D::Variant (peaceRequester_);
    infoData [DiplomacyInfoPeaceTreatyDeclined::ANOTHER_PLAYER] = Urho3D::Variant (enemy_);
    DiplomacyRequestsUtils::SendDiplomacyInfoMessageToAllPlayers (node_->GetScene (),
                                                                  DIPLOMACY_INFO_PEACE_TREATY_DECLINED,
                                                                  infoData);
}

DiplomacyPeaceRequest::DiplomacyPeaceRequest (Urho3D::Context *context) : DiplomacyRequest (context),
    warHash_ (),
    peaceRequester_ (),
    enemy_ (),
    // TODO: This var should be initialized by special value from GameConfiguration.
    untilAutodecline_ (30.0f)
{

}

DiplomacyPeaceRequest::~DiplomacyPeaceRequest ()
{

}

void DiplomacyPeaceRequest::RegisterObject (Urho3D::Context *context)
{
    context->RegisterFactory <DiplomacyPeaceRequest> (COLONIZATION_SERVER_ONLY_CATEGORY);
    URHO3D_COPY_BASE_ATTRIBUTES (DiplomacyRequest);

    URHO3D_MIXED_ACCESSOR_ATTRIBUTE ("WarHash", GetWarHash, SetWarHash, Urho3D::StringHash, Urho3D::StringHash::ZERO, Urho3D::AM_DEFAULT);
    URHO3D_MIXED_ACCESSOR_ATTRIBUTE ("Peace Requester", GetPeaceRequester, SetPeaceRequester, Urho3D::StringHash, Urho3D::StringHash::ZERO, Urho3D::AM_DEFAULT);
    URHO3D_MIXED_ACCESSOR_ATTRIBUTE ("Enemy", GetEnemy, SetEnemy, Urho3D::StringHash, Urho3D::StringHash::ZERO, Urho3D::AM_DEFAULT);
    URHO3D_ACCESSOR_ATTRIBUTE ("Until Autodecline", GetTimeUntilAutodecline, SetTimeUntilAutodecline, float, 30.0f, Urho3D::AM_DEFAULT);
}

Urho3D::StringHash DiplomacyPeaceRequest::GetWarHash () const
{
    return warHash_;
}

void DiplomacyPeaceRequest::SetWarHash (const Urho3D::StringHash &warHash)
{
    warHash_ = warHash;
}

Urho3D::StringHash DiplomacyPeaceRequest::GetPeaceRequester () const
{
    return peaceRequester_;
}

void DiplomacyPeaceRequest::SetPeaceRequester (const Urho3D::StringHash &peaceRequester)
{
    peaceRequester_ = peaceRequester;
}

Urho3D::StringHash DiplomacyPeaceRequest::GetEnemy () const
{
    return enemy_;
}

void DiplomacyPeaceRequest::SetEnemy (const Urho3D::StringHash &enemy)
{
    enemy_ = enemy;
}

float DiplomacyPeaceRequest::GetTimeUntilAutodecline () const
{
    return untilAutodecline_;
}

void DiplomacyPeaceRequest::SetTimeUntilAutodecline (float untilAutodecline)
{
    untilAutodecline_ = untilAutodecline;
}

void DiplomacyPeaceRequest::OnAddition ()
{
    Urho3D::PODVector <Urho3D::StringHash> recieviers;
    recieviers.Push (enemy_);
    Urho3D::VariantMap offerData;
    offerData [DiplomacyOfferPeace::ENEMY_NAME_HASH] = Urho3D::Variant (peaceRequester_);
    DiplomacyRequestsUtils::SendDiplomacyOfferMessage (node_->GetScene (),
                                                       DIPLOMACY_OFFER_PEACE,
                                                       requestId_,
                                                       untilAutodecline_,
                                                       offerData,
                                                       recieviers);
}

bool DiplomacyPeaceRequest::UpdatePlayerStatus (Urho3D::StringHash playerNameHash, DiplomacyRequestPlayerStatus status)
{
    if (playerNameHash == enemy_)
    {
        if (status == DRPSTATUS_ACCEPTED)
        {
            ProcessAcceptedResult ();
            return true;
        }
        else if (status == DRPSTATUS_DECLINED)
        {
            ProcessDeclinedResult ();
            return true;
        }
        else
        {
            return false;
        }
    }
    else
    {
        return false;
    }
}

bool DiplomacyPeaceRequest::TimeUpdate (float timeStep)
{
    untilAutodecline_ -= timeStep;
    if (untilAutodecline_ <= 0.0f)
    {
        ProcessDeclinedResult ();
        return true;
    }
    else
    {
        return false;
    }
}
}
