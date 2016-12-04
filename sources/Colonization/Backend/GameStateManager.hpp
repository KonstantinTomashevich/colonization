#pragma once
#include <Urho3D/Core/Object.h>

namespace Colonization
{
enum GameStateType
{
    GAME_STATE_UNITIALIZED = -1,
    GAME_STATE_WAITING_FOR_PLAYERS = 0,
    GAME_STATE_PLAYING = 1,
    GAME_STATE_FINISHED = 2
};

class GameStateManager : public Urho3D::Object
{
URHO3D_OBJECT (GameStateManager, Object)
protected:
    GameStateType currentState_;

    void SetupWaitingForPlayersState ();
    void DisposeWaitingForPlayersState ();

    void SetupPlayingState ();
    void DisposePlayingState ();

    void SetupFinishedState ();
    void DisposeFinishedState ();

    void SetupState (GameStateType state);
    void DisposeCurrentState ();

    bool WillIGoFromWaitingForPlayersToPlayingState ();
    bool WillIGoFromPlayingToFinishedState ();
public:
    GameStateManager (Urho3D::Context *context, int serverPort = 13751);
    virtual ~GameStateManager ();
    void Update (Urho3D::StringHash eventType, Urho3D::VariantMap &eventData);
    GameStateType GetCurrentState ();
};
}

