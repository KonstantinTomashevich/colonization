#include <Colonization/BuildConfiguration.hpp>
#include "MessagesHandler.hpp"
#include <Urho3D/Core/Context.h>
#include <Urho3D/Scene/Node.h>
#include <Urho3D/Scene/Scene.h>
#include <Urho3D/Network/NetworkEvents.h>
#include <Urho3D/Network/Connection.h>
#include <Urho3D/IO/Log.h>

#include <Colonization/Backend/MessagesHandler/NetworkMessageType.hpp>
#include <Colonization/Backend/PlayersManager.hpp>
#include <Colonization/Utils/Serialization/Categories.hpp>
#include <Colonization/Utils/Serialization/AttributeMacro.hpp>

namespace Colonization
{
void MessagesHandler::ProcessSendChatMessageInput (Player *player, Urho3D::VectorBuffer &messageData)
{
    PlayersManager *playersManager = node_->GetScene ()->GetChild ("players")->GetComponent <PlayersManager> ();
    Urho3D::PODVector <Player *> players = playersManager->GetAllPlayers ();
    SendChatMessage (player->GetName (), messageData.ReadString (), players, false);
    player->OnChatMessageSended ();
}

void MessagesHandler::ProcessSendPrivateMessageInput (Player *player, Urho3D::VectorBuffer &messageData)
{
    PlayersManager *playersManager = node_->GetScene ()->GetChild ("players")->GetComponent <PlayersManager> ();
    Urho3D::String message = messageData.ReadString ();
    Urho3D::PODVector <Urho3D::StringHash> recievers;
    recievers.Push (Urho3D::StringHash (player->GetName ()));
    while (!messageData.IsEof ())
    {
        recievers.Push (Urho3D::StringHash (messageData.ReadString ()));
    }

    Urho3D::PODVector <Player *> players = playersManager->GetPlayersByNames (recievers);
    SendChatMessage (player->GetName (), message, players, true);
    player->OnChatMessageSended ();
}

void MessagesHandler::ProcessSendPlayerActionInput (Player *player, Urho3D::VectorBuffer &messageData)
{
    PlayerActionType actionType = static_cast <PlayerActionType> (messageData.ReadInt ());
    Urho3D::Pair <PlayerActionType, Urho3D::Variant> action =
            Urho3D::Pair <PlayerActionType, Urho3D::Variant> (actionType, messageData);
    player->AddAction (action);
}

void MessagesHandler::ProcessSendIsPlayerReadyForStartInput (Player *player, Urho3D::VectorBuffer &messageData)
{
    bool isReadyForStart = messageData.ReadBool ();
    player->SetIsReadyForStart (isReadyForStart);
}

void MessagesHandler::ProcessReselectPlayerColor (Player *player, Urho3D::VectorBuffer &messageData)
{
    PlayersManager *playersManager = node_->GetScene ()->GetChild ("players")->GetComponent <PlayersManager> ();
    Urho3D::Color color = messageData.ReadColor ();
    if (!playersManager->IsColorUsed (color, player))
    {
        player->SetColor (color);
    }
}

MessagesHandler::MessagesHandler (Urho3D::Context *context) : Urho3D::Component (context)
{
    SubscribeToEvent (Urho3D::E_CLIENTIDENTITY, URHO3D_HANDLER (MessagesHandler, HandleClientIdentity));
    SubscribeToEvent (Urho3D::E_NETWORKMESSAGE, URHO3D_HANDLER (MessagesHandler, HandleNetworkMessage));
    SubscribeToEvent (EVENT_PLAYER_WILL_BE_DISCONNECTED, URHO3D_HANDLER (MessagesHandler, HandlePlayerWillBeDisconnected));
}

MessagesHandler::~MessagesHandler ()
{

}

void MessagesHandler::RegisterObject (Urho3D::Context *context)
{
    context->RegisterFactory <MessagesHandler> (COLONIZATION_SERVER_ONLY_CATEGORY);
    URHO3D_ACCESSOR_ATTRIBUTE ("Is Enabled", IsEnabled, SetEnabled, bool, true, Urho3D::AM_DEFAULT);
}

void MessagesHandler::HandleClientIdentity (Urho3D::StringHash eventType, Urho3D::VariantMap &eventData)
{
    if (enabled_)
    {
        Urho3D::Connection *connection = static_cast <Urho3D::Connection *> (
                eventData [Urho3D::ClientIdentity::P_CONNECTION].GetPtr ());
        PlayersManager *playersManager = node_->GetScene ()->GetChild ("players")->GetComponent <PlayersManager> ();
        assert (playersManager);

        Urho3D::String name = connection->GetIdentity () ["Name"].GetString ();
        Urho3D::Color color = connection->GetIdentity () ["Color"].GetColor ();
        // TODO: Currently we disconnect player if it sends identity with name which is already used by another player.
        if (playersManager->GetPlayerByNameHash (name) || !playersManager->IsAcceptingNewConnections () ||
                playersManager->IsColorUsed (color))
        {
            connection->Disconnect ();
        }
        else
        {
            playersManager->PlayerIdentified (connection, name, color);
        }
    }
}

void MessagesHandler::HandleNetworkMessage (Urho3D::StringHash eventType, Urho3D::VariantMap &eventData)
{
    if (enabled_)
    {
        PlayersManager *playersManager = node_->GetScene ()->GetChild ("players")->GetComponent <PlayersManager> ();
        assert (playersManager);

        Urho3D::Connection *connection = static_cast <Urho3D::Connection *> (
                eventData [Urho3D::NetworkMessage::P_CONNECTION].GetPtr ());
        Player *player = playersManager->GetPlayerByConnection (connection);

        // Process messages only if player identified by players manager.
        if (player)
        {
            NetworkMessageType messageType = static_cast <NetworkMessageType> (
                        eventData [Urho3D::NetworkMessage::P_MESSAGEID].GetInt ());
            Urho3D::VectorBuffer messageData = eventData [Urho3D::NetworkMessage::P_DATA].GetVectorBuffer ();

            if (messageType == CTS_NETWORK_MESSAGE_SEND_CHAT_MESSAGE && player->GetTimeUntilNewChatMessage () <= 0.0f)
            {
                ProcessSendChatMessageInput (player, messageData);
            }
            else if (messageType == CTS_NETWORK_MESSAGE_SEND_PRIVATE_MESSAGE && player->GetTimeUntilNewChatMessage () <= 0.0f)
            {
                ProcessSendPrivateMessageInput (player, messageData);
            }
            else if (messageType == CTS_NETWORK_MESSAGE_SEND_PLAYER_ACTION)
            {
                ProcessSendPlayerActionInput (player, messageData);
            }
            else if (messageType == CTS_NETWORK_MESSAGE_SEND_IS_PLAYER_READY_FOR_START)
            {
                ProcessSendIsPlayerReadyForStartInput (player, messageData);
            }
            else if (messageType == CTS_NETWORK_MESSAGE_RESELECT_PLAYER_COLOR)
            {
                ProcessReselectPlayerColor (player, messageData);
            }
        }
    }
}

void MessagesHandler::HandlePlayerWillBeDisconnected (Urho3D::StringHash eventType, Urho3D::VariantMap &eventData)
{
    PlayersManager *playersManager = node_->GetScene ()->GetChild ("players")->GetComponent <PlayersManager> ();
    assert (playersManager);
    Player *player = static_cast <Player *> (eventData [PlayerWillBeDisconnected::PLAYER].GetPtr ());
    Urho3D::PODVector <Player *> allPlayers = playersManager->GetAllPlayers ();
    SendTextInfoFromServer (player->GetName () + " left the game!", allPlayers);
}

void MessagesHandler::SendPlayersStats (Player *player)
{
    assert (player);
    Urho3D::VectorBuffer messageData;
    messageData.WriteFloat (player->GetGold ());
    messageData.WriteFloat (player->GetPoints ());
    player->SendMessage (STC_NETWORK_MESSAGE_SEND_PLAYER_STATS, true, false, messageData);
}

void MessagesHandler::SendChatMessage (Urho3D::String senderName, Urho3D::String message, Urho3D::PODVector <Player *> &recieviers, bool isPrivate)
{
    Urho3D::VectorBuffer messageData;
    messageData.WriteBool (isPrivate);
    messageData.WriteString (senderName);
    messageData.WriteString (message);

    for (int index = 0; index < recieviers.Size (); index++)
    {
        recieviers.At (index)->SendMessage (STC_NETWORK_MESSAGE_CHAT_MESSAGE, true, false, messageData);
    }
}

void MessagesHandler::SendTextInfoFromServer (Urho3D::String info, Urho3D::PODVector <Player *> &recieviers)
{
    Urho3D::VectorBuffer messageData;
    messageData.WriteString (info);
    for (int index = 0; index < recieviers.Size (); index++)
    {
        if (recieviers.At (index))
        {
            recieviers.At (index)->SendMessage (STC_NETWORK_MESSAGE_TEXT_INFO_FROM_SERVER, true, false, messageData);
        }
    }
}

void MessagesHandler::SendGameState (GameStateType gameState, Urho3D::PODVector <Player *> &recieviers)
{
    Urho3D::VectorBuffer messageData;
    messageData.WriteInt (static_cast <int> (gameState));
    for (int index = 0; index < recieviers.Size (); index++)
    {
        if (recieviers.At (index))
        {
            recieviers.At (index)->SendMessage (STC_NETWORK_MESSAGE_SEND_GAME_STATE, true, false, messageData);
        }
    }
}

void MessagesHandler::SendGameEnded (Urho3D::String winnerName, Urho3D::String victoryType, Urho3D::String victoryInfo, Urho3D::PODVector <Player *> &recieviers)
{
    Urho3D::VectorBuffer messageData;
    messageData.WriteString (winnerName);
    messageData.WriteString (victoryType);
    messageData.WriteString (victoryInfo);

    for (int index = 0; index < recieviers.Size (); index++)
    {
        if (recieviers.At (index))
        {
            recieviers.At (index)->SendMessage (STC_NETWORK_MESSAGE_GAME_ENDED, true, false, messageData);
        }
    }
}

void MessagesHandler::SendDiplomacyInfo (Urho3D::StringHash infoType, Urho3D::VariantMap infoData, Urho3D::PODVector <Player *> &recieviers)
{
    Urho3D::VectorBuffer messageData;
    messageData.WriteStringHash (infoType);
    messageData.WriteVariantMap (infoData);

    for (int index = 0; index < recieviers.Size (); index++)
    {
        {
            recieviers.At (index)->SendMessage (STC_NETWORK_MESSAGE_DIPLOMACY_INFO, true, false, messageData);
        }
    }
}

void MessagesHandler::SendDiplomacyOffer (Urho3D::StringHash offerType, unsigned offerDiplomacyRequestId, float autodeclineTime, Urho3D::VariantMap offerData, Urho3D::PODVector <Player *> &recieviers)
{
    Urho3D::VectorBuffer messageData;
    messageData.WriteStringHash (offerType);
    messageData.WriteUInt (offerDiplomacyRequestId);
    messageData.WriteFloat (autodeclineTime);
    messageData.WriteVariantData (offerData);

    for (int index = 0; index < recieviers.Size (); index++)
    {
        {
            recieviers.At (index)->SendMessage (STC_NETWORK_MESSAGE_DIPLOMACY_OFFER, true, false, messageData);
        }
    }
}
}
