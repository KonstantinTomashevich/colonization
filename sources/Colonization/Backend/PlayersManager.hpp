#pragma once
#include <Colonization/BuildConfiguration.hpp>
#include <Urho3D/Scene/Component.h>
#include <Urho3D/Container/HashMap.h>
#include <Urho3D/Network/Connection.h>
#include <Colonization/Backend/Player.hpp>
#include <Colonization/Backend/MessagesHandler.hpp>

namespace Colonization
{
class PlayersManager : public Urho3D::Component
{
URHO3D_OBJECT (PlayersManager, Component)
protected:
    Urho3D::HashMap <Urho3D::StringHash, Player *> players_;
    Urho3D::HashMap <Urho3D::StringHash, Urho3D::StringHash> connectionHashToNameHashMap_;
    Urho3D::Vector <Urho3D::Pair <float, Urho3D::Connection *> > connectionsWithoutId_;

    bool DeleteIdentificatedConnection (Urho3D::Connection *connection);
    void UpdatePlayers (MessagesHandler *messagesHandler, float timeStep);
    void UpdateConnectionsWithoudId (float timeStep);
    void UpdatePlayersInfos ();

public:
    PlayersManager (Urho3D::Context *context);
    virtual ~PlayersManager ();
    static void RegisterObject (Urho3D::Context *context);

    void Update (Urho3D::StringHash eventType, Urho3D::VariantMap &eventData);
    void HandlePlayerConnected (Urho3D::StringHash eventType, Urho3D::VariantMap &eventData);
    void HandlePlayerDisconnected (Urho3D::StringHash eventType, Urho3D::VariantMap &eventData);

    int GetPlayersCount ();
    void DisconnectAllUnidentificatedConnections ();
    Player *GetPlayer (Urho3D::StringHash nameHash);
    Urho3D::Vector <Player *> GetPlayersByNames (Urho3D::Vector <Urho3D::StringHash> namesHashes);
    Player *GetPlayer (Urho3D::Connection *connection);
    Urho3D::Vector <Player *> GetAllPlayers ();

    void PlayerIdentified (Urho3D::Connection *connection, Urho3D::String name);
    void DisconnectPlayer (Urho3D::StringHash nameHash);
    void DisconnectPlayer (Urho3D::Connection *connection);
};
}
