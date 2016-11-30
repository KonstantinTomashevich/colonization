#pragma once
#include <Urho3D/Engine/Application.h>
#include <Urho3D/Container/Ptr.h>
#include <Urho3D/Container/Vector.h>
#include <Colonization/Utils/Activity.hpp>

namespace Tests
{
class TestDistrictsNeighborsCalculationApplication : public Urho3D::Application
{
URHO3D_OBJECT (TestDistrictsNeighborsCalculationApplication, Object)
public:
    TestDistrictsNeighborsCalculationApplication (Urho3D::Context *context);
    virtual ~TestDistrictsNeighborsCalculationApplication ();

    virtual void Setup ();
    virtual void Start ();
    virtual void Stop ();
};
}
