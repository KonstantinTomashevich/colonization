#include "TestApplication.hpp"
#include <Tests/TestsUtils/EmptyInternalPlayer.hpp>
#include <Urho3D/Input/Input.h>
#include <Urho3D/Core/CoreEvents.h>
#include <Urho3D/Scene/SceneEvents.h>
#include <Urho3D/IO/Log.h>
#include <Urho3D/Scene/Scene.h>

#include <Colonization/Backend/Diplomacy/DiplomacyProcessor.hpp>
#include <Colonization/Backend/Diplomacy/DiplomacyWarRequest.hpp>
#include <Colonization/Backend/Diplomacy/DiplomacyPeaceRequest.hpp>
#include <Colonization/Backend/Diplomacy/DiplomacyOfferType.hpp>

#include <Colonization/Core/District/District.hpp>
#include <Colonization/Core/Map.hpp>
#include <Colonization/Core/Battle/Battle.hpp>
#include <Colonization/Core/GameConfiguration.hpp>

#include <Colonization/Backend/UnitsManager.hpp>
#include <Colonization/Backend/BattlesProcessor.hpp>
#include <Colonization/Backend/MessagesHandler/NetworkMessageType.hpp>
#include <Colonization/Backend/PlayersManager.hpp>

#include <Colonization/Utils/Network/NetworkUpdateSmoother.hpp>
#include <Colonization/Utils/Hubs/RegisterAllObjects.hpp>

URHO3D_DEFINE_APPLICATION_MAIN (TestApplication)
TestApplication::TestApplication (Urho3D::Context *context) :
    Urho3D::Application (context)
{

}

TestApplication::~TestApplication ()
{

}

void TestApplication::Setup ()
{
    engineParameters_ ["FullScreen"] = false;
    engineParameters_ ["WindowResizable"] = true;
    engineParameters_ ["LogName"] = "Test.log";
    engineParameters_ ["WindowTitle"] = "Test";
    engineParameters_ ["ResourcePrefixPaths"] = "..;.";
}

