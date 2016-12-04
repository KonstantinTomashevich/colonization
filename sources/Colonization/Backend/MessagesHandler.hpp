#pragma once
#include <Urho3D/Core/Object.h>
#include <Colonization/Backend/Player.hpp>
#include <Colonization/Backend/GameStateManager.hpp>

namespace Colonization
{
// TODO: Bind network messages types to AngelScript.
enum NetworkMessageType
{
    // CTS -- client to server message.
    CTS_NETWORK_MESSAGE_SEND_CHAT_MESSAGE = 100, // Data: message (String).
    CTS_NETWORK_MESSAGE_SEND_PRIVATE_MESSAGE = 101, // Data: ...recieviers... (String's), message (String).
    CTS_NETWORK_MESSAGE_SEND_PLAYER_ACTION = 102, // Data: actionType (int), actionData (VariantMap).

    // STC -- server to client message.
    STC_NETWORK_MESSAGE_SEND_PLAYER_STATS = 200, // Data: gold (float), points (float).
    STC_NETWORK_MESSAGE_CHAT_MESSAGE = 201, // Data: isPrivate (bool), senderName (String), message (String).
    STC_NETWORK_MESSAGE_TEXT_INFO_FROM_SERVER = 202, // Data: message (String).
    STC_NETWORK_MESSAGE_SEND_GAME_STATE = 203 // Data: int (GameState [enum]).

    // TODO: To be continued.
};

class MessagesHandler : public Urho3D::Object
{
URHO3D_OBJECT (MessagesHandler, Object)
public:
    MessagesHandler (Urho3D::Context *context);
    virtual ~MessagesHandler ();

    void HandleClientIdentity (Urho3D::StringHash eventType, Urho3D::VariantMap &eventData);
    void HandleNetworkMessage (Urho3D::StringHash eventType, Urho3D::VariantMap &eventData);

    void SendPlayersStats (Player *player);
    void SendChatMessage (Urho3D::String senderName, Urho3D::String message, Urho3D::Vector <Player *> &recieviers, bool isPrivate);
    void SendTextInfoFromServer (Urho3D::String info, Urho3D::Vector <Player *> &recieviers);
    void SendGameState (GameState gameState, Urho3D::Vector <Player *> &recieviers);
};
}
