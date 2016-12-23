#pragma once
#include <Urho3D/Engine/Application.h>

namespace Tests
{
class TestInternalTradeAreaApplication : public Urho3D::Application
{
URHO3D_OBJECT (TestInternalTradeAreaApplication, Object)
public:
    TestInternalTradeAreaApplication (Urho3D::Context *context);
    virtual ~TestInternalTradeAreaApplication ();

    virtual void Setup ();
    virtual void Start ();
    virtual void Stop ();
};
}

