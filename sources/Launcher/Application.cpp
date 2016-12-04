#include <Colonization/BuildConfiguration.hpp>
#include "Application.hpp"
#include <Urho3D/Input/Input.h>
#include <Urho3D/Core/CoreEvents.h>

#include <Urho3D/AngelScript/Script.h>
#include <Launcher/AngelScriptBinders/BindApplication.hpp>
#include <Colonization/AngelScriptBinders/BindActivity.hpp>
#include <Colonization/AngelScriptBinders/BindDistrict.hpp>
#include <Colonization/AngelScriptBinders/BindMap.hpp>
#include <Colonization/AngelScriptBinders/BindNetworkMessageType.hpp>
#include <Colonization/AngelScriptBinders/BindGameStateType.hpp>
#include <Colonization/AngelScriptBinders/BindHostActivity.hpp>
#include <Colonization/AngelScriptBinders/BindMainMenuActivity.hpp>

URHO3D_DEFINE_APPLICATION_MAIN (ColonizationLauncher::Application)
namespace ColonizationLauncher
{
Application::Application (Urho3D::Context *context) : Urho3D::Application (context), currentActivities_ (), activitiesToStop_ ()
{

}

Application::~Application ()
{

}

void Application::Setup ()
{
    engineParameters_ ["FullScreen"] = false;
    engineParameters_ ["WindowResizable"] = true;
    engineParameters_ ["LogName"] = "Colonization.log";
    engineParameters_ ["WindowTitle"] = "Colonization";
}

void Application::Start ()
{
    // Set mouse to free mode
    Urho3D::Input *input = GetSubsystem <Urho3D::Input> ();
    input->SetMouseVisible (true);
    input->SetMouseMode (Urho3D::MM_FREE);

    SubscribeToEvent (Urho3D::E_UPDATE, URHO3D_HANDLER (Application, Update));
    if (!currentActivities_.Empty ())
        for (int index = 0; index < currentActivities_.Size (); index++)
            currentActivities_.At (index)->Start ();

    // Register AngelScript subsystem.
    Urho3D::Script *script = new Urho3D::Script (context_);
    context_->RegisterSubsystem (script);
    Colonization::BindActivity (script);
    BindApplication (script);
    Colonization::BindDistrict (script);
    Colonization::BindMap (script);
    Colonization::BindNetworkMessageType (script);
    Colonization::BindGameStateType (script);
    Colonization::BindHostActivity (script);
    Colonization::BindMainMenuActivity (script);
}

void Application::Update (Urho3D::StringHash eventType, Urho3D::VariantMap &eventData)
{
    float timeStep = eventData [Urho3D::Update::P_TIMESTEP].GetFloat ();
    if (!activitiesToStop_.Empty ())
        while (!activitiesToStop_.Empty ())
        {
            Urho3D::SharedPtr <Colonization::Activity> activity = activitiesToStop_.Front ();
            activitiesToStop_.Remove (activity);
            StopActivity (activity);
        }

    if (!currentActivities_.Empty ())
        for (int index = 0; index < currentActivities_.Size (); index++)
            currentActivities_.At (index)->Update (timeStep);
}

void Application::Stop ()
{
    if (!currentActivities_.Empty ())
        for (int index = 0; index < currentActivities_.Size (); index++)
            currentActivities_.At (index)->Stop ();
}

void Application::SetupActivity (Urho3D::SharedPtr <Colonization::Activity> activity)
{
    assert (activity.NotNull ());
    currentActivities_.Push (activity);
    activity->Start ();
}

void Application::StopActivity (Urho3D::SharedPtr <Colonization::Activity> activity)
{
    assert (activity.NotNull ());
    currentActivities_.Remove (activity);
    activity->Stop ();
}

void Application::StopActivityNextFrame (Urho3D::SharedPtr <Colonization::Activity> activity)
{
    activitiesToStop_.Push (activity);
}

int Application::GetActivitiesCount ()
{
    return currentActivities_.Size ();
}

Urho3D::SharedPtr<Colonization::Activity> Application::GetActivityByIndex (int index)
{
    assert (index < currentActivities_.Size ());
    return currentActivities_.At (index);
}
}

