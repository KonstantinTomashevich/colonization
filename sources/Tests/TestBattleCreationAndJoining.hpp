#pragma once
#include <Urho3D/Engine/Application.h>
#include <Colonization/Backend/Player/Player.hpp>

namespace Tests
{
class TestBattleCreationAndJoiningApplication : public Urho3D::Application
{
URHO3D_OBJECT (TestBattleCreationAndJoiningApplication, Object)
public:
    TestBattleCreationAndJoiningApplication (Urho3D::Context *context);
    virtual ~TestBattleCreationAndJoiningApplication ();

    virtual void Setup ();
    virtual void Start ();
    virtual void Stop ();
};
}

