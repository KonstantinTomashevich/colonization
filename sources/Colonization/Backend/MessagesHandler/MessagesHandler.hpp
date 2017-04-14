#pragma once
#include <Urho3D/Scene/Component.h>
#include <Colonization/Backend/Player/Player.hpp>
#include <Colonization/Activities/HostActivity/GameStateType.hpp>

namespace Colonization
{
class MessagesHandler : public Urho3D::Component
{
URHO3D_OBJECT (MessagesHandler, Component)
protected:
    void ProcessSendChatMessageInput (Player *player, Urho3D::VectorBuffer &messageData);
    void ProcessSendPrivateMessageInput (Player *player, Urho3D::VectorBuffer &messageData);
    void ProcessSendPlayerActionInput (Player *player, Urho3D::VectorBuffer &messageData);
    void ProcessSendIsPlayerReadyForStartInput (Player *player, Urho3D::VectorBuffer &messageData);
    void ProcessReselectPlayerColor (Player *player, Urho3D::VectorBuffer &messageData);

public:
    MessagesHandler (Urho3D::Context *context);
    virtual ~MessagesHandler ();
    static void RegisterObject (Urho3D::Context *context);

    void HandleClientIdentity (Urho3D::StringHash eventType, Urho3D::VariantMap &eventData);
    void HandleNetworkMessage (Urho3D::StringHash eventType, Urho3D::VariantMap &eventData);

    void SendPlayersStats (Player *player);
    void SendChatMessage (Urho3D::String senderName, Urho3D::String message, Urho3D::Vector <Player *> &recieviers, bool isPrivate);
    void SendTextInfoFromServer (Urho3D::String info, Urho3D::Vector <Player *> &recieviers);
    void SendGameState (GameStateType gameState, Urho3D::Vector <Player *> &recieviers);
    void SendGameEnded (Urho3D::String winnerName, Urho3D::String victoryType, Urho3D::String victoryInfo, Urho3D::Vector <Player *> &recieviers);
};
}