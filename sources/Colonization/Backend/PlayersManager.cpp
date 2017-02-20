#include <Colonization/BuildConfiguration.hpp>
#include "PlayersManager.hpp"
#include <Urho3D/Core/Context.h>
#include <Urho3D/Scene/SceneEvents.h>
#include <Urho3D/Scene/Component.h>
#include <Urho3D/Scene/Scene.h>
#include <Urho3D/Network/NetworkEvents.h>
#include <Urho3D/IO/Log.h>

#include <Colonization/Backend/NetworkUpdateCounter.hpp>
#include <Colonization/Backend/MessagesHandler.hpp>
#include <Colonization/Core/PlayerInfo.hpp>
#include <Colonization/Utils/Categories.hpp>
#include <Colonization/Utils/AttributeMacro.hpp>

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
        }
        else
        {
            index++;
        }
    }
    return isFinded;
}

void PlayersManager::UpdatePlayers (MessagesHandler *messagesHandler, float timeStep)
{
    int index = 0;
    while (index < players_.Values ().Size ())
    {
        Player *player = players_.Values ().At (index);
        if (!player)
        {
            players_.Erase (players_.Keys ().At (index));
        }
        else
        {
            player->Update (timeStep);
            messagesHandler->SendPlayersStats (player);
            index++;
        }
    }
}

void PlayersManager::UpdateConnectionsWithoudId (float timeStep)
{
    int index = 0;
    while (index < connectionsWithoutId_.Size ())
    {
        connectionsWithoutId_.At (index).first_ -= timeStep;
        if (connectionsWithoutId_.At (index).first_ <= 0.0f)
        {
            Urho3D::Connection *connection = connectionsWithoutId_.At (index).second_;
            connectionsWithoutId_.Remove (connectionsWithoutId_.At (index));
            connection->Disconnect ();
        }
        else
        {
            index++;
        }
    }
}

void PlayersManager::UpdatePlayersInfos ()
{
    assert (node_);
    Urho3D::PODVector <Urho3D::Node *> playersInfosNodes;
    node_->GetChildrenWithComponent (playersInfosNodes, PlayerInfo::GetTypeStatic ());

    int index = 0;
    while (index < players_.Values ().Size () || index < playersInfosNodes.Size ())
    {
        if (index < players_.Values ().Size ())
        {
            Player *player = players_.Values ().At (index);
            Urho3D::Node *infoNode;
            if (index < playersInfosNodes.Size ())
            {
                infoNode = playersInfosNodes.At (index);
            }
            else
            {
                infoNode = node_->CreateChild (player->GetName (), Urho3D::REPLICATED);
                infoNode->CreateComponent <PlayerInfo> (Urho3D::REPLICATED);
            }

            infoNode->SetName (player->GetName ());
            PlayerInfo *playerInfo = infoNode->GetComponent <PlayerInfo> ();
            float updatePoints = 0.0f;

            if (playerInfo->GetName () != player->GetName ())
            {
                playerInfo->SetName (player->GetName ());
                updatePoints += 100.0f;
            }

            if (playerInfo->GetPoints () != player->GetPoints ())
            {
                float change = Urho3D::Abs (playerInfo->GetPoints () - player->GetPoints ());
                playerInfo->SetPoints (player->GetPoints ());
                updatePoints += change * 50.0f / player->GetPoints ();
            }

            if (playerInfo->GetColor () != player->GetColor ())
            {
                playerInfo->SetColor (player->GetColor ());
                updatePoints += 100.0f;
            }

            NetworkUpdateCounter *counter = playerInfo->GetNode ()->GetComponent <NetworkUpdateCounter> ();
            if (!counter)
            {
                counter = CreateNetworkUpdateCounterForComponent (playerInfo);
            }
            counter->AddUpdatePoints (updatePoints);
        }
        else
        {
            playersInfosNodes.At (index)->Remove ();
        }
        index++;
    }
}

void PlayersManager::OnSceneSet(Urho3D::Scene *scene)
{
    UnsubscribeFromAllEvents ();
    Urho3D::Component::OnSceneSet (scene);
    SubscribeToEvent (scene, Urho3D::E_SCENEUPDATE, URHO3D_HANDLER (PlayersManager, Update));
    SubscribeToEvent (Urho3D::E_CLIENTCONNECTED, URHO3D_HANDLER (PlayersManager, HandlePlayerConnected));
    SubscribeToEvent (Urho3D::E_CLIENTDISCONNECTED, URHO3D_HANDLER (PlayersManager, HandlePlayerDisconnected));
}

PlayersManager::PlayersManager (Urho3D::Context *context) : Urho3D::Component (context),
    players_ (),
    connectionHashToNameHashMap_ (),
    connectionsWithoutId_ ()
{

}

PlayersManager::~PlayersManager ()
{
    UnsubscribeFromAllEvents ();
    DisconnectAllUnidentificatedConnections ();

    while (!players_.Keys ().Empty ())
    {
        DisconnectPlayer (players_.Keys ().Front ());
    }
}

void PlayersManager::RegisterObject (Urho3D::Context *context)
{
    context->RegisterFactory <PlayersManager> (COLONIZATION_SERVER_ONLY_CATEGORY);
    URHO3D_ACCESSOR_ATTRIBUTE ("Is Enabled", IsEnabled, SetEnabled, bool, true, Urho3D::AM_DEFAULT);
}

