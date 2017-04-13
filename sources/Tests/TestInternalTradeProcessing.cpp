#include "TestInternalTradeProcessing.hpp"
#include <Urho3D/Input/Input.h>
#include <Urho3D/Core/CoreEvents.h>
#include <Urho3D/Scene/SceneEvents.h>
#include <Urho3D/IO/Log.h>
#include <Urho3D/Network/Network.h>

#include <Colonization/Core/District/District.hpp>
#include <Colonization/Core/Map.hpp>
#include <Colonization/Core/InternalTradeArea/InternalTradeArea.hpp>
#include <Colonization/Core/GameConfiguration.hpp>

#include <Colonization/Backend/TradeProcessor.hpp>
#include <Colonization/Backend/PlayersManager.hpp>
#include <Colonization/Backend/UnitsManager.hpp>
#include <Colonization/Utils/Network/NetworkUpdateSmoother.hpp>
#include <Colonization/Utils/Hubs/RegisterAllObjects.hpp>

URHO3D_DEFINE_APPLICATION_MAIN (Tests::TestInternalTradeProcessingApplication)
namespace Tests
{
TestInternalTradeProcessingApplication::TestInternalTradeProcessingApplication (Urho3D::Context *context) :
    Urho3D::Application (context),
    scene_ (),
    sceneForReplication_ ()
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

    Colonization::RegisterAllObjects (context_);
    scene_ = Urho3D::SharedPtr <Urho3D::Scene> (new Urho3D::Scene (context_));
    Colonization::GameConfiguration *configuration = scene_->CreateComponent <Colonization::GameConfiguration> ();
    Colonization::Map *map = scene_->CreateChild ("map")->CreateComponent <Colonization::Map> ();
    const int mapWidth = 4;
    const int mapHeight = 4;

    for (int x = 0; x < mapWidth; x++)
    {
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

            district->SetUnitPosition (Urho3D::Vector3 (x + 0.5f, 0.0f, y + 0.65f));
            district->SetColonyPosition (Urho3D::Vector3 (x + 0.5f, 0.0f, y + 0.2f));
        }
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
    coloniesCoordinatesWithLogistics.Push (Urho3D::Vector3 (0, 2, 8));
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

    Urho3D::PODVector <Urho3D::StringHash> wayToEuropeDistricts;
    wayToEuropeDistricts.Push (map->GetDistrictByIndex (0)->GetHash ());
    configuration->SetWayToEuropeDistricts (wayToEuropeDistricts);

    scene_->CreateComponent <Colonization::MessagesHandler> ();
    scene_->CreateChild ("players")->CreateComponent <Colonization::PlayersManager> ();
    scene_->CreateComponent <Colonization::NetworkUpdateSmoother> ();

    Urho3D::Network *network = context_->GetSubsystem <Urho3D::Network> ();
    network->StartServer (3793);
    sceneForReplication_ = Urho3D::SharedPtr <Urho3D::Scene> (new Urho3D::Scene (context_));

    Urho3D::VariantMap identity;
    identity ["Name"] = "PlayerX";
    network->Connect ("localhost", 3793, sceneForReplication_, identity);

    SubscribeToEvent (Urho3D::E_UPDATE, URHO3D_HANDLER (TestInternalTradeProcessingApplication, Update));
    timeUntilAutoExit_ = 2.5f;
}

void TestInternalTradeProcessingApplication::Update (Urho3D::StringHash eventType, Urho3D::VariantMap &eventData)
{
    float timeStep = eventData [Urho3D::Update::P_TIMESTEP].GetFloat ();
    timeUntilAutoExit_ -= timeStep;
    if (timeUntilAutoExit_ <= 0.0f)
    {
        ErrorExit ("Time elapsed!");
    }

    Colonization::PlayersManager *playersManager = scene_->GetChild ("players")->GetComponent <Colonization::PlayersManager> ();
    if (playersManager->GetPlayerByNameHash ("PlayerX"))
    {
        Colonization::Map *map = scene_->GetChild ("map")->GetComponent <Colonization::Map> ();
        Colonization::Player *playerX = playersManager->GetPlayerByNameHash ("PlayerX");

        Colonization::TradeProcessor *tradeProcessor =
                scene_->CreateComponent <Colonization::TradeProcessor> ();

        Colonization::UnitsManager *unitsManager =
                scene_->CreateChild ("units", Urho3D::REPLICATED)->CreateComponent <Colonization::UnitsManager> ();

        Urho3D::VariantMap eventData;
        eventData [Urho3D::SceneUpdate::P_TIMESTEP] = 0.1f;
        tradeProcessor->Update (Urho3D::E_SCENEUPDATE, eventData);

        Urho3D::Log::Write (Urho3D::LOG_INFO, "Trade areas count: " + Urho3D::String (tradeProcessor->GetTradeAreasCount ()));
        for (int index = 0; index < tradeProcessor->GetTradeAreasCount (); index++)
        {
            Colonization::InternalTradeArea *tradeArea = tradeProcessor->GetTradeAreaByIndex (index);
            Urho3D::Log::Write (Urho3D::LOG_INFO, "Trade area: " + Urho3D::String (index) + ", districts:");

            for (int districtIndex = 0; districtIndex < tradeArea->GetDistrictsHashesCount (); districtIndex++)
            {
                Urho3D::Log::Write (Urho3D::LOG_INFO, map->GetDistrictByHash (
                                        tradeArea->GetDistrictHashByIndex (districtIndex))->GetName ());
            }

            Urho3D::Log::Write (Urho3D::LOG_INFO, "");
        }

        Urho3D::Log::Write (Urho3D::LOG_INFO, "PlayerX's gold: " + Urho3D::String (playerX->GetGold ()));
        Urho3D::Log::Write (Urho3D::LOG_INFO, "Sended traders: " + Urho3D::String (unitsManager->GetUnitsCount ()));

        if (tradeProcessor->GetTradeAreasCount () != 2)
        {
            ErrorExit ("Expected 2 trade areas!");
        }
        else if (playerX->GetGold () <= 5.0f)
        {
            ErrorExit ("Expected that PlayerX has more than 5.0 gold!");
        }
        else if (unitsManager->GetUnitsCount () != 7)
        {
            ErrorExit ("Expected that PlayerX send 7 traders!");
        }
        else
        {
            engine_->Exit ();
        }
    }
}

void TestInternalTradeProcessingApplication::Stop ()
{
    scene_->Clear (true, true);
    Urho3D::Network *network = context_->GetSubsystem <Urho3D::Network> ();
    network->StopServer ();
    sceneForReplication_->Clear (true, true);
}
}

