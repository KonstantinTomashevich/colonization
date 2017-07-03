#pragma once
#include <Urho3D/Engine/Application.h>

namespace Tests
{
class TestBasicColonyBalanceApplication : public Urho3D::Application
{
URHO3D_OBJECT (TestBasicColonyBalanceApplication, Object)
public:
    explicit TestBasicColonyBalanceApplication (Urho3D::Context *context);
    virtual ~TestBasicColonyBalanceApplication ();

    virtual void Setup ();
    virtual void Start ();
    virtual void Stop ();
};
}

