#include <Colonization/BuildConfiguration.hpp>
#include "ActivitiesApplication.hpp"
#include <Urho3D/Core/CoreEvents.h>
#include <Urho3D/AngelScript/Script.h>

#include <Colonization/Utils/RegisterAllObjects.hpp>
#include <Colonization/Utils/BindAll.hpp>

namespace Colonization
{
ActivitiesApplication::ActivitiesApplication (Urho3D::Context *context) : Urho3D::Application (context),
    currentActivities_ (),
    activitiesToSetup_ (),
    activitiesToStop_ ()
{

}

ActivitiesApplication::~ActivitiesApplication ()
{

}

void ActivitiesApplication::Setup ()
{
    engineParameters_ ["FullScreen"] = false;
    engineParameters_ ["WindowResizable"] = true;
    engineParameters_ ["LogName"] = "Colonization.log";
    engineParameters_ ["WindowTitle"] = "Colonization";
}

void ActivitiesApplication::Start ()
{
    SubscribeToEvent (Urho3D::E_UPDATE, URHO3D_HANDLER (ActivitiesApplication, UpdateActivities));

    if (!currentActivities_.Empty ())
    {
        for (int index = 0; index < currentActivities_.Size (); index++)
        {
            currentActivities_.At (index)->Start ();
        }
    }

    RegisterAllObjects (context_);
    Urho3D::Script *script = new Urho3D::Script (context_);
    context_->RegisterSubsystem (script);
    BindAll (script);
}

void ActivitiesApplication::UpdateActivities (Urho3D::StringHash eventType, Urho3D::VariantMap &eventData)
{
    float timeStep = eventData [Urho3D::Update::P_TIMESTEP].GetFloat ();
    if (!activitiesToStop_.Empty ())
    {
        while (!activitiesToStop_.Empty ())
        {
            Urho3D::SharedPtr <Activity> activity = activitiesToStop_.Front ();
            activitiesToStop_.Remove (activity);
            assert (activity.NotNull ());
            currentActivities_.Remove (activity);
            activity->Stop ();
        }
    }

    if (!activitiesToSetup_.Empty ())
    {
        while (!activitiesToSetup_.Empty ())
        {
            Urho3D::SharedPtr <Activity> activity = activitiesToSetup_.Front ();
            activitiesToSetup_.Remove (activity);
            assert (activity.NotNull ());
            currentActivities_.Push (activity);
            activity->SetApplication (this);
            activity->Start ();
        }
    }

    if (!currentActivities_.Empty ())
    {
        for (int index = 0; index < currentActivities_.Size (); index++)
        {
            currentActivities_.At (index)->Update (timeStep);
        }
    }
}

void ActivitiesApplication::Stop ()
{
    if (!currentActivities_.Empty ())
    {
        for (int index = 0; index < currentActivities_.Size (); index++)
        {
            currentActivities_.At (index)->Stop ();
        }
    }
}

void ActivitiesApplication::SetupActivityNextFrame (Activity *activity)
{
    assert (activity);
    activitiesToSetup_.Push (Urho3D::SharedPtr <Activity> (activity));
}

void ActivitiesApplication::StopActivityNextFrame (Activity *activity)
{
    assert (activity);
    activitiesToStop_.Push (Urho3D::SharedPtr <Activity> (activity));
}

int ActivitiesApplication::GetActivitiesCount ()
{
    return currentActivities_.Size ();
}

Activity *ActivitiesApplication::GetActivityByIndex (int index)
{
    assert (index < currentActivities_.Size ());
    return currentActivities_.At (index);
}
}

