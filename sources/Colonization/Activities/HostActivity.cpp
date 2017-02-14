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
    // Load game configuration.
    Urho3D::Node *configurationNode = scene_->CreateChild ("configuration", Urho3D::LOCAL);
    configurationNode->LoadXML (resourceCache->GetResource <Urho3D::XMLFile> ("Maps/BasicLevel/configuration.xml")->GetRoot ());
    GameConfiguration *configuration = configurationNode->GetComponent <GameConfiguration> ();
    scene_->CloneComponent (configuration, Urho3D::REPLICATED);
    configuration->Remove ();

    // Load game map.
    Urho3D::Node *mapNode = scene_->CreateChild ("map", Urho3D::REPLICATED);
    mapNode->LoadXML (resourceCache->GetResource <Urho3D::XMLFile> ("Maps/BasicLevel/map.xml")->GetRoot ());
    Map *map = mapNode->GetComponent <Map> ();
    map->UpdateDistrictsList ();

    // Load units and create units manager.
    Urho3D::Node *unitsNode = scene_->CreateChild ("units", Urho3D::REPLICATED);
    unitsNode->LoadXML (resourceCache->GetResource <Urho3D::XMLFile> ("Maps/BasicLevel/units.xml")->GetRoot ());
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
}

void HostActivity::DisposePlayingState ()
{
    scene_->GetChild ("map")->GetComponent <Map> ()->Remove ();
    scene_->GetChild ("units")->GetComponent <UnitsManager> ()->Remove ();
    scene_->GetComponent <ColoniesManager> ()->Remove ();
    scene_->GetComponent <TradeProcessor> ()->Remove ();
    scene_->GetComponent <PlayersPointsCalculator> ()->Remove ();

    // TODO: This is temporary!
    currentState_ = GAME_STATE_UNITIALIZED;
    SetupState (GAME_STATE_FINISHED);
}

void HostActivity::SetupFinishedState ()
{

}

void HostActivity::DisposeFinishedState ()
{
    scene_->GetComponent <MessagesHandler> ()->Remove ();
    scene_->GetChild ("players")->GetComponent <PlayersManager> ()->Remove ();
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

bool HostActivity::WillIGoFromWaitingForPlayersToPlayingState ()
{
    PlayersManager *playersManager = scene_->GetChild ("players")->GetComponent <PlayersManager> ();
    // TODO: Reimplement later!
    return (playersManager->GetAllPlayers ().Size () > 0);
}

bool HostActivity::WillIGoFromPlayingToFinishedState ()
{
    // TODO: Implement later.
    return false;
}

HostActivity::HostActivity (Urho3D::Context *context) : Activity (context),
    serverPort_ (13534),
    scene_ (new Urho3D::Scene (context))
{

}

HostActivity::~HostActivity ()
{
    DisposeCurrentState ();
}

unsigned short HostActivity::GetServerPort()
{
    return serverPort_;
}

void HostActivity::SetServerPort (unsigned short serverPort)
{
    serverPort_ = serverPort;
}

Urho3D::Scene *HostActivity::GetScene ()
{
    return scene_;
}

void HostActivity::Start ()
{
    context_->GetSubsystem <Urho3D::Network> ()->StartServer (serverPort_);
    SetupState (GAME_STATE_WAITING_FOR_PLAYERS);
    SetupState (GAME_STATE_PLAYING);
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
        SetupState (GAME_STATE_FINISHED);
    }
}

void HostActivity::Stop ()
{
    DisposeCurrentState ();
    scene_->Clear (true, true);
    context_->GetSubsystem <Urho3D::Network> ()->StopServer ();
}
}
