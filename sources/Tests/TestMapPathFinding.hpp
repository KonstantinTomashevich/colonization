#pragma once
#include <Urho3D/Engine/Application.h>

namespace Tests
{
class TestMapPathFindingApplication : public Urho3D::Application
{
URHO3D_OBJECT (TestMapPathFindingApplication, Object)
public:
    TestMapPathFindingApplication (Urho3D::Context *context);
    virtual ~TestMapPathFindingApplication ();

    virtual void Setup ();
    virtual void Start ();
    virtual void Stop ();
};
}
