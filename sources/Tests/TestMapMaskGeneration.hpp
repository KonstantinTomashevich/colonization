#pragma once
#include <Urho3D/Engine/Application.h>

namespace Tests
{
class TestMapMaskGenerationApplication : public Urho3D::Application
{
URHO3D_OBJECT (TestMapMaskGenerationApplication, Object)
public:
    explicit TestMapMaskGenerationApplication (Urho3D::Context *context);
    virtual ~TestMapMaskGenerationApplication ();

    virtual void Setup ();
    virtual void Start ();
    virtual void Stop ();
};
}