void PlayersManager::Update (Urho3D::StringHash eventType, Urho3D::VariantMap &eventData)
{
    if (enabled_)
    {
        MessagesHandler *messagesHandler = node_->GetScene ()->GetComponent <MessagesHandler> ();
        assert (messagesHandler);
        float timeStep = eventData [Urho3D::SceneUpdate::P_TIMESTEP].GetFloat ();
        UpdatePlayers (messagesHandler, timeStep);
        UpdateConnectionsWithoudId (timeStep);
        UpdatePlayersInfos ();
    }
}

void PlayersManager::HandlePlayerConnected (Urho3D::StringHash eventType, Urho3D::VariantMap &eventData)
{
    if (enabled_)
    {
        Urho3D::Connection *connection = (Urho3D::Connection *)
                eventData [Urho3D::ClientDisconnected::P_CONNECTION].GetPtr ();
        connectionsWithoutId_.Push (Urho3D::Pair <float, Urho3D::Connection *> (1.0f, connection));
    }
}

void PlayersManager::HandlePlayerDisconnected (Urho3D::StringHash eventType, Urho3D::VariantMap &eventData)
{
    if (enabled_)
    {
        Urho3D::Connection *connection = (Urho3D::Connection *)
                eventData [Urho3D::ClientDisconnected::P_CONNECTION].GetPtr ();
        MessagesHandler *messagesHandler = node_->GetScene ()->GetComponent <MessagesHandler> ();
        assert (messagesHandler);

        Player *player = GetPlayerByConnection (connection);
        if (player)
        {
            Urho3D::Vector <Player *> allPlayers = players_.Values ();
            messagesHandler->SendTextInfoFromServer (GetPlayerByConnection (connection)->GetName () + " left game!", allPlayers);
            DisconnectPlayer (connection);
        }
    }
}

int PlayersManager::GetPlayersCount ()
{
    return players_.Values ().Size ();
}

Player *PlayersManager::GetPlayerByIndex (int index)
{
    assert (index < players_.Values ().Size ());
    return players_.Values ().At (index);
}

Player *PlayersManager::GetPlayerByConnection (Urho3D::Connection *connection)
{
    if (connectionHashToNameHashMap_ [connection->GetAddress ()].Value ())
    {
        return players_ [connectionHashToNameHashMap_ [connection->GetAddress ()]];
    }
    else
    {
        return 0;
    }
}

Player *PlayersManager::GetPlayerByNameHash (Urho3D::StringHash nameHash)
{
    return players_ [nameHash];
}

PlayerInfo *PlayersManager::GetPlayerInfoByPointer (Player *player)
{
    assert (player);
    return GetPlayerInfoByNameHash (Urho3D::StringHash (player->GetName ()));
}

PlayerInfo *PlayersManager::GetPlayerInfoByNameHash (Urho3D::StringHash nameHash)
{
    assert (node_);
    Urho3D::PODVector <Urho3D::Node *> playersInfosNodes;
    node_->GetChildrenWithComponent (playersInfosNodes, PlayerInfo::GetTypeStatic ());

    for (int index = 0; index < playersInfosNodes.Size (); index++)
    {
        PlayerInfo *playerInfo = playersInfosNodes.At (index)->GetComponent <PlayerInfo> ();
        if (Urho3D::StringHash (playerInfo->GetName ()) == nameHash)
        {
            return playerInfo;
        }
    }
    return 0;
}

void PlayersManager::DisconnectAllUnidentificatedConnections ()
{
    while (!connectionsWithoutId_.Empty ())
    {
        Urho3D::Connection *connection = (Urho3D::Connection *)
                connectionsWithoutId_.Front ().second_;
        connectionsWithoutId_.Remove (connectionsWithoutId_.Front ());
        connection->Disconnect ();
    }
}

Urho3D::Vector <Player *> PlayersManager::GetPlayersByNames (Urho3D::Vector <Urho3D::StringHash> namesHashes)
{
    Urho3D::Vector <Player *> players;
    for (int index = 0; index < namesHashes.Size (); index++)
    {
        Player *player = players_ [namesHashes.At (index)];
        if (player)
        {
            players.Push (player);
        }
    }
    return players;
}

Urho3D::Vector <Player *> PlayersManager::GetAllPlayers ()
{
    return players_.Values ();
}

void PlayersManager::PlayerIdentified (Urho3D::Connection *connection, Urho3D::String name, Urho3D::Color color)
{
    DeleteIdentificatedConnection (connection);
    Player *player = new Player (context_, name, color, connection, node_->GetScene ());
    players_ [name] = player;
    connectionHashToNameHashMap_ [connection->GetAddress ()] = name;
    player->SetGold (1000.0f);

    MessagesHandler *messagesHandler = node_->GetScene ()->GetComponent <MessagesHandler> ();
    assert (messagesHandler);
    Urho3D::Vector <Player *> allPlayers = players_.Values ();
    messagesHandler->SendTextInfoFromServer (player->GetName () + " entered game!", allPlayers);
    connection->SetScene (node_->GetScene ());
}

void PlayersManager::DisconnectPlayer (Urho3D::StringHash nameHash)
{
    Player *player = players_ [nameHash];
    assert (player);
    players_.Erase (nameHash);

    assert (player->GetConnection ());
    connectionHashToNameHashMap_.Erase (player->GetConnection ()->GetAddress ());
    player->GetConnection ()->Disconnect ();
    delete player;
}

void PlayersManager::DisconnectPlayer (Urho3D::Connection *connection)
{
    if (connectionHashToNameHashMap_ [connection->GetAddress ()].Value ())
    {
        DisconnectPlayer (connectionHashToNameHashMap_ [connection->GetAddress ()]);
    }
}
}
