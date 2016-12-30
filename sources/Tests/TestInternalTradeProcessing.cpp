#include "TestInternalTradeProcessing.hpp"
#include <Urho3D/Input/Input.h>
#include <Urho3D/Core/CoreEvents.h>

#include <Urho3D/IO/Log.h>
#include <Urho3D/Scene/Scene.h>
#include <Urho3D/Network/Network.h>

#include <Colonization/Core/District.hpp>
#include <Colonization/Core/Map.hpp>
#include <Colonization/Core/InternalTradeArea.hpp>

#include <Colonization/Backend/TradeProcessor.hpp>
#include <Colonization/Backend/PlayersManager.hpp>
#include <Colonization/Backend/UnitsManager.hpp>
#include <Colonization/Core/PlayerInfo.hpp>

URHO3D_DEFINE_APPLICATION_MAIN (Tests::TestInternalTradeProcessingApplication)
namespace Tests
{
TestInternalTradeProcessingApplication::TestInternalTradeProcessingApplication (Urho3D::Context *context) :
    Urho3D::Application (context)
{

}

TestInternalTradeProcessingApplication::~TestInternalTradeProcessingApplication ()
{

}

void TestInternalTradeProcessingApplication::Setup ()
{
    engineParameters_ ["FullScreen"] = false;
    engineParameters_ ["WindowResizable"] = true;
    engineParameters_ ["LogName"] = "TestInternalTradeProcessing.log";
    engineParameters_ ["WindowTitle"] = "TestInternalTradeProcessing";
}

void TestInternalTradeProcessingApplication::Start ()
{
    // Set mouse to free mode
    Urho3D::Input *input = GetSubsystem <Urho3D::Input> ();
    input->SetMouseVisible (true);
    input->SetMouseMode (Urho3D::MM_FREE);

    // Set logging level
    Urho3D::Log *log = context_->GetSubsystem <Urho3D::Log> ();
    log->SetLevel (Urho3D::LOG_DEBUG);

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

    context_->SetGlobalVar ("internalTaxes", 0.2f);
    context_->SetGlobalVar ("externalTaxes", 0.2f);

    Colonization::Map::RegisterObject (context_);
    Colonization::District::RegisterObject (context_);
    Colonization::InternalTradeArea::RegisterObject (context_);
    Colonization::PlayersManager::RegisterObject (context_);
    Colonization::TradeProcessor::RegisterObject (context_);
    Colonization::UnitsManager::RegisterObject (context_);
    Colonization::Unit::RegisterObject (context_);
    Colonization::PlayerInfo::RegisterObject (context_);
    Colonization::MessagesHandler::RegisterObject (context_);

    Urho3D::SharedPtr <Urho3D::Scene> scene (new Urho3D::Scene (context_));
    Colonization::Map *map = scene->CreateChild ("map")->CreateComponent <Colonization::Map> ();
    const int mapWidth = 4;
    const int mapHeight = 4;

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

            district->SetClimate (Colonization::CLIMATE_TEMPERATE);
            district->SetFarmingSquare (250.0f);
            district->SetLandAverageFertility (1.0f);
            district->SetForestsSquare (150.0f);
            district->SetForestsReproductivity (0.75f);

            district->SetUnitPosition (Urho3D::Vector3 (x + 0.5f, 0.0f, y + 0.65f));
            district->SetColonyPosition (Urho3D::Vector3 (x + 0.5f, 0.0f, y + 0.2f));
            district->UpdateHash (map);
        }
    map->RecalculateDistrictsNeighbors ();

    // Map: (~ -- sea, = -- terrain, numbers -- colonies with equal logistics level)
    //   0 1 2 3
    // 3 ~ ~ 5 ~
    // 2 7 ~ 5 ~
    // 1 ~ ~ 1 1
    // 0 ~ ~ 1 5
    // array (X * HEIGHT + Y) = (X, Y)

