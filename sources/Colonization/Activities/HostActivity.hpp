#pragma once
#include <Urho3D/Scene/Scene.h>
#include <Colonization/Utils/Activities/Activity.hpp>

namespace Colonization
{
const Urho3D::String EVENT_HOST_REQUEST_GAME_START ("HostRequestGameStart");
enum GameStateType
{
    GAME_STATE_UNITIALIZED = 0,
    GAME_STATE_WAITING_FOR_START = 1,
    GAME_STATE_PLAYING = 2,
    GAME_STATE_FINISHED = 3
};

class HostActivity : public Activity
{
URHO3D_OBJECT (HostActivity, Activity)
protected:
    Urho3D::SharedPtr <Urho3D::Scene> scene_;
    unsigned short serverPort_;
    bool isStartRequested_;
    Urho3D::String mapFolder_;
    Urho3D::String mapInfoPath_;
    GameStateType currentState_;

    // TODO: Refactor states mechanism.
    // In my opinion, "SetupState (state)" and states dispose isn't needed,
    // because game goes only from waiting to playing and then to finished states.
    void SetupWaitingForPlayersState ();
    void DisposeWaitingForPlayersState ();

    void SetupPlayingState ();
    void DisposePlayingState ();

    void SetupFinishedState ();
    void DisposeFinishedState ();

    void SetupState (GameStateType state);
    void DisposeCurrentState ();

    /// Parameters are parse output.
    bool LoadAndParseMapInfo (Urho3D::String &configurationPath, Urho3D::String &mapPath, Urho3D::String &unitsPath);
    bool WillGoFromWaitingForStartToPlayingState ();
    bool WillGoFromPlayingToFinishedState ();
public:
    HostActivity (Urho3D::Context *context);
    virtual ~HostActivity ();

    unsigned short GetServerPort () const;
    void SetServerPort (unsigned short serverPort);
    Urho3D::Scene *GetScene () const;

    Urho3D::String GetMapFolder () const;
    void SetMapFolder (Urho3D::String mapFolder);

    Urho3D::String GetMapInfoPath () const;
    void SetMapInfoPath (Urho3D::String mapInfoPath);

    bool IsStartRequested () const;
    void HandleGamerStartRequest (Urho3D::StringHash eventType, Urho3D::VariantMap &eventData);

    virtual void Start ();
    virtual void Update (float timeStep);
    virtual void Stop ();
};
}
