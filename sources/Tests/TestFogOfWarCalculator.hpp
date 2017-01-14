#pragma once
#include <Urho3D/Engine/Application.h>

namespace Tests
{
class TestFogOfWarCalculatorApplication : public Urho3D::Application
{
URHO3D_OBJECT (TestFogOfWarCalculatorApplication, Object)
public:
    TestFogOfWarCalculatorApplication (Urho3D::Context *context);
    virtual ~TestFogOfWarCalculatorApplication ();

    virtual void Setup ();
    virtual void Start ();
    virtual void Stop ();
};
}
