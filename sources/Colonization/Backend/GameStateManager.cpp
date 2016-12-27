#include <Colonization/BuildConfiguration.hpp>
#include "GameStateManager.hpp"
#include <Urho3D/Core/Context.h>

#include <Colonization/Backend/MessagesHandler.hpp>
#include <Colonization/Backend/PlayersManager.hpp>
#include <Colonization/Backend/SceneManager.hpp>
#include <Colonization/Backend/UnitsManager.hpp>
#include <Colonization/Backend/ColoniesManager.hpp>
#include <Colonization/Backend/TradeProcessor.hpp>
#include <Colonization/Core/Map.hpp>

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
            Colonization::District *district = new District (map->GetContext ());
            district->name_ = "(" + Urho3D::String (x) + "; " + Urho3D::String (y) + ")";

            district->polygonPoints_.Push (Urho3D::Vector3 (x, 0.0f, y ));
            district->polygonPoints_.Push (Urho3D::Vector3 (x + 1.0f, 0.0f, y));
            district->polygonPoints_.Push (Urho3D::Vector3 (x + 1.0f, 0.0f, y + 1.0f));
            district->polygonPoints_.Push (Urho3D::Vector3 (x, 0.0f, y + 1.0f));

            district->climate_ = CLIMATE_TEMPERATE;
            district->farmingSquare_ = Urho3D::Random (50.0f, 300.0f);
            district->landAverageFertility_ = Urho3D::Random (0.75f, 1.25f);
            district->forestsSquare_ = 300.0f - district->farmingSquare_;
            district->forestsReproductivity_ = Urho3D::Random (0.25f, 1.0f);

            district->farmsEvolutionPoints_ = 1.0f;
            district->minesEvolutionPoints_ = 1.0f;
            district->industryEvolutionPoints_ = 1.0f;
            district->logisticsEvolutionPoints_ = 1.0f;
            district->defenseEvolutionPoints_ = 1.0f;

            district->hasCoalDeposits_ = Urho3D::Random (0, 100) < 50;
            district->hasIronDeposits_ = Urho3D::Random (0, 100) < 40;
            district->hasSilverDeposits_ = Urho3D::Random (0, 100) < 20;
            district->hasGoldDeposits_ = Urho3D::Random (0, 100) < 10;

            district->unitPosition_ = Urho3D::Vector3 (x + 0.5f, 0.0f, y + 0.65f);
            district->colonyPosition_ = Urho3D::Vector3 (x + 0.5f, 0.0f, y + 0.2f);
            map->CreateDistrict (district);
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
    map->GetDistrictByIndex (1 * mapHeight + 3)->isSea_ = false;
    map->GetDistrictByIndex (2 * mapHeight + 3)->isSea_ = false;
    map->GetDistrictByIndex (3 * mapHeight + 3)->isSea_ = false;
    map->GetDistrictByIndex (3 * mapHeight + 2)->isSea_ = false;
    map->GetDistrictByIndex (3 * mapHeight + 1)->isSea_ = false;
    map->GetDistrictByIndex (2 * mapHeight + 1)->isSea_ = false;
    map->GetDistrictByIndex (1 * mapHeight + 1)->isSea_ = false;
    map->GetDistrictByIndex (1 * mapHeight + 0)->isSea_ = false;

    map->GetDistrictByIndex (2 * mapHeight + 3)->hasColony_ = true;
    map->GetDistrictByIndex (2 * mapHeight + 3)->colonyOwnerName_ = "Konstant";
    map->GetDistrictByIndex (2 * mapHeight + 3)->menCount_ = 50;
    map->GetDistrictByIndex (2 * mapHeight + 3)->womenCount_ = 50;

    map->GetDistrictByIndex (1 * mapHeight + 0)->hasColony_ = true;
    map->GetDistrictByIndex (1 * mapHeight + 0)->colonyOwnerName_ = "AIPlayer";
    map->GetDistrictByIndex (1 * mapHeight + 0)->menCount_ = 50;
    map->GetDistrictByIndex (1 * mapHeight + 0)->womenCount_ = 50;
    map->RecalculateDistrictsNeighbors ();
}

