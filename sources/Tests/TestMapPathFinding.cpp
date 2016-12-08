#include "TestMapPathFinding.hpp"
#include <Urho3D/Input/Input.h>
#include <Urho3D/Core/CoreEvents.h>

#include <Urho3D/IO/Log.h>
#include <Colonization/Core/District.hpp>
#include <Colonization/Core/Map.hpp>

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

    Urho3D::SharedPtr <Colonization::Map> map (new Colonization::Map (context_));
    const int mapWidth = 5;
    const int mapHeight = 5;

    for (int x = 0; x < mapWidth; x++)
        for (int y = 0; y < mapHeight; y++)
        {
            Colonization::District *district = new Colonization::District (context_);
            district->name_ = "(" + Urho3D::String (x) + "; " + Urho3D::String (y) + ")";

            district->polygonPoints_.Push (Urho3D::Vector3 (x - 0.5f, 0.0f, y - 0.5f));
            district->polygonPoints_.Push (Urho3D::Vector3 (x + 0.5f, 0.0f, y - 0.5f));
            district->polygonPoints_.Push (Urho3D::Vector3 (x + 0.5f, 0.0f, y + 0.5f));
            district->polygonPoints_.Push (Urho3D::Vector3 (x - 0.5f, 0.0f, y + 0.5f));

            district->unitPosition_ = Urho3D::Vector3 (x, 0.0f, y);
            district->colonyPosition_ = Urho3D::Vector3 (x, 0.0f, y);
            map->AddDistrict (district);
        }

    // Map: (~ -- sea, = -- terrain, @ -- colony)
    //   0 1 2 3 4
    // 0 ~ ~ ~ ~ ~
    // 1 ~ = = = ~
    // 2 ~ ~ ~ = ~
    // 3 ~ = @ = ~
    // 4 ~ = ~ ~ ~
    // Start point: (0, 2).
    // End point: (4, 4).

    // array (X * HEIGHT + Y) = (X, Y)
    map->GetDistrictByIndex (1 * mapHeight + 1)->isSea_ = false;
    map->GetDistrictByIndex (2 * mapHeight + 1)->isSea_ = false;
    map->GetDistrictByIndex (3 * mapHeight + 1)->isSea_ = false;
    map->GetDistrictByIndex (3 * mapHeight + 2)->isSea_ = false;
    map->GetDistrictByIndex (3 * mapHeight + 3)->isSea_ = false;
    map->GetDistrictByIndex (2 * mapHeight + 3)->isSea_ = false;
    map->GetDistrictByIndex (1 * mapHeight + 3)->isSea_ = false;
    map->GetDistrictByIndex (1 * mapHeight + 4)->isSea_ = false;

    map->GetDistrictByIndex (2 * mapHeight + 3)->hasColony_ = true;
    map->GetDistrictByIndex (2 * mapHeight + 3)->colonyOwnerName_ = "PlayerX";
    map->UpdateNeighborsOfDistricts ();

    // Test map's serialization.
    Urho3D::Node *node = new Urho3D::Node (context_);
    map->UpdateDataNode (node, true);
    map->ReadDataFromNode (node);

    // Firstly calculate path if ship owner isn't colony owner.
    Urho3D::Vector <Colonization::District *> way = map->FindPath (
                map->GetDistrictByIndex (0 * mapHeight + 2), map->GetDistrictByIndex (4 * mapHeight + 4), "PlayerY", true);

    Urho3D::String calculatedWay = "";
    for (int index = 0; index < way.Size (); index++)
        calculatedWay += way.At (index)->name_ + "\n";

    Urho3D::Log::Write (Urho3D::LOG_INFO, "First calculated way:\n" + calculatedWay);
    if (way.Size () != 11)
        ErrorExit ("First way points size don't match expected.");
    else if (way.At (0) != map->GetDistrictByIndex (0 * mapHeight + 2) ||
             way.At (1) != map->GetDistrictByIndex (0 * mapHeight + 1) ||
             way.At (2) != map->GetDistrictByIndex (0 * mapHeight + 0) ||
             way.At (3) != map->GetDistrictByIndex (1 * mapHeight + 0) ||
             way.At (4) != map->GetDistrictByIndex (2 * mapHeight + 0) ||
             way.At (5) != map->GetDistrictByIndex (3 * mapHeight + 0) ||
             way.At (6) != map->GetDistrictByIndex (4 * mapHeight + 0) ||
             way.At (7) != map->GetDistrictByIndex (4 * mapHeight + 1) ||
             way.At (8) != map->GetDistrictByIndex (4 * mapHeight + 2) ||
             way.At (9) != map->GetDistrictByIndex (4 * mapHeight + 3) ||
             way.At (10) != map->GetDistrictByIndex (4 * mapHeight + 4))
        ErrorExit ("First way don't match expected.");
    else
    {
        way = map->FindPath (map->GetDistrictByIndex (0 * mapHeight + 2), map->GetDistrictByIndex (4 * mapHeight + 4), "PlayerX", true);
        calculatedWay = "";
        for (int index = 0; index < way.Size (); index++)
            calculatedWay += way.At (index)->name_ + "\n";

        Urho3D::Log::Write (Urho3D::LOG_INFO, "Second calculated way:\n" + calculatedWay);

        if (way.Size () != 7)
            ErrorExit ("Second way points size don't match expected.");
        else if (way.At (0) != map->GetDistrictByIndex (0 * mapHeight + 2) ||
                 way.At (1) != map->GetDistrictByIndex (1 * mapHeight + 2) ||
                 way.At (2) != map->GetDistrictByIndex (2 * mapHeight + 2) ||
                 way.At (3) != map->GetDistrictByIndex (2 * mapHeight + 3) ||
                 way.At (4) != map->GetDistrictByIndex (2 * mapHeight + 4) ||
                 way.At (5) != map->GetDistrictByIndex (3 * mapHeight + 4) ||
                 way.At (6) != map->GetDistrictByIndex (4 * mapHeight + 4))
            ErrorExit ("Second way don't match expected.");
        else
            engine_->Exit ();
    }
}

void TestMapPathFindingApplication::Stop ()
{

}
}