void TestApplication::Start ()
{
    // Set mouse to free mode
    Urho3D::Input *input = GetSubsystem <Urho3D::Input> ();
    input->SetMouseVisible (true);
    input->SetMouseMode (Urho3D::MM_FREE);

    // Set logging level
    Urho3D::Log *log = context_->GetSubsystem <Urho3D::Log> ();
    log->SetLevel (Urho3D::LOG_DEBUG);

    Urho3D::SetRandomSeed (Urho3D::Time::GetTimeSinceEpoch ());
    Colonization::RegisterAllObjects (context_);
    Urho3D::SharedPtr <Urho3D::Scene> scene (new Urho3D::Scene (context_));
    Colonization::GameConfiguration *configuration = scene->CreateComponent <Colonization::GameConfiguration> ();
    configuration->ResetToDefault ();

    Colonization::Map *map = scene->CreateChild ("map")->CreateComponent <Colonization::Map> ();
    const int mapWidth = 3;
    const int mapHeight = 3;

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

    scene->CreateComponent <Colonization::MessagesHandler> ();
    scene->CreateComponent <Colonization::NetworkUpdateSmoother> ();
    Colonization::PlayersManager *playersManager  = scene->CreateChild ("players")->CreateComponent <Colonization::PlayersManager> ();
    Colonization::DiplomacyProcessor *diplomacyProcessor = scene->CreateChild ("diplomacy")->CreateComponent <Colonization::DiplomacyProcessor> ();

    EmptyInternalPlayer *firstPlayer = new EmptyInternalPlayer (context_, "PlayerX", Urho3D::Color::RED, scene);
    EmptyInternalPlayer *secondPlayer = new EmptyInternalPlayer (context_, "PlayerY", Urho3D::Color::BLUE, scene);
    playersManager->AddInternalPlayer (firstPlayer);
    playersManager->AddInternalPlayer (secondPlayer);

    Urho3D::VariantMap updateEventData;
    const float timestep = 1/30.0f;
    updateEventData [Urho3D::SceneUpdate::P_TIMESTEP] = Urho3D::Variant (timestep);

    {
        Urho3D::SharedPtr <Colonization::DiplomacyWarRequest>
                warRequest (new Colonization::DiplomacyWarRequest (context_));
        warRequest->SetAttacker (Urho3D::StringHash (firstPlayer->GetName ()));
        warRequest->SetDefender (Urho3D::StringHash (secondPlayer->GetName ()));
        diplomacyProcessor->AddDiplomacyRequest (warRequest);

        diplomacyProcessor->Update (Urho3D::E_SCENEUPDATE, updateEventData);
        playersManager->Update (Urho3D::E_SCENEUPDATE, updateEventData);
    }

    Colonization::UnitsManager *unitsManager = scene->CreateChild ("units")->CreateComponent <Colonization::UnitsManager> ();
    Colonization::BattlesProcessor *battlesProcessor = scene->CreateChild ("battles")->CreateComponent <Colonization::BattlesProcessor> ();

    // array (X * HEIGHT + Y) = (X, Y)
    Colonization::FleetUnit *unit1 = static_cast <Colonization::FleetUnit *> (
                unitsManager->CreateUnit (Colonization::UNIT_FLEET, "PlayerX",
                                          map->GetDistrictByIndex (1 * mapHeight + 1)->GetHash ()));

    Colonization::FleetUnit *unit2 = static_cast <Colonization::FleetUnit *> (
                unitsManager->CreateUnit (Colonization::UNIT_FLEET, "PlayerX",
                                          map->GetDistrictByIndex (1 * mapHeight + 1)->GetHash ()));

    Colonization::TradersUnit *unit3 = static_cast <Colonization::TradersUnit *> (
                unitsManager->CreateUnit (Colonization::UNIT_TRADERS, "PlayerY",
                                          map->GetDistrictByIndex (1 * mapHeight + 1)->GetHash ()));

    Colonization::TradersUnit *unit4 = static_cast <Colonization::TradersUnit *> (
                unitsManager->CreateUnit (Colonization::UNIT_TRADERS, "PlayerY",
                                          map->GetDistrictByIndex (1 * mapHeight + 1)->GetHash ()));

    const int warShipsCount = 10;
    unit1->SetWarShipsCount (warShipsCount / 2);
    unit2->SetWarShipsCount (warShipsCount / 2);
    unit3->SetTradeGoodsCost (2000.0f);
    unit4->SetTradeGoodsCost (2000.0f);

    const int framesCount = 120;
    for (int frame = 0; frame < framesCount; frame++)
    {
        playersManager->Update (Urho3D::E_SCENEUPDATE, updateEventData);
        battlesProcessor->Update (Urho3D::E_SCENEUPDATE, updateEventData);
        unitsManager->Update (Urho3D::E_SCENEUPDATE, updateEventData);
    }

    const float expectedMinimumLoot = (framesCount * timestep) *
            (warShipsCount * configuration->GetWarShipBasicNavalAttackForce () *
             configuration->GetLootingCoefficient () / configuration->GetTraderBasicHealth ()) * 0.75f;

    Urho3D::Log::Write (Urho3D::LOG_INFO, "War ships count (unit1): " + Urho3D::String (unit1->GetWarShipsCount ()));
    Urho3D::PODVector <float> warShipsHealth = unit1->GetWarShipsHealthPoints ();
    for (int index = 0; index < unit1->GetWarShipsCount (); index++)
    {
        Urho3D::Log::Write (Urho3D::LOG_INFO, "    " + Urho3D::String (warShipsHealth.At (index)) + "%");
    }

    Urho3D::Log::Write (Urho3D::LOG_INFO, "War ships count (unit2): " + Urho3D::String (unit2->GetWarShipsCount ()));
    warShipsHealth = unit2->GetWarShipsHealthPoints ();
    for (int index = 0; index < unit2->GetWarShipsCount (); index++)
    {
        Urho3D::Log::Write (Urho3D::LOG_INFO, "    " + Urho3D::String (warShipsHealth.At (index)) + "%");
    }

    Urho3D::Log::Write (Urho3D::LOG_INFO, "Traders trade goods cost (unit3): " + Urho3D::String (unit3->GetTradeGoodsCost ()));
    Urho3D::Log::Write (Urho3D::LOG_INFO, "Traders trade goods cost (unit4): " + Urho3D::String (unit4->GetTradeGoodsCost ()));
    Urho3D::Log::Write (Urho3D::LOG_INFO, "First player gold: " + Urho3D::String (firstPlayer->GetGold ()));

    if (firstPlayer->GetGold () < expectedMinimumLoot)
    {
        ErrorExit ("Expected that first player gold will be " + Urho3D::String (expectedMinimumLoot) +
                   ", but it is " + Urho3D::String (firstPlayer->GetGold ()) + "!");
        return;
    }
    else
    {
        engine_->Exit ();
    }
}

void TestApplication::Stop ()
{

}
