#include <Colonization/BuildConfiguration.hpp>
#include "HostActivity.hpp"

#include <Urho3D/Network/Network.h>
#include <Urho3D/Core/Context.h>
#include <Urho3D/IO/Log.h>

#include <Urho3D/Graphics/Octree.h>
#include <Urho3D/Resource/ResourceCache.h>
#include <Urho3D/Resource/XMLFile.h>

#include <Colonization/Core/District.hpp>
#include <Colonization/Core/Map.hpp>
#include <Colonization/Core/Unit.hpp>
#include <Colonization/Core/GameConfiguration.hpp>

#include <Colonization/Backend/ColoniesManager.hpp>
#include <Colonization/Backend/MessagesHandler.hpp>
#include <Colonization/Backend/PlayersManager.hpp>
#include <Colonization/Backend/PlayersPointsCalculator.hpp>
#include <Colonization/Backend/TradeProcessor.hpp>
#include <Colonization/Backend/UnitsManager.hpp>
#include <Colonization/Backend/VictoryProgressUpdater.hpp>

namespace Colonization
{
void HostActivity::SetupWaitingForPlayersState ()
{
    scene_->CreateComponent <Urho3D::Octree> (Urho3D::REPLICATED);
    scene_->CreateComponent <MessagesHandler> (Urho3D::LOCAL);
    scene_->CreateChild ("players", Urho3D::REPLICATED)->CreateComponent <PlayersManager> (Urho3D::LOCAL);
}

void HostActivity::DisposeWaitingForPlayersState ()
{

}

void HostActivity::SetupPlayingState ()
{
    Urho3D::ResourceCache *resourceCache = context_->GetSubsystem <Urho3D::ResourceCache> ();
    // Load map info and parse it.
    Urho3D::String configurationPath;
    Urho3D::String mapPath;
    Urho3D::String unitsPath;
    assert (LoadAndParseMapInfo (configurationPath, mapPath, unitsPath));

    // Load game configuration.
    Urho3D::Node *configurationNode = scene_->CreateChild ("configuration", Urho3D::LOCAL);
    configurationNode->LoadXML (resourceCache->GetResource <Urho3D::XMLFile> (configurationPath)->GetRoot ());
    GameConfiguration *configuration = configurationNode->GetComponent <GameConfiguration> ();
    scene_->CloneComponent (configuration, Urho3D::REPLICATED);
    configuration->Remove ();

    // Load game map.
    Urho3D::Node *mapNode = scene_->CreateChild ("map", Urho3D::REPLICATED);
    mapNode->LoadXML (resourceCache->GetResource <Urho3D::XMLFile> (mapPath)->GetRoot ());
    Map *map = mapNode->GetComponent <Map> ();
    map->UpdateDistrictsList ();

    // Load units and create units manager.
    Urho3D::Node *unitsNode = scene_->CreateChild ("units", Urho3D::REPLICATED);
    unitsNode->LoadXML (resourceCache->GetResource <Urho3D::XMLFile> (unitsPath)->GetRoot ());
    UnitsManager *unitsManager = unitsNode->CreateComponent <UnitsManager> (Urho3D::LOCAL);
    unitsManager->UpdateUnitsList ();

    // Recalculate units hashes.
    Urho3D::PODVector <Urho3D::Node *> units;
    unitsNode->GetChildrenWithComponent <Unit> (units);
    for (int index = 0; index < units.Size (); index++)
    {
        units.At (index)->GetComponent <Unit> ()->UpdateHash (unitsManager);
    }

    // Create server side components.
    scene_->CreateComponent <ColoniesManager> (Urho3D::LOCAL);
    scene_->CreateComponent <TradeProcessor> (Urho3D::LOCAL);
    scene_->CreateComponent <PlayersPointsCalculator> (Urho3D::LOCAL);
    scene_->CreateComponent <VictoryProgressUpdater> (Urho3D::LOCAL);
}

void HostActivity::DisposePlayingState ()
{
    scene_->GetChild ("units")->GetComponent <UnitsManager> ()->Remove ();
    scene_->GetComponent <ColoniesManager> ()->Remove ();
    scene_->GetComponent <TradeProcessor> ()->Remove ();
    scene_->GetComponent <PlayersPointsCalculator> ()->Remove ();
    scene_->GetComponent <VictoryProgressUpdater> ()->Remove ();
    scene_->SetUpdateEnabled (false);
}

void HostActivity::SetupFinishedState ()
{

}

void HostActivity::DisposeFinishedState ()
{
    scene_->GetComponent <MessagesHandler> ()->Remove ();
    scene_->GetChild ("players")->GetComponent <PlayersManager> ()->Remove ();
    scene_->GetChild ("map")->GetComponent <Map> ()->Remove ();
}

void HostActivity::SetupState (GameStateType state)
{
    DisposeCurrentState ();
    if (state == GAME_STATE_WAITING_FOR_PLAYERS)
    {
        SetupWaitingForPlayersState ();
    }
    else if (state == GAME_STATE_PLAYING)
    {
        SetupPlayingState ();
    }
    else if (state == GAME_STATE_FINISHED)
    {
        SetupFinishedState ();
    }
    currentState_ = state;
}

void HostActivity::DisposeCurrentState ()
{
    if (currentState_ == GAME_STATE_WAITING_FOR_PLAYERS)
    {
        DisposeWaitingForPlayersState ();
    }
    else if (currentState_ == GAME_STATE_PLAYING)
    {
        DisposePlayingState ();
    }
    else if (currentState_ == GAME_STATE_FINISHED)
    {
        DisposeFinishedState ();
    }
    currentState_ = GAME_STATE_UNITIALIZED;
}

bool HostActivity::LoadAndParseMapInfo (Urho3D::String &configurationPath, Urho3D::String &mapPath, Urho3D::String &unitsPath)
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
    return true;
}

