#pragma once
#include <Colonization/Utils/Activities/ActivitiesApplication.hpp>

class LauncherApplication : public Colonization::ActivitiesApplication
{
URHO3D_OBJECT (LauncherApplication, ActivitiesApplication)
public:
    LauncherApplication (Urho3D::Context *context);
    virtual ~LauncherApplication ();

    virtual void Setup ();
    virtual void Start ();
    virtual void Stop ();
};
