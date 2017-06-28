#include "TestBattleCreationAndJoining.hpp"
#include "EmptyInternalPlayer.hpp"
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
#include <Colonization/Core/Battle.hpp>
#include <Colonization/Core/GameConfiguration.hpp>

#include <Colonization/Backend/UnitsManager.hpp>
#include <Colonization/Backend/BattlesProcessor.hpp>
#include <Colonization/Backend/MessagesHandler/NetworkMessageType.hpp>
#include <Colonization/Backend/PlayersManager.hpp>

#include <Colonization/Utils/Network/NetworkUpdateSmoother.hpp>
#include <Colonization/Utils/Hubs/RegisterAllObjects.hpp>

URHO3D_DEFINE_APPLICATION_MAIN (Tests::TestBattleCreationAndJoiningApplication)
namespace Tests
{
TestBattleCreationAndJoiningApplication::TestBattleCreationAndJoiningApplication (Urho3D::Context *context) :
    Urho3D::Application (context)
{

}

TestBattleCreationAndJoiningApplication::~TestBattleCreationAndJoiningApplication ()
{

}

void TestBattleCreationAndJoiningApplication::Setup ()
{
    engineParameters_ ["FullScreen"] = false;
    engineParameters_ ["WindowResizable"] = true;
    engineParameters_ ["LogName"] = "TestBattleCreationAndJoining.log";
    engineParameters_ ["WindowTitle"] = "TestBattleCreationAndJoining";
    engineParameters_ ["ResourcePrefixPaths"] = "..;.";
}

void TestBattleCreationAndJoiningApplication::Start ()
{
    // Set mouse to free mode
    Urho3D::Input *input = GetSubsystem <Urho3D::Input> ();
    input->SetMouseVisible (true);
    input->SetMouseMode (Urho3D::MM_FREE);

    // Set logging level
    Urho3D::Log *log = context_->GetSubsystem <Urho3D::Log> ();
    log->SetLevel (Urho3D::LOG_DEBUG);

    Colonization::RegisterAllObjects (context_);
    Urho3D::SharedPtr <Urho3D::Scene> scene (new Urho3D::Scene (context_));
    scene->CreateComponent <Colonization::GameConfiguration> ()->ResetToDefault ();

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
    updateEventData [Urho3D::SceneUpdate::P_TIMESTEP] = Urho3D::Variant (10.0f);
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
    Colonization::FleetUnit *unit1 = (Colonization::FleetUnit *)
            unitsManager->CreateUnit (Colonization::UNIT_FLEET, "PlayerX",
                                      map->GetDistrictByIndex (1 * mapHeight + 1)->GetHash ());

    Colonization::FleetUnit *unit2 = (Colonization::FleetUnit *)
            unitsManager->CreateUnit (Colonization::UNIT_FLEET, "PlayerY",
                                      map->GetDistrictByIndex (1 * mapHeight + 1)->GetHash ());

    unit1->SetWarShipsCount (150);
    unit2->SetWarShipsCount (100);

    diplomacyProcessor->Update (Urho3D::E_SCENEUPDATE, updateEventData);
    playersManager->Update (Urho3D::E_SCENEUPDATE, updateEventData);
    battlesProcessor->Update (Urho3D::E_SCENEUPDATE, updateEventData);
    unitsManager->Update (Urho3D::E_SCENEUPDATE, updateEventData);

    if (battlesProcessor->GetBattlesCount () != 1)
    {
        ErrorExit ("Expected 1 battle, but got " + Urho3D::String (battlesProcessor->GetBattlesCount ()) + "!");
        return;
    }
    else if (!unit1->GetIsInBattle ())
    {
        ErrorExit ("Expected that unit1 will be in battle!");
        return;
    }
    else if (!unit2->GetIsInBattle ())
    {
        ErrorExit ("Expected that unit2 will be in battle!");
        return;
    }
    else if (!battlesProcessor->GetBattleByIndex (0)->IsAttackerUnit (unit2->GetHash ()))
    {
        ErrorExit ("Expected that unit2 will be an attacker in battle!");
        return;
    }

    // array (X * HEIGHT + Y) = (X, Y)
    Colonization::FleetUnit *unit3 = (Colonization::FleetUnit *)
            unitsManager->CreateUnit (Colonization::UNIT_FLEET, "PlayerY",
                                      map->GetDistrictByIndex (2 * mapHeight + 1)->GetHash ());
    unit3->SetWarShipsCount (50);

    map->FindPath (map->GetDistrictByIndex (0 * mapHeight + 1)->GetHash (), unit3);
    if (unit3->GetWay ().Empty ())
    {
        ErrorExit ("Can't set unit3 way!");
        return;
    }

    for (int index = 0; index < 3; index++)
    {
        diplomacyProcessor->Update (Urho3D::E_SCENEUPDATE, updateEventData);
        playersManager->Update (Urho3D::E_SCENEUPDATE, updateEventData);
        battlesProcessor->Update (Urho3D::E_SCENEUPDATE, updateEventData);
        unitsManager->Update (Urho3D::E_SCENEUPDATE, updateEventData);
    }

    if (unit3->GetPositionHash () != map->GetDistrictByIndex (1 * mapHeight + 1)->GetHash ())
    {
        ErrorExit ("Expected that unit3 will be in (1;1), but it is in " +
                   map->GetDistrictByHash (unit3->GetPositionHash ())->GetName () + "!");
        return;
    }
    else if (!unit3->GetIsInBattle ())
    {
        ErrorExit ("Expected that unit3 will be in battle!");
        return;
    }
    else if (!battlesProcessor->GetBattleByIndex (0)->IsAttackerUnit (unit3->GetHash ()))
    {
        ErrorExit ("Expected that unit3 will be an attacker in battle!");
        return;
    }
    engine_->Exit ();
}

void TestBattleCreationAndJoiningApplication::Stop ()
{

}
}
