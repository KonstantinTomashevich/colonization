#pragma once
#include <Urho3D/Engine/Application.h>
#include <Urho3D/Scene/Scene.h>

namespace Tests
{
class TestInternalTradeProcessingApplication : public Urho3D::Application
{
URHO3D_OBJECT (TestInternalTradeProcessingApplication, Object)
public:
    explicit TestInternalTradeProcessingApplication (Urho3D::Context *context);
    virtual ~TestInternalTradeProcessingApplication ();

    virtual void Setup ();
    virtual void Start ();
    virtual void Stop ();
};
}
