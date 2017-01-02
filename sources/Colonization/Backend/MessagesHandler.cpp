#include <Colonization/BuildConfiguration.hpp>
#include "MessagesHandler.hpp"
#include <Urho3D/Core/Context.h>
#include <Urho3D/Network/NetworkEvents.h>
#include <Urho3D/Network/Connection.h>
#include <Urho3D/IO/Log.h>
#include <Colonization/Backend/PlayersManager.hpp>
#include <Colonization/Utils/Categories.hpp>

namespace Colonization
{
MessagesHandler::MessagesHandler (Urho3D::Context *context) : Urho3D::Component (context)
{
    SubscribeToEvent (Urho3D::E_CLIENTIDENTITY, URHO3D_HANDLER (MessagesHandler, HandleClientIdentity));
    SubscribeToEvent (Urho3D::E_NETWORKMESSAGE, URHO3D_HANDLER (MessagesHandler, HandleNetworkMessage));
}

MessagesHandler::~MessagesHandler ()
{

}

void MessagesHandler::RegisterObject (Urho3D::Context *context)
{
    context->RegisterFactory <MessagesHandler> (COLONIZATION_SERVER_ONLY_CATEGORY);
    URHO3D_ACCESSOR_ATTRIBUTE("Is Enabled", IsEnabled, SetEnabled, bool, true, Urho3D::AM_DEFAULT);
}

void MessagesHandler::HandleClientIdentity (Urho3D::StringHash eventType, Urho3D::VariantMap &eventData)
{
    if (enabled_)
    {
        Urho3D::Connection *connection = (Urho3D::Connection *)
                eventData [Urho3D::ClientDisconnected::P_CONNECTION].GetPtr ();
        PlayersManager *playersManager = node_->GetScene ()->GetChild ("players")->GetComponent <PlayersManager> ();
        assert (playersManager);

        Urho3D::String name = connection->GetIdentity () ["Name"].GetString ();
        // TODO: Currently we disconnect player if it sends identity with name which is already used by another player.
        if (playersManager->GetPlayer (name))
            connection->Disconnect ();
        else
            playersManager->PlayerIdentified (connection, connection->GetIdentity () ["Name"].GetString ());
    }
}

void MessagesHandler::HandleNetworkMessage (Urho3D::StringHash eventType, Urho3D::VariantMap &eventData)
{
    if (enabled_)
    {
        PlayersManager *playersManager = node_->GetScene ()->GetChild ("players")->GetComponent <PlayersManager> ();
        assert (playersManager);

        Urho3D::Connection *connection = (Urho3D::Connection *)
                eventData [Urho3D::NetworkMessage::P_CONNECTION].GetPtr ();
        Player *player = playersManager->GetPlayer (connection);
        assert (player);

        NetworkMessageType messageType = static_cast <NetworkMessageType> (
                    eventData [Urho3D::NetworkMessage::P_MESSAGEID].GetInt ());
        Urho3D::VectorBuffer messageData = eventData [Urho3D::NetworkMessage::P_DATA].GetVectorBuffer ();

        if (messageType == CTS_NETWORK_MESSAGE_SEND_CHAT_MESSAGE)
        {
            Urho3D::Vector <Player *> players = playersManager->GetAllPlayers ();
            SendChatMessage (player->GetName (), messageData.ReadString (), players, false);
        }

        else if (messageType == CTS_NETWORK_MESSAGE_SEND_PRIVATE_MESSAGE)
        {
            Urho3D::String message = messageData.ReadString ();
            Urho3D::Vector <Urho3D::StringHash> recievers;
            while (!messageData.IsEof ())
                recievers.Push (Urho3D::StringHash (messageData.ReadString ()));
            Urho3D::Vector <Player *> players = playersManager->GetPlayersByNames (recievers);
            SendChatMessage (player->GetName (), message, players, true);
        }

        else if (messageType == CTS_NETWORK_MESSAGE_SEND_PLAYER_ACTION)
        {
            PlayerActionType actionType = static_cast <PlayerActionType> (messageData.ReadInt ());
            Urho3D::Pair <PlayerActionType, Urho3D::Variant> action =
                    Urho3D::Pair <PlayerActionType, Urho3D::Variant> (actionType, messageData);
            player->AddAction (action);
        }
    }
}

void MessagesHandler::SendPlayersStats (Player *player)
{
    assert (player);
    Urho3D::VectorBuffer messageData;
    messageData.WriteFloat (player->GetGold ());
    messageData.WriteFloat (player->GetPoints ());
    player->GetConnection ()->SendMessage (STC_NETWORK_MESSAGE_SEND_PLAYER_STATS, true, false, messageData);
}

void MessagesHandler::SendChatMessage (Urho3D::String senderName, Urho3D::String message, Urho3D::Vector <Player *> &recieviers, bool isPrivate)
{
    Urho3D::VectorBuffer messageData;
    messageData.WriteBool (isPrivate);
    messageData.WriteString (senderName);
    messageData.WriteString (message);

    for (int index = 0; index < recieviers.Size (); index++)
        recieviers.At (index)->GetConnection ()->SendMessage (STC_NETWORK_MESSAGE_CHAT_MESSAGE, true, false, messageData);
}

void MessagesHandler::SendTextInfoFromServer (Urho3D::String info, Urho3D::Vector <Player *> &recieviers)
{
    Urho3D::VectorBuffer messageData;
    messageData.WriteString (info);
    for (int index = 0; index < recieviers.Size (); index++)
        recieviers.At (index)->GetConnection ()->SendMessage (STC_NETWORK_MESSAGE_TEXT_INFO_FROM_SERVER, true, false, messageData);
}

void MessagesHandler::SendGameState (GameStateType gameState, Urho3D::Vector<Player *> &recieviers)
{
    Urho3D::VectorBuffer messageData;
    messageData.WriteInt (static_cast <int> (gameState));
    for (int index = 0; index < recieviers.Size (); index++)
        recieviers.At (index)->GetConnection ()->SendMessage (STC_NETWORK_MESSAGE_SEND_GAME_STATE, true, false, messageData);
}
}
