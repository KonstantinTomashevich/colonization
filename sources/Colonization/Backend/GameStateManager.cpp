#include <Colonization/BuildConfiguration.hpp>
#include "GameStateManager.hpp"
#include <Urho3D/Core/Context.h>
#include <Colonization/Backend/PlayersManager.hpp>

namespace Colonization
{
void GameStateManager::SetupWaitingForPlayersState ()
{
    // TODO: Implement later.
}

void GameStateManager::DisposeWaitingForPlayersState ()
{
    // TODO: Implement later.
}

void GameStateManager::SetupPlayingState ()
{
    // TODO: Implement later.
}

void GameStateManager::DisposePlayingState ()
{
    // TODO: Implement later.
}

void GameStateManager::SetupFinishedState ()
{
    // TODO: Implement later.
}

void GameStateManager::DisposeFinishedState ()
{
    // TODO: Implement later.
}

void GameStateManager::SetupState (GameState state)
{
    if (state == GAME_STATE_WAITING_FOR_PLAYERS)
        SetupWaitingForPlayersState ();
    else if (state == GAME_STATE_PLAYING)
        SetupPlayingState ();
    else if (state == GAME_STATE_FINISHED)
        SetupFinishedState ();
}

void GameStateManager::DisposeCurrentState ()
{
    if (state == GAME_STATE_WAITING_FOR_PLAYERS)
        DisposeWaitingForPlayersState ();
    else if (state == GAME_STATE_PLAYING)
        DisposePlayingState ();
    else if (state == GAME_STATE_FINISHED)
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

GameStateManager::GameStateManager (Urho3D::Context *context, int serverPort) : Urho3D::Object (context),
    gameState_ (GAME_STATE_UNITIALIZED)
{
    currentState_ = GAME_STATE_WAITING_FOR_PLAYERS;
    SetupState (currentState_);
}

GameStateManager::~GameStateManager ()
{
    DisposeCurrentState ();
}

GameState GameStateManager::GetCurrentState ()
{
    return currentState_;
}
}