bool HostActivity::WillIGoFromWaitingForPlayersToPlayingState ()
{
    PlayersManager *playersManager = scene_->GetChild ("players")->GetComponent <PlayersManager> ();
    // TODO: Reimplement later!
    return (playersManager->GetAllPlayers ().Size () > 0 &&
            mapFolder_ != Urho3D::String::EMPTY && mapInfoPath_ != Urho3D::String::EMPTY);
}

bool HostActivity::WillIGoFromPlayingToFinishedState ()
{
    VictoryProgressUpdater *victoryProgressUpdater = scene_->GetComponent <VictoryProgressUpdater> ();
    return victoryProgressUpdater->IsAnyoneWon ();
}

HostActivity::HostActivity (Urho3D::Context *context) : Activity (context),
    serverPort_ (13534),
    mapFolder_ (Urho3D::String::EMPTY),
    mapInfoPath_ (Urho3D::String::EMPTY),
    scene_ (new Urho3D::Scene (context))
{

}

HostActivity::~HostActivity ()
{
    DisposeCurrentState ();
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

void HostActivity::Start ()
{
    context_->GetSubsystem <Urho3D::Network> ()->StartServer (serverPort_);
    SetupState (GAME_STATE_WAITING_FOR_PLAYERS);
}

void HostActivity::Update (float timeStep)
{
    // Write replicated children count.
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
    scene_->SetVar ("ReplicatedNodesCount", replicated);

    // Go to next state if needed.
    if (currentState_ == GAME_STATE_WAITING_FOR_PLAYERS && WillIGoFromWaitingForPlayersToPlayingState ())
    {
        SetupState (GAME_STATE_PLAYING);
    }
    else if (currentState_ == GAME_STATE_PLAYING && WillIGoFromPlayingToFinishedState ())
    {
        // Send information about game end to players.
        VictoryProgressUpdater *victoryProgressUpdater = scene_->GetComponent <VictoryProgressUpdater> ();
        PlayersManager *playersManager = scene_->GetChild ("players")->GetComponent <PlayersManager> ();
        MessagesHandler *messagesHandler = scene_->GetComponent <MessagesHandler> ();

        if (victoryProgressUpdater && playersManager && messagesHandler)
        {
            Urho3D::Vector <Player *> players = playersManager->GetAllPlayers ();
            messagesHandler->SendGameEnded (victoryProgressUpdater->GetWinnerName (),
                                            victoryProgressUpdater->GetVictoryType (),
                                            victoryProgressUpdater->GetVictoryInfo (),
                                            players);
        }
        SetupState (GAME_STATE_FINISHED);
    }

    // Send current state to clients.
    PlayersManager *playersManager = scene_->GetChild ("players")->GetComponent <PlayersManager> ();
    MessagesHandler *messagesHandler = scene_->GetComponent <MessagesHandler> ();
    if (playersManager && messagesHandler)
    {
        Urho3D::Vector <Player *> players = playersManager->GetAllPlayers ();
        messagesHandler->SendGameState (currentState_, players);
    }
}

void HostActivity::Stop ()
{
    DisposeCurrentState ();
    scene_->Clear (true, true);
    context_->GetSubsystem <Urho3D::Network> ()->StopServer ();
}
}