void AddTestFleetUnits (UnitsContainer *container, Map *map, int count)
{
    for (int index = 0; index < count; index++)
    {
        FleetUnit *unit = new FleetUnit (map->GetContext ());
        unit->warShipsCount_ = Urho3D::Random (1, 10);

        int districtIndex;
        do
            districtIndex = Urho3D::Random (0, map->GetDistrictsCount ());
        while (!map->GetDistrictByIndex (districtIndex)->isSea_);

        unit->position_ = map->GetDistrictByIndex (districtIndex);
        if (Urho3D::Random () < 0.5f)
            unit->ownerPlayer_ = "Konstant";
        else
            unit->ownerPlayer_ = "AIPlayer";
        container->AddUnit (unit);
        unit->UpdateHash (container);
    }
}

void GameStateManager::SetupWaitingForPlayersState ()
{
    // TODO: To be continued...
    MessagesHandler *messagesHandler = new MessagesHandler (context_);
    PlayersManager *playersManager = new PlayersManager (context_);

    SceneManager *sceneManager = new SceneManager (context_);
    sceneManager->PrepareForGameState (GAME_STATE_WAITING_FOR_PLAYERS);

    context_->SetGlobalVar ("MessagesHandler", Urho3D::Variant (messagesHandler));
    context_->SetGlobalVar ("PlayersManager", Urho3D::Variant (playersManager));
    context_->SetGlobalVar ("SceneManager", Urho3D::Variant (sceneManager));
}

void GameStateManager::DisposeWaitingForPlayersState ()
{
    // TODO: Implement later.
}

void GameStateManager::SetupPlayingState ()
{
    // TODO: To be continued...
    context_->SetGlobalVar ("sailSpeed", 0.3f);
    context_->SetGlobalVar ("marchSpeed", 0.1f);
    context_->SetGlobalVar ("embarkationSpeed", 0.15f);
    context_->SetGlobalVar ("disembarkationSpeed", 0.2f);

    context_->SetGlobalVar ("oneColonistFarmsProductionConsumption", 0.01f);
    context_->SetGlobalVar ("farmsProductionMinesConsumption", 0.005f);
    context_->SetGlobalVar ("farmsProductionIndustryConsumption", 0.3f);

    context_->SetGlobalVar ("oneColonistMinesProductionConsumption", 0.002f);
    context_->SetGlobalVar ("minesProductionFarmsConsumption", 0.2f);
    context_->SetGlobalVar ("minesProductionIndustryConsumption", 0.9f);

    context_->SetGlobalVar ("oneColonistIndustryProductionConsumption", 0.003f);
    context_->SetGlobalVar ("industryProductionFarmsConsumption", 0.15f);
    context_->SetGlobalVar ("industryProductionMinesConsumption", 0.25f);

    context_->SetGlobalVar ("farmsProductionInternalCost", 1.0f);
    context_->SetGlobalVar ("minesProductionInternalCost", 1.5f);
    context_->SetGlobalVar ("industryProductionInternalCost", 5.0f);

    context_->SetGlobalVar ("farmsProductionExternalCost", 0.5f);
    context_->SetGlobalVar ("minesProductionExternalCost", 1.0f);
    context_->SetGlobalVar ("industryProductionExternalCost", 3.0f);

    context_->SetGlobalVar ("internalTaxes", 0.2f);
    context_->SetGlobalVar ("externalTaxes", 0.2f);

    Map *map = new Map (context_);
    CreateTestMap (map);
    context_->SetGlobalVar ("Map", Urho3D::Variant (map));

    UnitsManager *unitsManager = new UnitsManager (context_);
    AddTestFleetUnits (unitsManager->GetUnitsContainer (), map, Urho3D::Random (5, 15));
    context_->SetGlobalVar ("UnitsManager", Urho3D::Variant (unitsManager));

    ColoniesManager *coloniesManager = new ColoniesManager (context_);
    context_->SetGlobalVar ("ColoniesManager", Urho3D::Variant (coloniesManager));

    TradeProcessor *tradeProcessor = new TradeProcessor (context_);
    context_->SetGlobalVar ("TradeProcessor", Urho3D::Variant (tradeProcessor));

    SceneManager *sceneManager = (SceneManager *) context_->GetGlobalVar ("SceneManager").GetPtr ();
    assert (sceneManager);
    sceneManager->PrepareForGameState (GAME_STATE_PLAYING);
}

