#pragma once
#include <Urho3D/Engine/Application.h>

namespace Tests
{
class TestInternalTradeProcessingApplication : public Urho3D::Application
{
URHO3D_OBJECT (TestInternalTradeProcessingApplication, Object)
public:
    TestInternalTradeProcessingApplication (Urho3D::Context *context);
    virtual ~TestInternalTradeProcessingApplication ();

    virtual void Setup ();
    virtual void Start ();
    virtual void Stop ();
};
}
