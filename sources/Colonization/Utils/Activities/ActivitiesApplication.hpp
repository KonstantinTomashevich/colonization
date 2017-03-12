#pragma once
#include <Urho3D/Engine/Application.h>
#include <Urho3D/Container/Ptr.h>
#include <Urho3D/Container/Vector.h>
#include <Colonization/Utils/Activities/Activity.hpp>

namespace Colonization
{
class ActivitiesApplication : public Urho3D::Application
{
URHO3D_OBJECT (ActivitiesApplication, Application)
protected:
    Urho3D::Vector <Urho3D::SharedPtr <Colonization::Activity> > currentActivities_;
    Urho3D::Vector <Urho3D::SharedPtr <Colonization::Activity> > activitiesToSetup_;
    Urho3D::Vector <Urho3D::SharedPtr <Colonization::Activity> > activitiesToStop_;
public:
    ActivitiesApplication (Urho3D::Context *context);
    virtual ~ActivitiesApplication ();

    virtual void Setup ();
    virtual void Start ();
    void UpdateActivities (Urho3D::StringHash eventType, Urho3D::VariantMap &eventData);
    virtual void Stop ();

    void SetupActivityNextFrame (Colonization::Activity *activity);
    void StopActivityNextFrame (Colonization::Activity *activity);
    unsigned GetActivitiesCount ();
    Colonization::Activity *GetActivityByIndex (int index);
};
}
