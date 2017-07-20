#include "DiplomacyInternalPlayer.hpp"
#include <Urho3D/Input/Input.h>
#include <Urho3D/Core/CoreEvents.h>
#include <Urho3D/Scene/SceneEvents.h>
#include <Urho3D/IO/Log.h>
#include <Urho3D/Scene/Scene.h>

#include <Colonization/Backend/Diplomacy/DiplomacyOfferType.hpp>
#include <Colonization/Backend/Diplomacy/DiplomacyProcessor.hpp>
#include <Colonization/Backend/MessagesHandler/NetworkMessageType.hpp>

DiplomacyInternalPlayer::DiplomacyInternalPlayer (Urho3D::Context *context, Urho3D::String name, Urho3D::Color color, Urho3D::Scene *scene) :
    Colonization::Player (context, name, color, 0, scene),
    declinesCount_ (0)
{

}

DiplomacyInternalPlayer::~DiplomacyInternalPlayer ()
{

}

void DiplomacyInternalPlayer::SendMessage (int messageId, bool reliable, bool inOrder, const Urho3D::VectorBuffer &message, unsigned contentId)
{
    if (messageId == Colonization::STC_NETWORK_MESSAGE_DIPLOMACY_OFFER)
    {
        Urho3D::VectorBuffer messageData;
        messageData.SetData (message.GetBuffer ());
        Urho3D::StringHash offerType = messageData.ReadStringHash ();

        if (offerType == Colonization::DIPLOMACY_OFFER_PEACE)
        {
            unsigned offerId = messageData.ReadUInt ();
            if (declinesCount_ != 1)
            {
                Urho3D::VectorBuffer actionData;
                actionData.WriteInt (static_cast <int> (Colonization::PLAYER_ACTION_RESPONCE_TO_DIPLOMACY_OFFER));
                actionData.WriteUInt (offerId);

                if (declinesCount_ == 0)
                {
                    actionData.WriteUInt (static_cast <unsigned> (Colonization::DRPSTATUS_DECLINED));
                    declinesCount_++;
                }
                else
                {
                    actionData.WriteUInt (static_cast <unsigned> (Colonization::DRPSTATUS_ACCEPTED));
                }

                Urho3D::Pair <Colonization::PlayerActionType, Urho3D::Variant> action;
                action.first_ = Colonization::PLAYER_ACTION_RESPONCE_TO_DIPLOMACY_OFFER;
                action.second_ = Urho3D::Variant (actionData);
                AddAction (action);
            }
            else
            {
                declinesCount_++;
            }
        }
    }
}

void DiplomacyInternalPlayer::Disconnect (int wait)
{

}

bool DiplomacyInternalPlayer::IsInternal() const
{
    return true;
}
