#pragma once
#include <Urho3D/Engine/Application.h>
#include <Urho3D/Scene/Scene.h>

namespace Tests
{
class TestColonyActionFormArmyApplication : public Urho3D::Application
{
URHO3D_OBJECT (TestColonyActionFormArmyApplication, Object)
protected:
    Urho3D::SharedPtr <Urho3D::Scene> scene_;

public:
    explicit TestColonyActionFormArmyApplication (Urho3D::Context *context);
    virtual ~TestColonyActionFormArmyApplication ();

    virtual void Setup ();
    virtual void Start ();
    virtual void Stop ();
};
}
