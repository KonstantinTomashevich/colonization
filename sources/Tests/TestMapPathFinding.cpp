#include "TestMapPathFinding.hpp"
#include <Urho3D/Input/Input.h>
#include <Urho3D/Core/CoreEvents.h>
#include <Urho3D/Scene/Scene.h>
#include <Urho3D/IO/Log.h>

#include <Colonization/Core/District/District.hpp>
#include <Colonization/Core/Map.hpp>
#include <Colonization/Backend/UnitsManager.hpp>
#include <Colonization/Core/GameConfiguration.hpp>
#include <Colonization/Utils/Hubs/RegisterAllObjects.hpp>

URHO3D_DEFINE_APPLICATION_MAIN (Tests::TestMapPathFindingApplication)
namespace Tests
{
TestMapPathFindingApplication::TestMapPathFindingApplication (Urho3D::Context *context) :
    Urho3D::Application (context)
{

}

TestMapPathFindingApplication::~TestMapPathFindingApplication ()
{

}

void TestMapPathFindingApplication::Setup ()
{
    engineParameters_ ["FullScreen"] = false;
    engineParameters_ ["WindowResizable"] = true;
    engineParameters_ ["LogName"] = "TestMapPathFinding.log";
    engineParameters_ ["WindowTitle"] = "TestMapPathFinding";
}

void TestMapPathFindingApplication::Start ()
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

    map->GetDistrictByIndex (2 * mapHeight + 1)->SetColony (true);
    map->GetDistrictByIndex (2 * mapHeight + 1)->SetColonyOwnerName ("PlayerX");
    map->RecalculateDistrictsNeighbors ();

    // Create UnitsManager and Unit.
    Colonization::UnitsManager *unitsManager = scene->CreateChild ("units")->CreateComponent <Colonization::UnitsManager> ();
    Colonization::Unit *unit = unitsManager->CreateUnit ();
    unit->SetUnitType (Colonization::UNIT_TRADERS);
    unit->SetOwnerPlayerName ("PlayerX");
    unit->SetPositionHash (map->GetDistrictByIndex (4 * mapHeight + 0)->GetHash ());

    // Firstly calculate path if ship owner isn't colony owner.
    Urho3D::PODVector <Urho3D::StringHash> way = map->FindPath (map->GetDistrictByIndex (2 * mapHeight + 2)->GetHash (), unit);

    Urho3D::String calculatedWay = "";
    for (int index = 0; index < way.Size (); index++)
    {
        calculatedWay += map->GetDistrictByHash (way.At (index))->GetName () + "\n";
    }

    Urho3D::Log::Write (Urho3D::LOG_INFO, "First calculated way:\n" + calculatedWay);
    if (way.Size () != 5)
    {
        ErrorExit ("First way points size don't match expected.");
    }
    else if (way.At (0) != map->GetDistrictByIndex (4 * mapHeight + 0)->GetHash ()||
             way.At (1) != map->GetDistrictByIndex (3 * mapHeight + 0)->GetHash () ||
             way.At (2) != map->GetDistrictByIndex (2 * mapHeight + 0)->GetHash () ||
             way.At (3) != map->GetDistrictByIndex (2 * mapHeight + 1)->GetHash () ||
             way.At (4) != map->GetDistrictByIndex (2 * mapHeight + 2)->GetHash ())
    {
        ErrorExit ("First way don't match expected.");
    }
    else
    {
        unit->SetOwnerPlayerName ("PlayerY");
        way = map->FindPath (map->GetDistrictByIndex (2 * mapHeight + 2)->GetHash (), unit);

        calculatedWay = "";
        for (int index = 0; index < way.Size (); index++)
        {
            calculatedWay += map->GetDistrictByHash (way.At (index))->GetName () + "\n";
        }

        Urho3D::Log::Write (Urho3D::LOG_INFO, "Second calculated way:\n" + calculatedWay);
        if (way.Size () != 13)
        {
            ErrorExit ("Second way points size don't match expected.");
        }
        else if (way.At (0) != map->GetDistrictByIndex (4 * mapHeight + 0)->GetHash () ||
                 way.At (1) != map->GetDistrictByIndex (4 * mapHeight + 1)->GetHash () ||
                 way.At (2) != map->GetDistrictByIndex (4 * mapHeight + 2)->GetHash () ||
                 way.At (3) != map->GetDistrictByIndex (4 * mapHeight + 3)->GetHash () ||
                 way.At (4) != map->GetDistrictByIndex (4 * mapHeight + 4)->GetHash () ||
                 way.At (5) != map->GetDistrictByIndex (3 * mapHeight + 4)->GetHash () ||
                 way.At (6) != map->GetDistrictByIndex (2 * mapHeight + 4)->GetHash () ||
                 way.At (7) != map->GetDistrictByIndex (1 * mapHeight + 4)->GetHash () ||
                 way.At (8) != map->GetDistrictByIndex (0 * mapHeight + 4)->GetHash () ||
                 way.At (9) != map->GetDistrictByIndex (0 * mapHeight + 3)->GetHash () ||
                 way.At (10) != map->GetDistrictByIndex (0 * mapHeight + 2)->GetHash () ||
                 way.At (11) != map->GetDistrictByIndex (1 * mapHeight + 2)->GetHash () ||
                 way.At (12) != map->GetDistrictByIndex (2 * mapHeight + 2)->GetHash ())
        {
            ErrorExit ("Second way don't match expected.");
        }
        else
        {
            engine_->Exit ();
        }
    }
}

void TestMapPathFindingApplication::Stop ()
{

}
}