    Urho3D::PODVector <Urho3D::Vector3> coloniesCoordinatesWithLogistics;
    coloniesCoordinatesWithLogistics.Push (Urho3D::Vector3 (0, 2, 7));
    coloniesCoordinatesWithLogistics.Push (Urho3D::Vector3 (2, 3, 5));
    coloniesCoordinatesWithLogistics.Push (Urho3D::Vector3 (2, 2, 5));
    coloniesCoordinatesWithLogistics.Push (Urho3D::Vector3 (2, 1, 1));
    coloniesCoordinatesWithLogistics.Push (Urho3D::Vector3 (2, 0, 1));
    coloniesCoordinatesWithLogistics.Push (Urho3D::Vector3 (3, 1, 1));
    coloniesCoordinatesWithLogistics.Push (Urho3D::Vector3 (3, 0, 5));

    for (int index = 0; index < coloniesCoordinatesWithLogistics.Size (); index++)
    {
        Urho3D::Vector3 coordinate = coloniesCoordinatesWithLogistics.At (index);
        Colonization::District *district = map->GetDistrictByIndex (coordinate.x_ * mapHeight + coordinate.y_);
        district->SetColony (true);
        district->SetColonyOwnerName ("PlayerX");
        district->SetFarmsEvolutionPoints (4.0f);
        district->SetMinesEvolutionPoints (2.0f);
        district->SetIndustryEvolutionPoints (1.5f);
        district->SetLogisticsEvolutionPoints (coordinate.z_);
        district->SetDefenseEvolutionPoints (5.0f);
    }

    Colonization::MessagesHandler *messagesHandler = scene->CreateComponent <Colonization::MessagesHandler> ();
    Colonization::PlayersManager *playersManager =
            scene->CreateChild ("players")->CreateComponent <Colonization::PlayersManager> ();

    Urho3D::Network *network = context_->GetSubsystem <Urho3D::Network> ();
    network->StartServer (3793);
    Urho3D::SharedPtr <Urho3D::Scene> sceneForReplication (new Urho3D::Scene (context_));

    Urho3D::VariantMap identity;
    identity ["Name"] = "PlayerX";
    network->Connect ("localhost", 3793, sceneForReplication, identity);
    Colonization::Player *playerX = playersManager->GetPlayer ("PlayerX");

    Colonization::TradeProcessor *tradeProcessor =
            scene->CreateComponent <Colonization::TradeProcessor> ();

    Colonization::UnitsManager *unitsManager =
            scene->CreateChild ("units", Urho3D::REPLICATED)->CreateComponent <Colonization::UnitsManager> ();

    Urho3D::VariantMap eventData;
    eventData [Urho3D::Update::P_TIMESTEP] = 0.1f;
    tradeProcessor->Update (Urho3D::E_UPDATE, eventData);

    Urho3D::Log::Write (Urho3D::LOG_INFO, "Trade areas count: " + Urho3D::String (tradeProcessor->GetTradeAreasCount ()));
    for (int index = 0; index < tradeProcessor->GetTradeAreasCount (); index++)
    {
        Colonization::InternalTradeArea *tradeArea = tradeProcessor->GetTradeAreaByIndex (index);
        Urho3D::Log::Write (Urho3D::LOG_INFO, "Trade area: " + Urho3D::String (index) + ", districts:");

        for (int districtIndex = 0; districtIndex < tradeArea->GetDistrictsHashesCount (); districtIndex++)
            Urho3D::Log::Write (Urho3D::LOG_INFO, map->GetDistrictByHash (
                                    tradeArea->GetDistrictHashByIndex (districtIndex))->GetName ());

        Urho3D::Log::Write (Urho3D::LOG_INFO, "");
    }


    Urho3D::Log::Write (Urho3D::LOG_INFO, "PlayerX's gold: " + Urho3D::String (playerX->GetGold ()));
    Urho3D::Log::Write (Urho3D::LOG_INFO, "Sended traders: " + Urho3D::String (unitsManager->GetUnitsCount ()));

    if (tradeProcessor->GetTradeAreasCount () != 2)
        ErrorExit ("Expected 2 trade areas!");
    else if (playerX->GetGold () <= 5.0f)
        ErrorExit ("Expected that PlayerX has more than 5.0 gold!");
    else if (unitsManager->GetUnitsCount () != 7)
        ErrorExit ("Expected that PlayerX sended 7 traders!");
    else
        engine_->Exit ();
}

void TestInternalTradeProcessingApplication::Stop ()
{

}
}

