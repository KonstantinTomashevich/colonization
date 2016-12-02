#include <Colonization/BuildConfiguration.hpp>
#include "PlayersManager.hpp"
#include <Urho3D/Core/Context.h>
#include <Urho3D/Core/CoreEvents.h>
#include <Urho3D/Network/NetworkEvents.h>
#include <Colonization/Backend/MessagesHandler.hpp>

namespace Colonization
{
bool PlayersManager::DeleteIdentificatedConnection (Urho3D::Connection *connection)
{
    bool isFinded = false;
    int index = 0;
    while (index < connectionsWithoutId_.Size () && !isFinded)
    {
        if (connectionsWithoutId_.At (index).second_ == connection)
        {
            connectionsWithoutId_.Remove (connectionsWithoutId_.At (index));
            isFinded = true;
            delete connection;
        }
        else
            index++;
    }
    return isFinded;
}

PlayersManager::PlayersManager (Urho3D::Context *context) : Urho3D::Object (context),
    players_ (),
    connectionHashToNameHashMap_ (),
    connectionsWithoutId_ ()
{
    SubscribeToEvent (Urho3D::E_UPDATE, URHO3D_HANDLER (PlayersManager, Update));
    SubscribeToEvent (Urho3D::E_CLIENTCONNECTED, URHO3D_HANDLER (PlayersManager, HandlePlayerConnected));
    SubscribeToEvent (Urho3D::E_CLIENTDISCONNECTED, URHO3D_HANDLER (PlayersManager, HandlePlayerDisconnected));
}

PlayersManager::~PlayersManager ()
{
    UnsubscribeFromAllEvents ();
    DisconnectAllUnidentificatedConnections ();

    while (!players_.Keys ().Empty ())
        DisconnectPlayer (players_.Keys ().Front ());
}

void PlayersManager::Update (Urho3D::StringHash eventType, Urho3D::VariantMap &eventData)
{
    MessagesHandler *messagesHandler = context_->GetSubsystem <MessagesHandler> ();
    assert (messagesHandler);

    float timeStep = eventData [Urho3D::Update::P_TIMESTEP].GetFloat ();
    for (int index = 0; index < players_.Values ().Size (); index++)
    {
        Player *player = players_.Values ().At (index);
        player->Update (timeStep);
        messagesHandler->SendPlayersStats (player);
    }

    int index = 0;
    while (index < connectionsWithoutId_.Size ())
    {
        connectionsWithoutId_.At (index).first_ -= timeStep;
        if (connectionsWithoutId_.At (index).first_ <= 0.0f)
        {
            connectionsWithoutId_.At (index).second_->Disconnect ();
            delete connectionsWithoutId_.At (index).second_;
            connectionsWithoutId_.Remove (connectionsWithoutId_.At (index));
        }
        else
            index++;
    }
}

void PlayersManager::HandlePlayerConnected (Urho3D::StringHash eventType, Urho3D::VariantMap &eventData)
{
    Urho3D::Connection *connection = (Urho3D::Connection *)
            eventData [Urho3D::ClientDisconnected::P_CONNECTION].GetPtr ();
    connectionsWithoutId_.Push (Urho3D::Pair <float, Urho3D::Connection *> (1.0f, connection));
}

void PlayersManager::HandlePlayerDisconnected (Urho3D::StringHash eventType, Urho3D::VariantMap &eventData)
{
    Urho3D::Connection *connection = (Urho3D::Connection *)
            eventData [Urho3D::ClientDisconnected::P_CONNECTION].GetPtr ();
    MessagesHandler *messagesHandler = context_->GetSubsystem <MessagesHandler> ();
    assert (messagesHandler);
    Urho3D::Vector <Player *> allPlayers = players_.Values ();
    messagesHandler->SendTextInfoFromServer (GetPlayer (connection)->GetName () + " left game!", allPlayers);
    DisconnectPlayer (connection);
}

int PlayersManager::GetPlayersCount ()
{
    return players_.Values ().Size ();
}

void PlayersManager::DisconnectAllUnidentificatedConnections ()
{
    while (!connectionsWithoutId_.Empty ())
    {
        Urho3D::Connection *connection = (Urho3D::Connection *)
                connectionsWithoutId_.Front ().second_;
        connectionsWithoutId_.Remove (connectionsWithoutId_.Front ());
        connection->Disconnect ();
        delete connection;
    }
}

Player *PlayersManager::GetPlayer (Urho3D::StringHash nameHash)
{
    return players_ [nameHash];
}

Urho3D::Vector <Player *> PlayersManager::GetPlayersByNames (Urho3D::Vector <Urho3D::StringHash> namesHashes)
{
    Urho3D::Vector <Player *> players;
    for (int index = 0; index < namesHashes.Size (); index++)
        players.Push (players_ [namesHashes.At (index)]);
    return players;
}

Player *PlayersManager::GetPlayer (Urho3D::Connection *connection)
{
    return players_ [connectionHashToNameHashMap_ [connection->GetAddress ()]];
}

Urho3D::Vector<Player *> PlayersManager::GetAllPlayers ()
{
    return players_.Values ();
}

void PlayersManager::PlayerIdentified (Urho3D::Connection *connection, Urho3D::String name)
{
    DeleteIdentificatedConnection (connection);
    Player *player = new Player (context_, name, connection);
    players_ [name] = player;
    connectionHashToNameHashMap_ [connection->GetAddress ()] = name;

    MessagesHandler *messagesHandler = context_->GetSubsystem <MessagesHandler> ();
    assert (messagesHandler);
    Urho3D::Vector <Player *> allPlayers = players_.Values ();
    messagesHandler->SendTextInfoFromServer (player->GetName () + " entered game!", allPlayers);
}

void PlayersManager::DisconnectPlayer (Urho3D::StringHash nameHash)
{
    Player *player = players_ [nameHash];
    players_.Erase (nameHash);
    connectionHashToNameHashMap_.Erase (player->GetConnection ()->GetAddress ());
    player->GetConnection ()->Disconnect ();
    delete player;
}

void PlayersManager::DisconnectPlayer (Urho3D::Connection *connection)
{
    DisconnectPlayer (connectionHashToNameHashMap_ [connection->GetAddress ()]);
}
}
