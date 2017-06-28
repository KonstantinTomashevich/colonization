#pragma once
#include <Urho3D/Scene/Scene.h>
#include <Urho3D/Resource/ResourceCache.h>
#include <Colonization/Utils/Activities/Activity.hpp>
#include <Colonization/Backend/UnitsManager.hpp>
#include <Colonization/Activities/HostActivity/GameStateType.hpp>

namespace Colonization
{
//@ASBindGen Class ObjectType=Ref
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
    void LoadDiplomacy (Urho3D::ResourceCache *resourceCache, Urho3D::String diplomacyPath);
    void LoadBattles (Urho3D::ResourceCache *resourceCache, Urho3D::String battlesPath);
    void RecalculateUnitsHashes (Colonization::UnitsManager *unitsManager);
    void CreateServerProcessorsAndManagers ();
    void BanNewConnectionsAndSetStartGold ();

    /// Parameters are parse output.
    bool LoadAndParseMapInfo (Urho3D::String &configurationPath, Urho3D::String &mapPath, Urho3D::String &unitsPath,
                              Urho3D::String &diplomacyPath, Urho3D::String &battlesPath);
    bool WillGoFromWaitingForStartToPlayingState ();
    bool WillGoFromPlayingToFinishedState ();
public:
    //@ASBindGen Constructor UseUrho3DScriptContext_arg0
    HostActivity (Urho3D::Context *context);
    virtual ~HostActivity ();

    //@ASBindGen Function OverrideName=get_serverPort
    unsigned short GetServerPort () const;
    //@ASBindGen Function OverrideName=set_serverPort
    void SetServerPort (unsigned short serverPort);
    //@ASBindGen Function AddRef_arg-1 OverrideName=get_scene
    Urho3D::Scene *GetScene () const;

    //@ASBindGen Function OverrideName=get_mapFolder
    Urho3D::String GetMapFolder () const;
    //@ASBindGen Function OverrideName=set_mapFolder
    void SetMapFolder (Urho3D::String mapFolder);

    //@ASBindGen Function OverrideName=get_mapInfoPath
    Urho3D::String GetMapInfoPath () const;
    //@ASBindGen Function OverrideName=set_mapInfoPath
    void SetMapInfoPath (Urho3D::String mapInfoPath);

    //@ASBindGen Function
    bool IsStartRequested () const;
    void HandleGameStartRequest (Urho3D::StringHash eventType, Urho3D::VariantMap &eventData);
    void HandleKickPlayerRequest (Urho3D::StringHash eventType, Urho3D::VariantMap &eventData);
    void HandleSelectMapRequest (Urho3D::StringHash eventType, Urho3D::VariantMap &eventData);

    virtual void Start ();
    virtual void Update (float timeStep);
    virtual void Stop ();
};
}
