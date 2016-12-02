#include <Colonization/BuildConfiguration.hpp>
#include "PlayersManager.hpp"
#include <Urho3D/Core/CoreEvents.h>
#include <Urho3D/Network/NetworkEvents.h>

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

}

PlayersManager::~PlayersManager ()
{
    while (!connectionsWithoutId_.Empty ())
    {
        Urho3D::Connection *connection = connectionsWithoutId_.Front ();
        connectionsWithoutId_.Remove (connectionsWithoutId_.Front ());
        delete connection;
    }

    while (!players_.Keys ().Empty ())
        DisconnectPlayer (players_.Keys ().Front ());
}

void PlayersManager::Update (Urho3D::StringHash eventType, Urho3D::VariantMap &eventData)
{
    float timeStep = eventData [Urho3D::Update::P_TIMESTEP].GetFloat ();
    for (int index = 0; index < players_.Values ().Size (); index++)
        players_.Values ().At (index)->Update (timeStep);

    int index = 0;
    while (index < connectionsWithoutId_.Size ())
    {
        connectionsWithoutId_.At (index).first_ -= timeStep;
        if (connectionsWithoutId_.At (index).first_ <= 0.0f)
        {
            delete connectionsWithoutId_.At (index).second_;
            connectionsWithoutId_.Remove (connectionsWithoutId_.At (index));
        }
        else
            index++;
    }
}

void PlayersManager::HandlePlayerConnected (Urho3D::StringHash eventType, Urho3D::VariantMap &eventData)
{
    Urho3D::Connection *connection = eventData [Urho3D::ClientDisconnected::P_CONNECTION].GetPtr ();
    connectionsWithoutId_.Push (Urho3D::Pair <float, Urho3D::Connection *> (1.0f, connection));
}

void PlayersManager::HandlePlayerDisconnected (Urho3D::StringHash eventType, Urho3D::VariantMap &eventData)
{
    Urho3D::Connection *connection = eventData [Urho3D::ClientDisconnected::P_CONNECTION].GetPtr ();
    DisconnectPlayer (connection);
    // TODO: Inform other players.
}

Player *PlayersManager::GetPlayer (Urho3D::StringHash nameHash)
{
    return players_ [nameHash];
}

Player *PlayersManager::GetPlayer (Urho3D::Connection *connection)
{
    return players_ [connectionHashToNameHashMap_ [connection->GetAddress ()]];
}

void PlayersManager::PlayerIdentified (Urho3D::Connection *connection, Urho3D::String name)
{
    DeleteIdentificatedConnection (connection);
    Player *player = new Player (context_, name, connection);
    players_ [name] = player;
    connectionHashToNameHashMap_ [connection->GetAddress ()] = name;
    // TODO: Inform other players.
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
