#include <Colonization/BuildConfiguration.hpp>
#include "PlayersManager.hpp"
#include <Urho3D/Core/Context.h>
#include <Urho3D/Scene/SceneEvents.h>
#include <Urho3D/Scene/Component.h>
#include <Urho3D/Scene/Scene.h>
#include <Urho3D/Network/NetworkEvents.h>
#include <Urho3D/IO/Log.h>

#include <Colonization/Utils/Network/NetworkUpdateCounter.hpp>
#include <Colonization/Backend/MessagesHandler/MessagesHandler.hpp>
#include <Colonization/Core/PlayerInfo/PlayerInfo.hpp>
#include <Colonization/Core/GameConfiguration.hpp>
#include <Colonization/Utils/Serialization/Categories.hpp>
#include <Colonization/Utils/Serialization/AttributeMacro.hpp>

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
    Urho3D::HashMap <Urho3D::StringHash, Player *>::Iterator iterator = players_.Begin ();
    while (iterator != players_.End ())
    {
        Player *player = iterator->second_;
        if (!player)
        {
            iterator = players_.Erase (iterator);
        }
        else
        {
            player->Update (timeStep);
            messagesHandler->SendPlayersStats (player);
            iterator++;
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
    while (index < players_.Size () || index < playersInfosNodes.Size ())
    {
        if (index < players_.Size ())
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

            if (playerInfo->IsReadyForStart () != player->IsReadyForStart ())
            {
                playerInfo->SetIsReadyForStart (player->IsReadyForStart ());
                updatePoints += 100.0f;
            }

            if (playerInfo->GetEnemies () != player->GetEnemies ())
            {
                playerInfo->RemoveAllEnemies ();
                for (int index = 0; index < player->GetEnemiesCount (); index++)
                {
                    playerInfo->AddEnemy (player->GetEnemyByIndex (index));
                }
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
    SubscribeToEvent (Urho3D::E_CLIENTCONNECTED, URHO3D_HANDLER (PlayersManager, HandleClientConnected));
    SubscribeToEvent (Urho3D::E_CLIENTDISCONNECTED, URHO3D_HANDLER (PlayersManager, HandleClientDisconnected));
}

PlayersManager::PlayersManager (Urho3D::Context *context) : Urho3D::Component (context),
    isAcceptingNewConnections_ (true),
    players_ (),
    connectionHashToNameHashMap_ (),
    connectionsWithoutId_ ()
{

}

PlayersManager::~PlayersManager ()
{
    UnsubscribeFromAllEvents ();
    DisconnectAllUnidentificatedConnections ();

    while (!players_.Empty ())
    {
        DisconnectPlayer (players_.Front ().first_);
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

void PlayersManager::HandleClientConnected (Urho3D::StringHash eventType, Urho3D::VariantMap &eventData)
{
    if (enabled_)
    {
        Urho3D::Connection *connection = (Urho3D::Connection *)
                eventData [Urho3D::ClientDisconnected::P_CONNECTION].GetPtr ();

        if (isAcceptingNewConnections_)
        {
            connectionsWithoutId_.Push (Urho3D::Pair <float, Urho3D::Connection *> (1.0f, connection));
        }
        else
        {
            connection->Disconnect ();
        }
    }
}

void PlayersManager::HandleClientDisconnected (Urho3D::StringHash eventType, Urho3D::VariantMap &eventData)
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
            Urho3D::PODVector <Player *> allPlayers = GetAllPlayers ();
            messagesHandler->SendTextInfoFromServer (GetPlayerByConnection (connection)->GetName () + " left game!", allPlayers);
            DisconnectPlayer (connection);
        }
    }
}

bool PlayersManager::IsAcceptingNewConnections () const
{
    return isAcceptingNewConnections_;
}

void PlayersManager::SetIsAcceptingNewConnections (bool isAcceptingNewConnections)
{
    isAcceptingNewConnections_ = isAcceptingNewConnections;
}

void PlayersManager::SetStartGoldForAllPlayers ()
{
    GameConfiguration *configuration = node_->GetScene ()->GetComponent <GameConfiguration> ();
    assert (configuration);

    for (Urho3D::HashMap <Urho3D::StringHash, Player *>::Iterator iterator = players_.Begin ();
         iterator != players_.End (); iterator++)
    {
        iterator->second_->SetGold (configuration->GetPlayerStartGold ());
    }
}

int PlayersManager::GetPlayersCount () const
{
    return players_.Size ();
}

bool PlayersManager::AddInternalPlayer (Player *player)
{
    assert (player->IsInternal ());
    if (player->IsInternal () &&
            !IsColorUsed (player->GetColor ()) &&
            !GetPlayerByNameHash (Urho3D::StringHash (player->GetName ())))
    {
        players_ [Urho3D::StringHash (player->GetName ())] = player;
        return true;
    }
    else
    {
        return false;
    }
}

Player *PlayersManager::GetPlayerByIndex (int index) const
{
    assert (index < players_.Size ());
    return players_.Values ().At (index);
}

Player *PlayersManager::GetPlayerByConnection (Urho3D::Connection *connection) const
{
    Urho3D::StringHash *nameHash = connectionHashToNameHashMap_ [Urho3D::StringHash (connection->ToString ())];
    if (nameHash)
    {
        return GetPlayerByNameHash (*nameHash);
    }
    else
    {
        return 0;
    }
}

Player *PlayersManager::GetPlayerByNameHash (Urho3D::StringHash nameHash) const
{
    Player ** playerPtr = players_ [nameHash];
    if (playerPtr)
    {
        return *playerPtr;
    }
    else
    {
        return 0;
    }
}

PlayerInfo *PlayersManager::GetPlayerInfoByPointer (Player *player) const
{
    assert (player);
    return GetPlayerInfoByNameHash (Urho3D::StringHash (player->GetName ()));
}

PlayerInfo *PlayersManager::GetPlayerInfoByNameHash (Urho3D::StringHash nameHash) const
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

bool PlayersManager::IsColorUsed (Urho3D::Color color, Player *excludePlayer) const
{
    float smallestDifference = 1.0f;
    for (int index = 0; index < players_.Size (); index++)
    {
        Player *player = players_.Values ().At (index);
        if (player && player != excludePlayer)
        {
            Urho3D::Color playerColor = player->GetColor ();

            Urho3D::Vector3 difference;
            difference.x_ = color.r_ - playerColor.r_;
            difference.y_ = color.g_ - playerColor.g_;
            difference.z_ = color.b_ - playerColor.b_;

            if (difference.Length () < smallestDifference)
            {
                smallestDifference = difference.Length ();
            }
        }
    }

    return (smallestDifference < 0.2f);
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

Urho3D::PODVector <Player *> PlayersManager::GetPlayersByNames (Urho3D::PODVector<Urho3D::StringHash> namesHashes) const
{
    Urho3D::PODVector <Player *> players;
    for (int index = 0; index < namesHashes.Size (); index++)
    {
        Player ** player = players_ [namesHashes.At (index)];
        if (player)
        {
            players.Push (*player);
        }
    }
    return players;
}

Urho3D::PODVector <Player *> PlayersManager::GetAllPlayers () const
{
    Urho3D::PODVector <Player *> allPlayers;
    for (Urho3D::HashMap <Urho3D::StringHash, Player *>::ConstIterator iterator = players_.Begin ();
         iterator != players_.End (); iterator++)
    {
        allPlayers.Push (iterator->second_);
    }
    return allPlayers;
}

void PlayersManager::PlayerIdentified (Urho3D::Connection *connection, Urho3D::String name, Urho3D::Color color)
{
    DeleteIdentificatedConnection (connection);
    Player *player = new Player (context_, name, color, connection, node_->GetScene ());
    players_ [name] = player;
    connectionHashToNameHashMap_ [Urho3D::StringHash (connection->ToString ())] = Urho3D::StringHash (name);

    MessagesHandler *messagesHandler = node_->GetScene ()->GetComponent <MessagesHandler> ();
    assert (messagesHandler);
    Urho3D::PODVector <Player *> allPlayers = GetAllPlayers ();
    messagesHandler->SendTextInfoFromServer (player->GetName () + " entered game!", allPlayers);
    connection->SetScene (node_->GetScene ());
}

void PlayersManager::DisconnectPlayer (Urho3D::StringHash nameHash)
{
    Player *player = players_ [nameHash];
    assert (player);
    players_.Erase (nameHash);

    assert (player->GetConnection () || player->IsInternal ());
    if (player->GetConnection ())
    {
        connectionHashToNameHashMap_.Erase (Urho3D::StringHash (player->GetConnection ()->ToString ()));
    }
    player->Disconnect (0);
    delete player;
}

void PlayersManager::DisconnectPlayer (Urho3D::Connection *connection)
{
    if (connectionHashToNameHashMap_ [Urho3D::StringHash (connection->ToString ())].Value ())
    {
        DisconnectPlayer (connectionHashToNameHashMap_ [Urho3D::StringHash (connection->ToString ())]);
    }
}
}
