#pragma once
#include <Urho3D/Scene/Scene.h>
#include <Colonization/Utils/Activity.hpp>

namespace Colonization
{
enum GameStateType
{
    GAME_STATE_UNITIALIZED = -1,
    GAME_STATE_WAITING_FOR_PLAYERS = 0,
    GAME_STATE_PLAYING = 1,
    GAME_STATE_FINISHED = 2
};

class HostActivity : public Activity
{
URHO3D_OBJECT (HostActivity, Activity)
protected:
    Urho3D::Scene *scene_;
    unsigned short serverPort_;
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
    HostActivity (Urho3D::Context *context);
    virtual ~HostActivity ();

    unsigned short GetServerPort ();
    void SetServerPort (unsigned short serverPort);
    Urho3D::Scene *GetScene ();

    virtual void Start ();
    virtual void Update (float timeStep);
    virtual void Stop ();
};
}
