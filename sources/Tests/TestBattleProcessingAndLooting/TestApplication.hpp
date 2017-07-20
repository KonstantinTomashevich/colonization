#pragma once
#include <Urho3D/Engine/Application.h>

class TestApplication : public Urho3D::Application
{
URHO3D_OBJECT (TestApplication, Object)
public:
    explicit TestApplication (Urho3D::Context *context);
    virtual ~TestApplication ();

    virtual void Setup ();
    virtual void Start ();
    virtual void Stop ();
};
