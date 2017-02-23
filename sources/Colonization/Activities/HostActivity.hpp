#pragma once
#include <Urho3D/Scene/Scene.h>
#include <Colonization/Utils/Activity.hpp>

namespace Colonization
{
enum GameStateType
{
    GAME_STATE_UNITIALIZED = -1,
    GAME_STATE_WAITING_FOR_START = 0,
    GAME_STATE_PLAYING = 1,
    GAME_STATE_FINISHED = 2
};

class HostActivity : public Activity
{
URHO3D_OBJECT (HostActivity, Activity)
protected:
    Urho3D::SharedPtr <Urho3D::Scene> scene_;
    unsigned short serverPort_;
    Urho3D::String mapFolder_;
    Urho3D::String mapInfoPath_;
    GameStateType currentState_;

    void SetupWaitingForPlayersState ();
    void DisposeWaitingForPlayersState ();

    void SetupPlayingState ();
    void DisposePlayingState ();

    void SetupFinishedState ();
    void DisposeFinishedState ();

    void SetupState (GameStateType state);
    void DisposeCurrentState ();

    // Parameters are parse output.
    bool LoadAndParseMapInfo (Urho3D::String &configurationPath, Urho3D::String &mapPath, Urho3D::String &unitsPath);
    bool WillIGoFromWaitingForStartToPlayingState ();
    bool WillIGoFromPlayingToFinishedState ();
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

    virtual void Start ();
    virtual void Update (float timeStep);
    virtual void Stop ();
};
}
