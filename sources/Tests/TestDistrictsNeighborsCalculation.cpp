#include "TestDistrictsNeighborsCalculation.hpp"
#include <Urho3D/Input/Input.h>
#include <Urho3D/Core/CoreEvents.h>

#include <Urho3D/Scene/Scene.h>
#include <Urho3D/IO/Log.h>
#include <Colonization/Core/District/District.hpp>
#include <Colonization/Core/Map.hpp>
#include <Colonization/Utils/Hubs/RegisterAllObjects.hpp>

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

    Colonization::RegisterAllObjects (context_);
    Urho3D::SharedPtr <Urho3D::Scene> scene (new Urho3D::Scene (context_));
    Colonization::Map *map = scene->CreateChild ("map")->CreateComponent <Colonization::Map> ();
    Colonization::District *districtA = map->CreateDistrict ("A");
    Colonization::District *districtB = map->CreateDistrict ("B");
    Colonization::District *districtC = map->CreateDistrict ("C");
    Colonization::District *districtD = map->CreateDistrict ("D");

    // A neighbors will be: B, D.
    Urho3D::PODVector <Urho3D::Vector3> pointsA;
    pointsA.Push (Urho3D::Vector3 (1.0f, 0.0f, 1.0f));
    pointsA.Push (Urho3D::Vector3 (-1.0f, 0.0f, 1.0f));
    pointsA.Push (Urho3D::Vector3 (-1.0f, 0.0f, -1.0f));
    pointsA.Push (Urho3D::Vector3 (1.0f, 0.0f, -1.0f));
    districtA->SetPolygonPoints (pointsA);

    // B neighbors will be: A, D.
    Urho3D::PODVector <Urho3D::Vector3> pointsB;
    pointsB.Push (Urho3D::Vector3 (1.0f, 0.0f, 1.0f));
    pointsB.Push (Urho3D::Vector3 (2.0f, 0.0f, 1.0f));
    pointsB.Push (Urho3D::Vector3 (1.0f, 0.0f, -1.0f));
    districtB->SetPolygonPoints (pointsB);

    // C neighbors will be: D.
    Urho3D::PODVector <Urho3D::Vector3> pointsC;
    pointsC.Push (Urho3D::Vector3 (-1.0f, 0.0f, 1.0f));
    pointsC.Push (Urho3D::Vector3 (-2.0f, 0.0f, 1.0f));
    pointsC.Push (Urho3D::Vector3 (-2.0f, 0.0f, 2.0f));
    districtC->SetPolygonPoints (pointsC);

    // D neighbors will be: A, B, C.
    Urho3D::PODVector <Urho3D::Vector3> pointsD;
    pointsD.Push (Urho3D::Vector3 (2.0f, 0.0f, 1.0f));
    pointsD.Push (Urho3D::Vector3 (1.0f, 0.0f, 1.0f));
    pointsD.Push (Urho3D::Vector3 (-1.0f, 0.0f, 1.0f));
    pointsD.Push (Urho3D::Vector3 (-2.0f, 0.0f, 1.0f));
    districtD->SetPolygonPoints (pointsD);
    map->RecalculateDistrictsNeighbors ();

    if (!districtA->GetNeighborsHashes ().Contains (districtB->GetHash ()) ||
            !districtA->GetNeighborsHashes ().Contains (districtD->GetHash ()))
    {
        ErrorExit ("Incorrect A neighbors!");
    }
    else if (!districtB->GetNeighborsHashes ().Contains (districtA->GetHash ()) ||
             !districtB->GetNeighborsHashes ().Contains (districtD->GetHash ()))
    {
        ErrorExit ("Incorrect B neighbors!");
    }
    else if (!districtC->GetNeighborsHashes ().Contains (districtD->GetHash ()))
    {
        ErrorExit ("Incorrect C neighbors!");
    }
    else if (!districtD->GetNeighborsHashes ().Contains (districtA->GetHash ()) ||
             !districtD->GetNeighborsHashes ().Contains (districtB->GetHash ()) ||
             !districtD->GetNeighborsHashes ().Contains (districtC->GetHash ()))
    {
        ErrorExit ("Incorrect D neighbors!");
    }
    else
    {
        engine_->Exit ();
    }
}

void TestDistrictsNeighborsCalculationApplication::Stop ()
{

}
}
