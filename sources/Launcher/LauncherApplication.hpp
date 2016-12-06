#pragma once
#include <Urho3D/Engine/Application.h>
#include <Urho3D/Container/Ptr.h>
#include <Urho3D/Container/Vector.h>
#include <Colonization/Utils/Activity.hpp>

namespace ColonizationLauncher
{
class LauncherApplication : public Urho3D::Application
{
URHO3D_OBJECT (Application, Object)
protected:
    Urho3D::Vector <Urho3D::SharedPtr <Colonization::Activity> > currentActivities_;
    Urho3D::Vector <Urho3D::SharedPtr <Colonization::Activity> > activitiesToSetup_;
    Urho3D::Vector <Urho3D::SharedPtr <Colonization::Activity> > activitiesToStop_;
public:
    LauncherApplication (Urho3D::Context *context);
    virtual ~LauncherApplication ();

    virtual void Setup ();
    virtual void Start ();
    void Update (Urho3D::StringHash eventType, Urho3D::VariantMap &eventData);
    virtual void Stop ();

    void SetupActivityNextFrame (Urho3D::SharedPtr <Colonization::Activity> activity);
    void StopActivityNextFrame (Urho3D::SharedPtr <Colonization::Activity> activity);
    int GetActivitiesCount ();
    Urho3D::SharedPtr <Colonization::Activity> GetActivityByIndex (int index);
};
}

