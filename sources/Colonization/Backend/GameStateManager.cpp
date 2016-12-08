#include <Colonization/BuildConfiguration.hpp>
#include "GameStateManager.hpp"
#include <Urho3D/Core/Context.h>

#include <Colonization/Backend/MessagesHandler.hpp>
#include <Colonization/Backend/PlayersManager.hpp>
#include <Colonization/Backend/SceneManager.hpp>
#include <Colonization/Core/Map.hpp>

namespace Colonization
{
void CreateTestMap (Map *map)
{
    // Map from TestMapPathFinding.
    const int mapWidth = 5;
    const int mapHeight = 5;

    for (int x = 0; x < mapWidth; x++)
        for (int y = 0; y < mapHeight; y++)
        {
            Colonization::District *district = new District (map->GetContext ());
            district->name_ = "(" + Urho3D::String (x) + "; " + Urho3D::String (y) + ")";

            district->polygonPoints_.Push (Urho3D::Vector3 (x - 0.5f, 0.0f, y - 0.5f));
            district->polygonPoints_.Push (Urho3D::Vector3 (x + 0.5f, 0.0f, y - 0.5f));
            district->polygonPoints_.Push (Urho3D::Vector3 (x + 0.5f, 0.0f, y + 0.5f));
            district->polygonPoints_.Push (Urho3D::Vector3 (x - 0.5f, 0.0f, y + 0.5f));

            district->unitPosition_ = Urho3D::Vector3 (x, 0.0f, y);
            district->colonyPosition_ = Urho3D::Vector3 (x, 0.0f, y);
            map->AddDistrict (district);
        }

    // Map: (~ -- sea, = -- terrain, @ -- colony)
    //   0 1 2 3 4
    // 0 ~ ~ ~ ~ ~
    // 1 ~ = = = ~
    // 2 ~ ~ ~ = ~
    // 3 ~ = @ = ~
    // 4 ~ = ~ ~ ~
    // Start point: (0, 2).
    // End point: (4, 4).

    // array (X * HEIGHT + Y) = (X, Y)
    map->GetDistrictByIndex (1 * mapHeight + 1)->isSea_ = false;
    map->GetDistrictByIndex (2 * mapHeight + 1)->isSea_ = false;
    map->GetDistrictByIndex (3 * mapHeight + 1)->isSea_ = false;
    map->GetDistrictByIndex (3 * mapHeight + 2)->isSea_ = false;
    map->GetDistrictByIndex (3 * mapHeight + 3)->isSea_ = false;
    map->GetDistrictByIndex (2 * mapHeight + 3)->isSea_ = false;
    map->GetDistrictByIndex (1 * mapHeight + 3)->isSea_ = false;
    map->GetDistrictByIndex (1 * mapHeight + 4)->isSea_ = false;
    map->UpdateNeighborsOfDistricts ();
}

void GameStateManager::SetupWaitingForPlayersState ()
{
    // TODO: To be continued...
    MessagesHandler *messagesHandler = new MessagesHandler (context_);
    PlayersManager *playersManager = new PlayersManager (context_);

    SceneManager *sceneManager = new SceneManager (context_);
    sceneManager->PrepareForGameState (GAME_STATE_WAITING_FOR_PLAYERS);

    context_->SetGlobalVar ("MessagesHandler", Urho3D::Variant (messagesHandler));
    context_->SetGlobalVar ("PlayersManager", Urho3D::Variant (playersManager));
    context_->SetGlobalVar ("SceneManager", Urho3D::Variant (sceneManager));
}

void GameStateManager::DisposeWaitingForPlayersState ()
{
    // TODO: Implement later.
}

void GameStateManager::SetupPlayingState ()
{
    // TODO: To be continued...
    Map *map = new Map (context_);
    CreateTestMap (map);
    context_->SetGlobalVar ("Map", Urho3D::Variant (map));

    SceneManager *sceneManager = (SceneManager *) context_->GetGlobalVar ("SceneManager").GetPtr ();
    assert (sceneManager);
    sceneManager->PrepareForGameState (GAME_STATE_PLAYING);
}

void GameStateManager::DisposePlayingState ()
{
    // TODO: Temporary. Reimplement later.
    delete ( (Map *) context_->GetGlobalVar ("Map").GetPtr ());
    SetupState (GAME_STATE_FINISHED);
    DisposeCurrentState ();
}

void GameStateManager::SetupFinishedState ()
{
    // TODO: Implement later.
}

void GameStateManager::DisposeFinishedState ()
{
    // TODO: To be continued...
    delete ( (MessagesHandler *) context_->GetGlobalVar ("MessagesHandler").GetPtr ());
    delete ( (PlayersManager *) context_->GetGlobalVar ("PlayersManager").GetPtr ());
    delete ( (SceneManager *) context_->GetGlobalVar ("SceneManager").GetPtr ());
}

void GameStateManager::SetupState (GameStateType state)
{
    if (state == GAME_STATE_WAITING_FOR_PLAYERS)
        SetupWaitingForPlayersState ();
    else if (state == GAME_STATE_PLAYING)
        SetupPlayingState ();
    else if (state == GAME_STATE_FINISHED)
        SetupFinishedState ();
    currentState_ = state;
}

void GameStateManager::DisposeCurrentState ()
{
    if (currentState_ == GAME_STATE_WAITING_FOR_PLAYERS)
        DisposeWaitingForPlayersState ();
    else if (currentState_ == GAME_STATE_PLAYING)
        DisposePlayingState ();
    else if (currentState_ == GAME_STATE_FINISHED)
        DisposeFinishedState ();
}

bool GameStateManager::WillIGoFromWaitingForPlayersToPlayingState ()
{
    PlayersManager *playersManager = (PlayersManager *) context_->GetGlobalVar ("PlayersManager").GetPtr ();
    // TODO: Reimplement later!
    return (playersManager->GetAllPlayers ().Size () > 0);
}

bool GameStateManager::WillIGoFromPlayingToFinishedState ()
{
    // TODO: Implement later.
    return false;
}

GameStateManager::GameStateManager (Urho3D::Context *context) : Urho3D::Object (context),
    currentState_ (GAME_STATE_UNITIALIZED)
{
    SetupState (GAME_STATE_WAITING_FOR_PLAYERS);
}

GameStateManager::~GameStateManager ()
{
    DisposeCurrentState ();
}

void GameStateManager::Update (float timeStep)
{
    if (currentState_ == GAME_STATE_WAITING_FOR_PLAYERS && WillIGoFromWaitingForPlayersToPlayingState ())
        SetupState (GAME_STATE_PLAYING);
    else if (currentState_ == GAME_STATE_PLAYING && WillIGoFromPlayingToFinishedState ())
        SetupState (GAME_STATE_FINISHED);
}

GameStateType GameStateManager::GetCurrentState ()
{
    return currentState_;
}
}
