#include <Colonization/BuildConfiguration.hpp>
#include "HostActivity.hpp"

#include <Urho3D/Network/Network.h>
#include <Urho3D/Core/Context.h>
#include <Urho3D/IO/Log.h>
#include <Urho3D/Graphics/Octree.h>

#include <Colonization/Core/District.hpp>
#include <Colonization/Core/Map.hpp>
#include <Colonization/Core/Unit.hpp>
#include <Colonization/Core/GameConfiguration.hpp>

#include <Colonization/Backend/ColoniesManager.hpp>
#include <Colonization/Backend/MessagesHandler.hpp>
#include <Colonization/Backend/PlayersManager.hpp>
#include <Colonization/Backend/TradeProcessor.hpp>
#include <Colonization/Backend/UnitsManager.hpp>

namespace Colonization
{
void CreateTestMap (Map *map)
{
    // Map from TestMapPathFinding.
    const int mapWidth = 5;
    const int mapHeight = 5;

    for (int x = 0; x < mapWidth; x++)
        for (int y = 0; y < mapHeight; y++)
        {
            Colonization::District *district = map->CreateDistrict (
                        "(" + Urho3D::String (x) + "; " + Urho3D::String (y) + ")");

            Urho3D::PODVector <Urho3D::Vector3> polygonPoints;
            polygonPoints.Push (Urho3D::Vector3 (x, 0.0f, y ));
            polygonPoints.Push (Urho3D::Vector3 (x + 1.0f, 0.0f, y));
            polygonPoints.Push (Urho3D::Vector3 (x + 1.0f, 0.0f, y + 1.0f));
            polygonPoints.Push (Urho3D::Vector3 (x, 0.0f, y + 1.0f));
            district->SetPolygonPoints (polygonPoints);

            district->SetClimate (CLIMATE_TEMPERATE);
            district->SetFarmingSquare (Urho3D::Random (100.0f, 300.0f));
            district->SetLandAverageFertility (Urho3D::Random (0.75f, 1.25f));
            district->SetForestsSquare (400.0f - district->GetFarmingSquare ());
            district->SetForestsReproductivity (Urho3D::Random (0.25f, 1.0f));

            district->SetFarmsEvolutionPoints (2.0f);
            district->SetMinesEvolutionPoints (0.5f);
            district->SetIndustryEvolutionPoints (0.1f);
            district->SetLogisticsEvolutionPoints (0.5f);
            district->SetDefenseEvolutionPoints (0.25f);

            district->SetCoalDeposits (Urho3D::Random (0, 100) < 50);
            district->SetIronDeposits (Urho3D::Random (0, 100) < 40);
            district->SetSilverDeposits (Urho3D::Random (0, 100) < 20);
            district->SetGoldDeposits (Urho3D::Random (0, 100) < 10);

            district->SetUnitPosition (Urho3D::Vector3 (x + 0.5f, 0.0f, y + 0.65f));
            district->SetColonyPosition (Urho3D::Vector3 (x + 0.5f, 0.0f, y + 0.2f));
            district->UpdateHash (map);
        }

    // Map: (~ -- sea, = -- terrain, @ -- colony)
    //   0 1 2 3 4
    // 4 ~ ~ ~ ~ ~
    // 3 ~ = @ = ~
    // 2 ~ ~ ~ = ~
    // 1 ~ = @ = ~
    // 0 ~ = ~ ~ ~
    // Start point: (0, 2).
    // End point: (4, 4).

    // array (X * HEIGHT + Y) = (X, Y)
    map->GetDistrictByIndex (1 * mapHeight + 3)->SetIsSea (false);
    map->GetDistrictByIndex (2 * mapHeight + 3)->SetIsSea (false);
    map->GetDistrictByIndex (3 * mapHeight + 3)->SetIsSea (false);
    map->GetDistrictByIndex (3 * mapHeight + 2)->SetIsSea (false);
    map->GetDistrictByIndex (3 * mapHeight + 1)->SetIsSea (false);
    map->GetDistrictByIndex (2 * mapHeight + 1)->SetIsSea (false);
    map->GetDistrictByIndex (1 * mapHeight + 1)->SetIsSea (false);
    map->GetDistrictByIndex (1 * mapHeight + 0)->SetIsSea (false);

    map->GetDistrictByIndex (2 * mapHeight + 3)->SetColony (true);
    map->GetDistrictByIndex (2 * mapHeight + 3)->SetColonyOwnerName ("Konstant");
    map->GetDistrictByIndex (2 * mapHeight + 3)->SetMenCount (50);
    map->GetDistrictByIndex (2 * mapHeight + 3)->SetWomenCount (50);

    map->GetDistrictByIndex (1 * mapHeight + 0)->SetColony (true);
    map->GetDistrictByIndex (1 * mapHeight + 0)->SetColonyOwnerName ("AIPlayer");
    map->GetDistrictByIndex (1 * mapHeight + 0)->SetMenCount (50);
    map->GetDistrictByIndex (1 * mapHeight + 0)->SetWomenCount (50);
    map->RecalculateDistrictsNeighbors ();
}

void AddTestFleetUnits (UnitsManager *manager, Map *map, int count)
{
    for (int index = 0; index < count; index++)
    {
        Unit *unit = manager->CreateUnit ();
        unit->SetUnitType (UNIT_FLEET);
        unit->FleetUnitSetWarShipsCount (Urho3D::Random (5, 15));

        int districtIndex;
        do
            districtIndex = Urho3D::Random (0, map->GetDistrictsCount ());
        while (!map->GetDistrictByIndex (districtIndex)->IsSea ());

        unit->SetPositionHash (map->GetDistrictByIndex (districtIndex)->GetHash ());
        if (Urho3D::Random () < 0.5f)
            unit->SetOwnerPlayerName ("Konstant");
        else
            unit->SetOwnerPlayerName ("AIPlayer");
        unit->UpdateHash (manager);
    }
}

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
    GameConfiguration *configuration = scene_->CreateComponent <GameConfiguration> (Urho3D::REPLICATED);
    Map *map = scene_->CreateChild ("map", Urho3D::REPLICATED)->CreateComponent <Map> (Urho3D::REPLICATED);
    CreateTestMap (map);
    map->GetNode ()->SetVar ("PrefabXMLPath", "Objects/TestMapLocal.xml");

