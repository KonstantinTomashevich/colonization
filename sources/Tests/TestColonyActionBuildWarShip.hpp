#pragma once
#include <Urho3D/Engine/Application.h>
#include <Urho3D/Scene/Scene.h>

namespace Tests
{
class TestColonyActionBuildWarShipApplication : public Urho3D::Application
{
URHO3D_OBJECT (TestColonyActionBuildWarShipApplication, Object)
public:
    explicit TestColonyActionBuildWarShipApplication (Urho3D::Context *context);
    virtual ~TestColonyActionBuildWarShipApplication ();

    virtual void Setup ();
    virtual void Start ();
    virtual void Stop ();
};
}