void GameStateManager::DisposePlayingState ()
{
    // TODO: Temporary. Reimplement later.
    delete ( (Map *) context_->GetGlobalVar ("Map").GetPtr ());
    delete ( (UnitsManager *) context_->GetGlobalVar ("UnitsManager").GetPtr ());
    delete ( (ColoniesManager *) context_->GetGlobalVar ("ColoniesManager").GetPtr ());
    delete ( (TradeProcessor *) context_->GetGlobalVar ("TradeProcessor").GetPtr ());
    SetupState (GAME_STATE_FINISHED);
    DisposeCurrentState ();
}

void GameStateManager::SetupFinishedState ()
{
    // TODO: Implement later.
}

void GameStateManager::DisposeFinishedState ()
{
    // TODO: To be continued...
    delete ( (MessagesHandler *) context_->GetGlobalVar ("MessagesHandler").GetPtr ());
    delete ( (PlayersManager *) context_->GetGlobalVar ("PlayersManager").GetPtr ());
    delete ( (SceneManager *) context_->GetGlobalVar ("SceneManager").GetPtr ());
}

void GameStateManager::SetupState (GameStateType state)
{
    if (state == GAME_STATE_WAITING_FOR_PLAYERS)
        SetupWaitingForPlayersState ();
    else if (state == GAME_STATE_PLAYING)
        SetupPlayingState ();
    else if (state == GAME_STATE_FINISHED)
        SetupFinishedState ();
    currentState_ = state;
}

void GameStateManager::DisposeCurrentState ()
{
    if (currentState_ == GAME_STATE_WAITING_FOR_PLAYERS)
        DisposeWaitingForPlayersState ();
    else if (currentState_ == GAME_STATE_PLAYING)
        DisposePlayingState ();
    else if (currentState_ == GAME_STATE_FINISHED)
        DisposeFinishedState ();
}

bool GameStateManager::WillIGoFromWaitingForPlayersToPlayingState ()
{
    PlayersManager *playersManager = (PlayersManager *) context_->GetGlobalVar ("PlayersManager").GetPtr ();
    // TODO: Reimplement later!
    return (playersManager->GetAllPlayers ().Size () > 0);
}

bool GameStateManager::WillIGoFromPlayingToFinishedState ()
{
    // TODO: Implement later.
    return false;
}

GameStateManager::GameStateManager (Urho3D::Context *context) : Urho3D::Object (context),
    currentState_ (GAME_STATE_UNITIALIZED)
{
    SetupState (GAME_STATE_WAITING_FOR_PLAYERS);
}

GameStateManager::~GameStateManager ()
{
    DisposeCurrentState ();
}

void GameStateManager::Update (float timeStep)
{
    if (currentState_ == GAME_STATE_WAITING_FOR_PLAYERS && WillIGoFromWaitingForPlayersToPlayingState ())
        SetupState (GAME_STATE_PLAYING);
    else if (currentState_ == GAME_STATE_PLAYING && WillIGoFromPlayingToFinishedState ())
        SetupState (GAME_STATE_FINISHED);
}

GameStateType GameStateManager::GetCurrentState ()
{
    return currentState_;
}
}
