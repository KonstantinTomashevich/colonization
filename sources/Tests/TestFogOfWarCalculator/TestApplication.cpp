#include "TestApplication.hpp"
#include <Urho3D/Input/Input.h>
#include <Urho3D/Core/CoreEvents.h>
#include <Urho3D/Scene/Scene.h>
#include <Urho3D/Scene/SceneEvents.h>
#include <Urho3D/IO/Log.h>

#include <Colonization/Core/District/District.hpp>
#include <Colonization/Core/Map.hpp>
#include <Colonization/Core/GameConfiguration.hpp>
#include <Colonization/Core/Unit/Unit.hpp>

#include <Colonization/Backend/UnitsManager.hpp>
#include <Colonization/Frontend/FogOfWarCalculator.hpp>
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

    Colonization::RegisterAllObjects (context_);
    Urho3D::SharedPtr <Urho3D::Scene> scene (new Urho3D::Scene (context_));
    scene->CreateComponent <Colonization::GameConfiguration> ();
    Colonization::Map *map = scene->CreateChild ("map")->CreateComponent <Colonization::Map> ();
    const int mapWidth = 5;
    const int mapHeight = 5;

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
            district->SetColonyOwnerName ("PlayerX");
        }
    }

    // Map: (~ -- sea, = -- terrain, @ -- colony)
    //   0 1 2 3 4
    // 4 ~ ~ ~ ~ ~
    // 3 ~ = = = ~
    // 2 ~ ~ ~ = ~
    // 1 ~ = @ = ~
    // 0 ~ = ~ ~ ~
    // Start point: (4, 0).
    // End point: (2, 2).

    // array (X * HEIGHT + Y) = (X, Y)
    map->GetDistrictByIndex (1 * mapHeight + 3)->SetIsSea (false);
    map->GetDistrictByIndex (2 * mapHeight + 3)->SetIsSea (false);
    map->GetDistrictByIndex (3 * mapHeight + 3)->SetIsSea (false);
    map->GetDistrictByIndex (3 * mapHeight + 2)->SetIsSea (false);
    map->GetDistrictByIndex (3 * mapHeight + 1)->SetIsSea (false);
    map->GetDistrictByIndex (2 * mapHeight + 1)->SetIsSea (false);
    map->GetDistrictByIndex (1 * mapHeight + 1)->SetIsSea (false);
    map->GetDistrictByIndex (1 * mapHeight + 0)->SetIsSea (false);

    map->GetDistrictByIndex (2 * mapHeight + 1)->SetHasColony (true);
    map->GetDistrictByIndex (2 * mapHeight + 1)->SetColonyOwnerName ("PlayerX");
    map->RecalculateDistrictsNeighbors ();

    Colonization::UnitsManager *unitsManager =
            scene->CreateChild ("units", Urho3D::REPLICATED)->CreateComponent <Colonization::UnitsManager> ();
    // array (X * HEIGHT + Y) = (X, Y)
    unitsManager->CreateUnit (Colonization::UNIT_FLEET, "PlayerX", map->GetDistrictByIndex (0 * mapHeight + 4)->GetHash ());
    unitsManager->CreateUnit (Colonization::UNIT_FLEET, "PlayerX", map->GetDistrictByIndex (4 * mapHeight + 1)->GetHash ());

    Colonization::FogOfWarCalculator *fogOfWarCalculator = scene->CreateComponent <Colonization::FogOfWarCalculator> ();
    fogOfWarCalculator->SetPlayerName ("PlayerX");
    Urho3D::VariantMap eventData;
    eventData [Urho3D::SceneUpdate::P_TIMESTEP] = 1.0f / 60.0f;
    fogOfWarCalculator->Update (Urho3D::E_SCENEUPDATE, eventData);

    Urho3D::HashMap <Urho3D::StringHash, bool> resultMap = fogOfWarCalculator->GetFogOfWarMap ();
    Urho3D::Log::Write (Urho3D::LOG_INFO, "Result fog of war map:");
    Urho3D::String resultMapStr = "\n";
    for (int y = 0; y < mapHeight; y++)
    {
        for (int x = 0; x < mapWidth; x++)
        {
            if (resultMap [map->GetDistrictByIndex (x * mapHeight + y)->GetHash ()])
            {
                resultMapStr += "+";
            }
            else
            {
                resultMapStr += "#";
            }
        }
        resultMapStr += "|\n";
    }
    Urho3D::Log::Write (Urho3D::LOG_INFO, resultMapStr);

    Urho3D::HashMap <Urho3D::StringHash, bool> expectedMap;
    for (int index = 0; index < mapWidth * mapHeight; index++)
    {
        expectedMap [map->GetDistrictByIndex (index)->GetHash ()] = false;
    }

    // array (X * HEIGHT + Y) = (X, Y)
    expectedMap [map->GetDistrictByIndex (0 * mapHeight + 4)->GetHash ()] = true;
    expectedMap [map->GetDistrictByIndex (1 * mapHeight + 4)->GetHash ()] = true;
    expectedMap [map->GetDistrictByIndex (0 * mapHeight + 3)->GetHash ()] = true;

    expectedMap [map->GetDistrictByIndex (2 * mapHeight + 1)->GetHash ()] = true;
    expectedMap [map->GetDistrictByIndex (3 * mapHeight + 1)->GetHash ()] = true;
    expectedMap [map->GetDistrictByIndex (1 * mapHeight + 1)->GetHash ()] = true;
    expectedMap [map->GetDistrictByIndex (2 * mapHeight + 2)->GetHash ()] = true;
    expectedMap [map->GetDistrictByIndex (2 * mapHeight + 0)->GetHash ()] = true;

    expectedMap [map->GetDistrictByIndex (4 * mapHeight + 2)->GetHash ()] = true;
    expectedMap [map->GetDistrictByIndex (4 * mapHeight + 1)->GetHash ()] = true;
    expectedMap [map->GetDistrictByIndex (4 * mapHeight + 0)->GetHash ()] = true;

    Urho3D::Log::Write (Urho3D::LOG_INFO, "Expected fog of war map:");
    Urho3D::String expectedMapStr = "\n";
    for (int y = 0; y < mapHeight; y++)
    {
        for (int x = 0; x < mapWidth; x++)
        {
            if (expectedMap [map->GetDistrictByIndex (x * mapHeight + y)->GetHash ()])
            {
                expectedMapStr += "+";
            }
            else
            {
                expectedMapStr += "#";
            }
        }
        expectedMapStr += "|\n";
    }
    Urho3D::Log::Write (Urho3D::LOG_INFO, expectedMapStr);

    if (resultMap != expectedMap)
    {
        ErrorExit ("Result fog of war map don't match expected!");
    }
    else
    {
        engine_->Exit ();
    }
}

void TestApplication::Stop ()
{

}
