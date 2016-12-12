#include <Colonization/BuildConfiguration.hpp>
#include "SceneManager.hpp"
#include <Urho3D/Core/CoreEvents.h>
#include <Urho3D/Core/Context.h>
#include <Urho3D/Network/Network.h>
#include <Urho3D/Graphics/Octree.h>
#include <Urho3D/IO/Log.h>

#include <Colonization/Backend/PlayersManager.hpp>
#include <Colonization/Backend/UnitsManager.hpp>
#include <Colonization/Core/Map.hpp>

namespace Colonization
{
void SceneManager::WritePlayersStats ()
{
    PlayersManager *playersManager = (PlayersManager *) context_->GetGlobalVar ("PlayersManager").GetPtr ();
    assert (playersManager);
    Urho3D::Vector <Player *> players = playersManager->GetAllPlayers ();
    Urho3D::HashMap <Urho3D::StringHash, Player*> sortedPlayers;

    for (int index = 0; index < players.Size (); index++)
    {
        Player *player = players.At (index);
        sortedPlayers [Urho3D::StringHash (static_cast <int> (player->GetPoints () * 1000))] = player;
    }

    sortedPlayers.Sort ();
    Urho3D::VariantVector playersLeaderboard;
    for (int index = sortedPlayers.Size () - 1; index >= 0; index--)
    {
        Player *player = sortedPlayers.Values ().At (index);
        Urho3D::String infoString;
        infoString += player->GetName ();
        infoString += ";" + Urho3D::String (player->GetPoints ());
        playersLeaderboard.Push (infoString);
    }
    scene_->SetVar ("sortedByPointsPlayersList", playersLeaderboard);
}

void SceneManager::UpdateMap (bool rewriteDistrictsPolygons)
{
    Map *map = (Map *) context_->GetGlobalVar ("Map").GetPtr ();
    assert (map);
    Urho3D::Node *mapNode = scene_->GetChild ("map");
    map->UpdateDataNode (mapNode, rewriteDistrictsPolygons);
}

void SceneManager::UpdateUnits ()
{
    UnitsManager *unitsManager = (UnitsManager *) context_->GetGlobalVar ("UnitsManager").GetPtr ();
    Urho3D::Node *unitsNode = scene_->GetChild ("units");
    unitsManager->GetUnitsContainer ()->UpdateDataNode (unitsNode);
}

void SceneManager::WriteChildrenCount ()
{
    Urho3D::PODVector <Urho3D::Node *> children;
    scene_->GetChildren (children, true);
    int replicated = 0;
    for (int index = 0; index < children.Size (); index++)
        if (children.At (index)->GetID () < Urho3D::FIRST_LOCAL_ID)
            replicated++;
    scene_->SetVar ("ReplicatedNodesCount", replicated);
}

void SceneManager::UpdateWaitingForPlayersState ()
{
    // TODO: To be continued....
    WritePlayersStats ();
}

void SceneManager::UpdatePlayingState ()
{
    // TODO: To be continued...
    WritePlayersStats ();
    UpdateMap (false);
    WriteChildrenCount ();
    UpdateUnits ();
}

void SceneManager::UpdateFinishedState ()
{
    // TODO: To be continued...
    WritePlayersStats ();
}

void SceneManager::PrepareForWaitingForPlayersState ()
{
    // TODO: Implement later.
    scene_->SetVar ("GameState", static_cast <int> (GAME_STATE_WAITING_FOR_PLAYERS));
}

void SceneManager::PrepareForPlayingState ()
{
    // TODO: Implement later.
    scene_->SetVar ("GameState", static_cast <int> (GAME_STATE_PLAYING));
    Urho3D::Node *mapNode = scene_->CreateChild ("map", Urho3D::REPLICATED);
    mapNode->SetVar ("PrefabXMLPath", "Objects/TestMapLocal.xml");
    Urho3D::Node *unitsNode = scene_->CreateChild ("units", Urho3D::REPLICATED);

    UpdateMap (true);
    UpdateUnits ();
    WriteChildrenCount ();
}

void SceneManager::PrepareForFinishedState ()
{
    // TODO: Implement later.
    scene_->SetVar ("GameState", static_cast <int> (GAME_STATE_FINISHED));
}

SceneManager::SceneManager (Urho3D::Context *context) : Urho3D::Object (context),
    scene_ (new Urho3D::Scene (context))
{
    SubscribeToEvent (Urho3D::E_UPDATE, URHO3D_HANDLER (SceneManager, Update));
    scene_->CreateComponent <Urho3D::Octree> (Urho3D::REPLICATED);
}

SceneManager::~SceneManager ()
{
    scene_->RemoveAllComponents ();
    scene_->RemoveAllChildren ();
    delete scene_;
}

void SceneManager::Update (Urho3D::StringHash eventType, Urho3D::VariantMap &eventData)
{
    if (context_->GetSubsystem <Urho3D::Network> ()->GetServerConnection () &&
            !context_->GetSubsystem <Urho3D::Network> ()->GetServerConnection ()->GetScene ())
        context_->GetSubsystem <Urho3D::Network> ()->GetServerConnection ()->SetScene (scene_);

    GameStateType gameState = static_cast <GameStateType> (scene_->GetVar ("GameState").GetInt ());
    if (gameState == GAME_STATE_WAITING_FOR_PLAYERS)
        UpdateWaitingForPlayersState ();
    else if (gameState == GAME_STATE_PLAYING)
        UpdatePlayingState ();
    else if (gameState == GAME_STATE_FINISHED)
        UpdateFinishedState ();
}

Urho3D::Scene *SceneManager::GetScene ()
{
    return scene_;
}

void SceneManager::PrepareForGameState (GameStateType gameState)
{
    assert (gameState != GAME_STATE_UNITIALIZED);
    if (gameState == GAME_STATE_WAITING_FOR_PLAYERS)
        PrepareForWaitingForPlayersState ();
    else if (gameState == GAME_STATE_PLAYING)
        PrepareForPlayingState ();
    else if (gameState == GAME_STATE_FINISHED)
        PrepareForFinishedState ();
}
}
