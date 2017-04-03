#pragma once
#include <Urho3D/Engine/Application.h>
#include <Urho3D/Scene/Scene.h>

namespace Tests
{
class TestColonyActionBuildWarShipApplication : public Urho3D::Application
{
URHO3D_OBJECT (TestColonyActionBuildWarShipApplication, Object)
protected:
    Urho3D::SharedPtr <Urho3D::Scene> scene_;
    Urho3D::SharedPtr <Urho3D::Scene> sceneForReplication_;
    float timeUntilAutoExit_;

public:
    TestColonyActionBuildWarShipApplication (Urho3D::Context *context);
    virtual ~TestColonyActionBuildWarShipApplication ();

    virtual void Setup ();
    virtual void Start ();
    void Update (Urho3D::StringHash eventType, Urho3D::VariantMap &eventData);
    virtual void Stop ();
};
}
