#include "TestDistrictsNeighborsCalculation.hpp"
#include <Urho3D/Input/Input.h>
#include <Urho3D/Core/CoreEvents.h>

#include <Urho3D/IO/Log.h>
#include <Colonization/Core/District.hpp>
#include <Colonization/Core/Map.hpp>

URHO3D_DEFINE_APPLICATION_MAIN (Tests::TestDistrictsNeighborsCalculationApplication)
namespace Tests
{
TestDistrictsNeighborsCalculationApplication::TestDistrictsNeighborsCalculationApplication (Urho3D::Context *context) :
    Urho3D::Application (context)
{

}

TestDistrictsNeighborsCalculationApplication::~TestDistrictsNeighborsCalculationApplication ()
{

}

void TestDistrictsNeighborsCalculationApplication::Setup ()
{
    engineParameters_ ["FullScreen"] = false;
    engineParameters_ ["WindowResizable"] = true;
    engineParameters_ ["LogName"] = "TestDistrictsNeighborsCalculation.log";
    engineParameters_ ["WindowTitle"] = "TestDistrictsNeighborsCalculation";
}

void TestDistrictsNeighborsCalculationApplication::Start ()
{
    // Set mouse to free mode
    Urho3D::Input *input = GetSubsystem <Urho3D::Input> ();
    input->SetMouseVisible (true);
    input->SetMouseMode (Urho3D::MM_FREE);

    Colonization::District *districtA = new Colonization::District (context_);
    Colonization::District *districtB = new Colonization::District (context_);
    Colonization::District *districtC = new Colonization::District (context_);
    Colonization::District *districtD = new Colonization::District (context_);
    Urho3D::SharedPtr <Colonization::Map> map (new Colonization::Map (context_));

    // A neighbors will be: B, D.
    districtA->polygonPoints_.Push (Urho3D::Vector3 (1.0f, 0.0f, 1.0f));
    districtA->polygonPoints_.Push (Urho3D::Vector3 (-1.0f, 0.0f, 1.0f));
    districtA->polygonPoints_.Push (Urho3D::Vector3 (-1.0f, 0.0f, -1.0f));
    districtA->polygonPoints_.Push (Urho3D::Vector3 (1.0f, 0.0f, -1.0f));

    // B neighbors will be: A, D.
    districtB->polygonPoints_.Push (Urho3D::Vector3 (1.0f, 0.0f, 1.0f));
    districtB->polygonPoints_.Push (Urho3D::Vector3 (2.0f, 0.0f, 1.0f));
    districtB->polygonPoints_.Push (Urho3D::Vector3 (1.0f, 0.0f, -1.0f));

    // C neighbors will be: D.
    districtC->polygonPoints_.Push (Urho3D::Vector3 (-1.0f, 0.0f, 1.0f));
    districtC->polygonPoints_.Push (Urho3D::Vector3 (-2.0f, 0.0f, 1.0f));
    districtC->polygonPoints_.Push (Urho3D::Vector3 (-2.0f, 0.0f, 2.0f));

    // D neighbors will be: A, B, C.
    districtD->polygonPoints_.Push (Urho3D::Vector3 (2.0f, 0.0f, 1.0f));
    districtD->polygonPoints_.Push (Urho3D::Vector3 (1.0f, 0.0f, 1.0f));
    districtD->polygonPoints_.Push (Urho3D::Vector3 (-1.0f, 0.0f, 1.0f));
    districtD->polygonPoints_.Push (Urho3D::Vector3 (-2.0f, 0.0f, 1.0f));

    map->AddDistrict (districtA);
    map->AddDistrict (districtB);
    map->AddDistrict (districtC);
    map->AddDistrict (districtD);
    map->RecalculateDistrictsNeighbors ();

    if (!districtA->neighbors_.Contains (districtB) || !districtA->neighbors_.Contains (districtD))
        ErrorExit ("Incorrect A neighbors!");

    else if (!districtB->neighbors_.Contains (districtA) || !districtB->neighbors_.Contains (districtD))
        ErrorExit ("Incorrect B neighbors!");

    else if (!districtC->neighbors_.Contains (districtD))
        ErrorExit ("Incorrect C neighbors!");

    else if (!districtD->neighbors_.Contains (districtA) || !districtD->neighbors_.Contains (districtB) || !districtD->neighbors_.Contains (districtC))
        ErrorExit ("Incorrect D neighbors!");

    else
        engine_->Exit ();
}

void TestDistrictsNeighborsCalculationApplication::Stop ()
{

}
}
