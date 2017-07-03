#pragma once
#include <Urho3D/Engine/Application.h>

namespace Tests
{
class TestDistrictsNeighborsCalculationApplication : public Urho3D::Application
{
URHO3D_OBJECT (TestDistrictsNeighborsCalculationApplication, Object)
public:
    explicit TestDistrictsNeighborsCalculationApplication (Urho3D::Context *context);
    virtual ~TestDistrictsNeighborsCalculationApplication ();

    virtual void Setup ();
    virtual void Start ();
    virtual void Stop ();
};
}
