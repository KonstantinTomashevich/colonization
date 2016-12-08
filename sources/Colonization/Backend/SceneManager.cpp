#include <Colonization/BuildConfiguration.hpp>
#include "SceneManager.hpp"
#include <Urho3D/Core/CoreEvents.h>

namespace Colonization
{
void SceneManager::UpdateWaitingForPlayersState ()
{
    // TODO: Implement later.
}

void SceneManager::UpdatePlayingState ()
{
    // TODO: Implement later.
}

void SceneManager::UpdateFinishedState ()
{
    // TODO: Implement later.
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
}

SceneManager::~SceneManager ()
{
    scene_->RemoveAllComponents ();
    scene_->RemoveAllChildren ();
    delete scene_;
}

void SceneManager::Update (Urho3D::StringHash eventType, Urho3D::VariantMap &eventData)
{
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
