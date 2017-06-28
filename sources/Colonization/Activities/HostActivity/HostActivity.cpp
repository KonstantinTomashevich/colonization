#include <Colonization/BuildConfiguration.hpp>
#include "HostActivity.hpp"

#include <Urho3D/Network/Network.h>
#include <Urho3D/Core/Context.h>
#include <Urho3D/IO/Log.h>
#include <Urho3D/Engine/Engine.h>
#include <Urho3D/Graphics/Octree.h>
#include <Urho3D/Resource/XMLFile.h>

#include <Colonization/Core/Map.hpp>
#include <Colonization/Core/Unit/Unit.hpp>
#include <Colonization/Core/Unit/UnitEvents.hpp>
#include <Colonization/Core/GameConfiguration.hpp>
#include <Colonization/Utils/Network/NetworkUpdateSmoother.hpp>
#include <Colonization/Activities/HostActivity/HostActivityEvents.hpp>
#include <Colonization/Activities/HostActivity/HostActivityConstants.hpp>

#include <Colonization/Backend/ColoniesActionsProcessor.hpp>
#include <Colonization/Backend/ColoniesEvolutionManager.hpp>
#include <Colonization/Backend/MessagesHandler/MessagesHandler.hpp>
#include <Colonization/Backend/PlayersManager.hpp>
#include <Colonization/Backend/PlayersPointsCalculator.hpp>
#include <Colonization/Backend/TradeProcessor.hpp>
#include <Colonization/Backend/UnitsManager.hpp>
#include <Colonization/Backend/VictoryProgressUpdater/VictoryProgressUpdater.hpp>
#include <Colonization/Backend/Diplomacy/DiplomacyProcessor.hpp>

