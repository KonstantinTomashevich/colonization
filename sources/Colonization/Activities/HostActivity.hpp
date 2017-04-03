#pragma once
#include <Urho3D/Scene/Scene.h>
#include <Urho3D/Resource/ResourceCache.h>
#include <Colonization/Utils/Activities/Activity.hpp>
#include <Colonization/Backend/UnitsManager.hpp>

namespace Colonization
{
// TODO: Maybe split to HostActivity, GameStateType, HostActivityEvents, HostActivityConstants.
// Declare host activity request events.
const Urho3D::String EVENT_HOST_REQUEST_GAME_START ("HostRequestGameStart");
const Urho3D::String EVENT_HOST_REQUEST_KICK_PLAYER ("HostRequestKickPlayer");
namespace HostRequestKickPlayer
{
const Urho3D::String PLAYER_NAME ("PlayerName");
}

const Urho3D::String EVENT_HOST_REQUEST_SELECT_MAP ("HostRequestSelectMap");
namespace HostRequestSelectMap
{
const Urho3D::String MAP_FOLDER ("MapFolder");
const Urho3D::String MAP_INFO_PATH ("MapInfoPath");
}

const Urho3D::StringHash VAR_REPLICATED_NODES_COUNT ("ReplicatedNodesCount");
const Urho3D::StringHash VAR_MAP_FOLDER ("MapFolder");
const Urho3D::StringHash VAR_MAP_INFO_PATH ("MapInfoPath");

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

    void WriteSceneReplicatedChildrenCount ();
    void UpdateMapFolderAndMapInfoPathSceneVars ();
    void GoToNextStateIfNeeded ();
    void SendCurrentStateToClients ();
    void SendInformationAboutGameEnd ();

    void SetupWaitingForPlayersState ();
    void SetupPlayingState ();
    void SetupFinishedState ();

    void LoadGameConfiguration (Urho3D::ResourceCache *resourceCache, Urho3D::String configurationPath);
    void LoadMap (Urho3D::ResourceCache *resourceCache, Urho3D::String mapPath);
    void LoadUnits (Urho3D::ResourceCache *resourceCache, Urho3D::String unitsPath);
    void RecalculateUnitsHashes (Colonization::UnitsManager *unitsManager);
    void CreateServerProcessorsAndManagers ();
    void BanNewConnectionsAndSetStartGold ();

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
    void HandleGameStartRequest (Urho3D::StringHash eventType, Urho3D::VariantMap &eventData);
    void HandleKickPlayerRequest (Urho3D::StringHash eventType, Urho3D::VariantMap &eventData);
    void HandleSelectMapRequest (Urho3D::StringHash eventType, Urho3D::VariantMap &eventData);

    virtual void Start ();
    virtual void Update (float timeStep);
    virtual void Stop ();
};
}