    UnitsManager *unitsManager = scene_->CreateChild ("units", Urho3D::REPLICATED)->CreateComponent <UnitsManager> (Urho3D::LOCAL);
    AddTestFleetUnits (unitsManager, map, Urho3D::Random (5, 15));

    Urho3D::PODVector <Urho3D::StringHash> wayToEuropeDistricts;
    // [X * HEIGHT + Y] = (X, Y)
    wayToEuropeDistricts.Push (map->GetDistrictByIndex (0 + 5 * 0)->GetHash ());
    wayToEuropeDistricts.Push (map->GetDistrictByIndex (0 + 5 * 4)->GetHash ());
    wayToEuropeDistricts.Push (map->GetDistrictByIndex (4 + 5 * 4)->GetHash ());
    wayToEuropeDistricts.Push (map->GetDistrictByIndex (4 + 5 * 0)->GetHash ());
    configuration->SetWayToEuropeDistricts (wayToEuropeDistricts);

    scene_->CreateComponent <ColoniesManager> (Urho3D::LOCAL);
    scene_->CreateComponent <TradeProcessor> (Urho3D::LOCAL);
}

void HostActivity::DisposePlayingState ()
{
    scene_->GetChild ("map")->GetComponent <Map> ()->Remove ();
    scene_->GetChild ("units")->GetComponent <UnitsManager> ()->Remove ();
    scene_->GetComponent <ColoniesManager> ()->Remove ();
    scene_->GetComponent <TradeProcessor> ()->Remove ();

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
        SetupWaitingForPlayersState ();
    else if (state == GAME_STATE_PLAYING)
        SetupPlayingState ();
    else if (state == GAME_STATE_FINISHED)
        SetupFinishedState ();
    currentState_ = state;
}

void HostActivity::DisposeCurrentState ()
{
    if (currentState_ == GAME_STATE_WAITING_FOR_PLAYERS)
        DisposeWaitingForPlayersState ();
    else if (currentState_ == GAME_STATE_PLAYING)
        DisposePlayingState ();
    else if (currentState_ == GAME_STATE_FINISHED)
        DisposeFinishedState ();
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
        if (children.At (index)->GetID () < Urho3D::FIRST_LOCAL_ID)
            replicated++;
    scene_->SetVar ("ReplicatedNodesCount", replicated);

    // Go to next state if needed.
    if (currentState_ == GAME_STATE_WAITING_FOR_PLAYERS && WillIGoFromWaitingForPlayersToPlayingState ())
        SetupState (GAME_STATE_PLAYING);
    else if (currentState_ == GAME_STATE_PLAYING && WillIGoFromPlayingToFinishedState ())
        SetupState (GAME_STATE_FINISHED);
}

void HostActivity::Stop ()
{
    DisposeCurrentState ();
    scene_->Clear (true, true);
    context_->GetSubsystem <Urho3D::Network> ()->StopServer ();
}
}