namespace Colonization
{
void HostActivity::WriteSceneReplicatedChildrenCount ()
{
    Urho3D::PODVector <Urho3D::Node *> children;
    scene_->GetChildren (children, true);
    int replicated = 0;
    for (int index = 0; index < children.Size (); index++)
    {
        if (children.At (index)->GetID () < Urho3D::FIRST_LOCAL_ID)
        {
            replicated++;
        }
    }
    scene_->SetVar (VAR_REPLICATED_NODES_COUNT, replicated);
}

void HostActivity::UpdateMapFolderAndMapInfoPathSceneVars ()
{
    if (currentState_ == GAME_STATE_WAITING_FOR_START)
    {
        scene_->SetVar (VAR_MAP_FOLDER, mapFolder_);
        scene_->SetVar (VAR_MAP_INFO_PATH, mapInfoPath_);
    }
}

void HostActivity::GoToNextStateIfNeeded ()
{
    if (currentState_ == GAME_STATE_WAITING_FOR_START && WillGoFromWaitingForStartToPlayingState ())
    {
        SetupPlayingState ();
    }
    else if (currentState_ == GAME_STATE_PLAYING && WillGoFromPlayingToFinishedState ())
    {
        SendInformationAboutGameEnd ();
        SetupFinishedState ();
    }
}

void HostActivity::SendCurrentStateToClients ()
{
    PlayersManager *playersManager = scene_->GetChild ("players")->GetComponent <PlayersManager> ();
    MessagesHandler *messagesHandler = scene_->GetComponent <MessagesHandler> ();
    if (playersManager && messagesHandler)
    {
        Urho3D::PODVector <Player *> players = playersManager->GetAllPlayers ();
        messagesHandler->SendGameState (currentState_, players);
    }
}

void HostActivity::SendInformationAboutGameEnd ()
{
    VictoryProgressUpdater *victoryProgressUpdater = scene_->GetComponent <VictoryProgressUpdater> ();
    PlayersManager *playersManager = scene_->GetChild ("players")->GetComponent <PlayersManager> ();
    MessagesHandler *messagesHandler = scene_->GetComponent <MessagesHandler> ();

    if (victoryProgressUpdater && playersManager && messagesHandler)
    {
        Urho3D::PODVector <Player *> players = playersManager->GetAllPlayers ();
        messagesHandler->SendGameEnded (victoryProgressUpdater->GetWinnerName (),
                                        victoryProgressUpdater->GetVictoryType (),
                                        victoryProgressUpdater->GetVictoryInfo (),
                                        players);
    }
}

void HostActivity::SetupWaitingForPlayersState ()
{
    currentState_ = GAME_STATE_WAITING_FOR_START;
    scene_->CreateComponent <Urho3D::Octree> (Urho3D::REPLICATED);
    scene_->CreateComponent <MessagesHandler> (Urho3D::LOCAL);
    scene_->CreateComponent <NetworkUpdateSmoother> (Urho3D::LOCAL);
    scene_->CreateChild ("players", Urho3D::REPLICATED)->CreateComponent <PlayersManager> (Urho3D::LOCAL);
}

void HostActivity::SetupPlayingState ()
{
    currentState_ = GAME_STATE_PLAYING;
    Urho3D::ResourceCache *resourceCache = context_->GetSubsystem <Urho3D::ResourceCache> ();
    Urho3D::String configurationPath;
    Urho3D::String mapPath;
    Urho3D::String unitsPath;
    Urho3D::String diplomacyPath;
    bool isMapLoaded = LoadAndParseMapInfo (configurationPath, mapPath, unitsPath, diplomacyPath);
    assert (isMapLoaded);

    LoadGameConfiguration (resourceCache, configurationPath);
    LoadMap (resourceCache, mapPath);
    LoadUnits (resourceCache, unitsPath);
    LoadDiplomacy (resourceCache, diplomacyPath);
    RecalculateUnitsHashes (scene_->GetChild ("units")->GetComponent <UnitsManager> ());
    CreateServerProcessorsAndManagers ();
    BanNewConnectionsAndSetStartGold ();
}

void HostActivity::SetupFinishedState ()
{
    currentState_ = GAME_STATE_FINISHED;
    scene_->GetChild ("units")->GetComponent <UnitsManager> ()->Remove ();
    scene_->GetComponent <ColoniesActionsProcessor> ()->Remove ();
    scene_->GetComponent <ColoniesEvolutionManager> ()->Remove ();
    scene_->GetComponent <TradeProcessor> ()->Remove ();
    scene_->GetComponent <PlayersPointsCalculator> ()->Remove ();
    scene_->GetComponent <VictoryProgressUpdater> ()->Remove ();
    scene_->GetChild ("diplomacy")->GetComponent <DiplomacyProcessor> ()->Remove ();
    scene_->SetUpdateEnabled (false);
}

void HostActivity::LoadGameConfiguration (Urho3D::ResourceCache *resourceCache, Urho3D::String configurationPath)
{
    Urho3D::Node *configurationNode = scene_->CreateChild ("configuration", Urho3D::LOCAL);
    configurationNode->LoadXML (resourceCache->GetResource <Urho3D::XMLFile> (configurationPath)->GetRoot ());
    GameConfiguration *configuration = configurationNode->GetComponent <GameConfiguration> ();
    scene_->CloneComponent (configuration, Urho3D::REPLICATED);
    configuration->Remove ();
}

void HostActivity::LoadMap (Urho3D::ResourceCache *resourceCache, Urho3D::String mapPath)
{
    Urho3D::Node *mapNode = scene_->CreateChild ("map", Urho3D::REPLICATED);
    mapNode->LoadXML (resourceCache->GetResource <Urho3D::XMLFile> (mapPath)->GetRoot ());
    Map *map = mapNode->GetComponent <Map> ();
    map->UpdateDistrictsList ();
}

void HostActivity::LoadUnits (Urho3D::ResourceCache *resourceCache, Urho3D::String unitsPath)
{
    Urho3D::Node *unitsNode = scene_->CreateChild ("units", Urho3D::REPLICATED);
    unitsNode->LoadXML (resourceCache->GetResource <Urho3D::XMLFile> (unitsPath)->GetRoot ());
    UnitsManager *unitsManager = unitsNode->CreateComponent <UnitsManager> (Urho3D::LOCAL);
    unitsManager->UpdateUnitsList ();
    for (int index = 0; index < unitsManager->GetUnitsCount (); index++)
    {
        Unit *unit = unitsManager->GetUnitByIndex (index);
        Urho3D::VariantMap eventData;
        eventData [UnitCreated::UNIT_HASH] = unit->GetHash ();
        SendEvent (EVENT_UNIT_CREATED, eventData);
    }
}

void HostActivity::LoadDiplomacy (Urho3D::ResourceCache *resourceCache, Urho3D::String diplomacyPath)
{
    Urho3D::Node *diplomacyNode = scene_->CreateChild ("diplomacy", Urho3D::REPLICATED);
    diplomacyNode->LoadXML (resourceCache->GetResource <Urho3D::XMLFile> (diplomacyPath)->GetRoot ());
    DiplomacyProcessor *diplomacyProcessor = diplomacyNode->CreateComponent <DiplomacyProcessor> (Urho3D::LOCAL);
    diplomacyProcessor->UpdateWarsList ();
}

void HostActivity::RecalculateUnitsHashes (UnitsManager *unitsManager)
{
    for (int index = 0; index < unitsManager->GetUnitsCount (); index++)
    {
        unitsManager->GetUnitByIndex (index)->UpdateHash (unitsManager);
    }
}

void HostActivity::CreateServerProcessorsAndManagers ()
{
    scene_->CreateComponent <ColoniesActionsProcessor> (Urho3D::LOCAL);
    scene_->CreateComponent <ColoniesEvolutionManager> (Urho3D::LOCAL);
    scene_->CreateComponent <TradeProcessor> (Urho3D::LOCAL);
    scene_->CreateComponent <PlayersPointsCalculator> (Urho3D::LOCAL);
    scene_->CreateComponent <VictoryProgressUpdater> (Urho3D::LOCAL);
}

void HostActivity::BanNewConnectionsAndSetStartGold ()
{
    PlayersManager *playersManager = scene_->GetChild ("players")->GetComponent <PlayersManager> ();
    playersManager->SetIsAcceptingNewConnections (false);
    playersManager->SetStartGoldForAllPlayers ();
}

bool HostActivity::LoadAndParseMapInfo (Urho3D::String &configurationPath, Urho3D::String &mapPath, Urho3D::String &unitsPath, Urho3D::String &diplomacyPath)
{
    Urho3D::ResourceCache *resourceCache = context_->GetSubsystem <Urho3D::ResourceCache> ();
    Urho3D::XMLFile *infoFile = resourceCache->GetResource <Urho3D::XMLFile> (mapInfoPath_);

    if (!infoFile || !infoFile->GetRoot ().HasChild ("mapFiles"))
    {
        return false;
    }

    Urho3D::XMLElement filesInfoRoot = infoFile->GetRoot ().GetChild ("mapFiles");
    configurationPath = mapFolder_ + filesInfoRoot.GetAttribute ("gameConfigurationXML");
    mapPath = mapFolder_ + filesInfoRoot.GetAttribute ("mapXML");
    unitsPath = mapFolder_ + filesInfoRoot.GetAttribute ("unitsXML");
    diplomacyPath = mapFolder_ + filesInfoRoot.GetAttribute ("diplomacyPath");
    return true;
}

bool HostActivity::WillGoFromWaitingForStartToPlayingState ()
{
    PlayersManager *playersManager = scene_->GetChild ("players")->GetComponent <PlayersManager> ();
    if (isStartRequested_ && playersManager->GetPlayersCount () > 0 &&
            mapFolder_ != Urho3D::String::EMPTY && mapInfoPath_ != Urho3D::String::EMPTY)
    {
        Urho3D::PODVector <Player *> players = playersManager->GetAllPlayers ();
        bool isAllReadyForStart = true;
        int index = 0;

        while (index < players.Size () && isAllReadyForStart)
        {
            Player *player = players.At (index);
            if (player && !player->IsReadyForStart ())
            {
                isAllReadyForStart = false;
            }
            index++;
        }

        if (isAllReadyForStart)
        {
            isStartRequested_ = false;
            MessagesHandler *messagesHandler = scene_->GetComponent <MessagesHandler> ();
            if (messagesHandler)
            {
                messagesHandler->SendTextInfoFromServer ("Starting game...", players);
            }
            return true;
        }
        else
        {
            isStartRequested_ = false;
            MessagesHandler *messagesHandler = scene_->GetComponent <MessagesHandler> ();
            if (messagesHandler)
            {
                messagesHandler->SendTextInfoFromServer ("Host wants to start game...", players);
            }
            return false;
        }
    }
    else
    {
        return false;
    }
}

bool HostActivity::WillGoFromPlayingToFinishedState ()
{
    VictoryProgressUpdater *victoryProgressUpdater = scene_->GetComponent <VictoryProgressUpdater> ();
    return victoryProgressUpdater->IsAnyoneWon ();
}

HostActivity::HostActivity (Urho3D::Context *context) : Activity (context),
    serverPort_ (13534),
    isStartRequested_ (false),
    mapFolder_ (Urho3D::String::EMPTY),
    mapInfoPath_ (Urho3D::String::EMPTY),
    scene_ (new Urho3D::Scene (context))
{

}

HostActivity::~HostActivity ()
{

}

unsigned short HostActivity::GetServerPort () const
{
    return serverPort_;
}

void HostActivity::SetServerPort (unsigned short serverPort)
{
    serverPort_ = serverPort;
}

Urho3D::Scene *HostActivity::GetScene () const
{
    return scene_;
}

Urho3D::String HostActivity::GetMapFolder () const
{
    return mapFolder_;
}

void HostActivity::SetMapFolder (Urho3D::String mapFolder)
{
    mapFolder_ = mapFolder;
}

Urho3D::String HostActivity::GetMapInfoPath () const
{
    return mapInfoPath_;
}

void HostActivity::SetMapInfoPath (Urho3D::String mapInfoPath)
{
    mapInfoPath_ = mapInfoPath;
}

bool HostActivity::IsStartRequested () const
{
    return isStartRequested_;
}

void HostActivity::HandleGameStartRequest (Urho3D::StringHash eventType, Urho3D::VariantMap &eventData)
{
    isStartRequested_ = true;
}

void HostActivity::HandleKickPlayerRequest (Urho3D::StringHash eventType, Urho3D::VariantMap &eventData)
{
    Urho3D::String playerName = eventData [HostRequestKickPlayer::PLAYER_NAME].GetString ();
    PlayersManager *playersManager = scene_->GetChild ("players")->GetComponent <PlayersManager> ();
    if (playersManager && playersManager->GetPlayerByNameHash (Urho3D::StringHash (playerName)))
    {
        playersManager->DisconnectPlayer (Urho3D::StringHash (playerName));
    }
}

void HostActivity::HandleSelectMapRequest (Urho3D::StringHash eventType, Urho3D::VariantMap &eventData)
{
    SetMapFolder (eventData [HostRequestSelectMap::MAP_FOLDER].GetString ());
    SetMapInfoPath (eventData [HostRequestSelectMap::MAP_INFO_PATH].GetString ());
}

void HostActivity::Start ()
{
    bool isServerStarted = context_->GetSubsystem <Urho3D::Network> ()->StartServer (serverPort_);
    assert (isServerStarted);
    if (!isServerStarted)
    {
        Urho3D::Log::Write (Urho3D::LOG_ERROR, "Can't start server!");
        context_->GetSubsystem <Urho3D::Engine> ()->Exit ();
    }
    SetupWaitingForPlayersState ();

    SubscribeToEvent (Urho3D::StringHash (EVENT_HOST_REQUEST_GAME_START), URHO3D_HANDLER (HostActivity, HandleGameStartRequest));
    SubscribeToEvent (Urho3D::StringHash (EVENT_HOST_REQUEST_KICK_PLAYER), URHO3D_HANDLER (HostActivity, HandleKickPlayerRequest));
    SubscribeToEvent (Urho3D::StringHash (EVENT_HOST_REQUEST_SELECT_MAP), URHO3D_HANDLER (HostActivity, HandleSelectMapRequest));
}

void HostActivity::Update (float timeStep)
{
    WriteSceneReplicatedChildrenCount ();
    UpdateMapFolderAndMapInfoPathSceneVars ();
    GoToNextStateIfNeeded ();
    SendCurrentStateToClients ();
}

void HostActivity::Stop ()
{
    bool clearReplicated = true;
    bool clearLocal = true;
    scene_->Clear (clearReplicated, clearLocal);
    context_->GetSubsystem <Urho3D::Network> ()->StopServer ();
}
}
