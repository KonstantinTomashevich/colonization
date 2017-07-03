#pragma once
#include <Urho3D/Engine/Application.h>
#include <Colonization/Backend/Player/Player.hpp>

namespace Tests
{
class TestBattleProcessingAndLootingApplication : public Urho3D::Application
{
URHO3D_OBJECT (TestBattleProcessingAndLootingApplication, Object)
public:
    explicit TestBattleProcessingAndLootingApplication (Urho3D::Context *context);
    virtual ~TestBattleProcessingAndLootingApplication ();

    virtual void Setup ();
    virtual void Start ();
    virtual void Stop ();
};
}

