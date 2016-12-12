#include <Colonization/BuildConfiguration.hpp>
#include "LauncherApplication.hpp"
#include <Urho3D/Input/Input.h>
#include <Urho3D/Core/CoreEvents.h>
#include <Urho3D/AngelScript/Script.h>

#include <Launcher/AngelScriptBinders/BindLauncherApplication.hpp>
#include <Colonization/AngelScriptBinders/BindActivity.hpp>
#include <Colonization/AngelScriptBinders/BindDistrict.hpp>
#include <Colonization/AngelScriptBinders/BindMap.hpp>
#include <Colonization/AngelScriptBinders/BindNetworkMessageType.hpp>
#include <Colonization/AngelScriptBinders/BindGameStateType.hpp>
#include <Colonization/AngelScriptBinders/BindHostActivity.hpp>
#include <Colonization/AngelScriptBinders/BindMainMenuActivity.hpp>
#include <Colonization/AngelScriptBinders/BindIngamePlayerActivity.hpp>
#include <Colonization/AngelScriptBinders/BindAngelScriptDirectMemoryAccess.hpp>
#include <Colonization/AngelScriptBinders/BindUnits.hpp>
#include <Colonization/AngelScriptBinders/BindUnitsContainer.hpp>
#include <Colonization/Activities/MainMenuActivity.hpp>

URHO3D_DEFINE_APPLICATION_MAIN (ColonizationLauncher::LauncherApplication)
namespace ColonizationLauncher
{
LauncherApplication::LauncherApplication (Urho3D::Context *context) : Urho3D::Application (context),
    currentActivities_ (),
    activitiesToSetup_ (),
    activitiesToStop_ ()
{

}

LauncherApplication::~LauncherApplication ()
{

}

void LauncherApplication::Setup ()
{
    engineParameters_ ["FullScreen"] = false;
    engineParameters_ ["WindowResizable"] = true;
    engineParameters_ ["LogName"] = "Colonization.log";
    engineParameters_ ["WindowTitle"] = "Colonization";
}

void LauncherApplication::Start ()
{
    // Set mouse to free mode
    Urho3D::Input *input = GetSubsystem <Urho3D::Input> ();
    input->SetMouseVisible (true);
    input->SetMouseMode (Urho3D::MM_FREE);

    // Init random
    Urho3D::SetRandomSeed (Urho3D::Time::GetTimeSinceEpoch ());

    SubscribeToEvent (Urho3D::E_UPDATE, URHO3D_HANDLER (LauncherApplication, Update));
    if (!currentActivities_.Empty ())
        for (int index = 0; index < currentActivities_.Size (); index++)
            currentActivities_.At (index)->Start ();

    // Register AngelScript subsystem.
    Urho3D::Script *script = new Urho3D::Script (context_);
    context_->RegisterSubsystem (script);
    Colonization::BindActivity (script);
    BindLauncherApplication (script);
    Colonization::BindDistrict (script);
    Colonization::BindMap (script);
    Colonization::BindNetworkMessageType (script);
    Colonization::BindGameStateType (script);
    Colonization::BindHostActivity (script);
    Colonization::BindMainMenuActivity (script);
    Colonization::BindIngamePlayerActivity (script);
    Colonization::BindAngelScriptDirectMemoryAccess (script);
    Colonization::BindUnits (script);
    Colonization::BindUnitsContainer (script);

    Urho3D::SharedPtr <Colonization::MainMenuActivity> mainMenuActivity (new Colonization::MainMenuActivity (context_));
    SetupActivityNextFrame (mainMenuActivity);
}

void LauncherApplication::Update (Urho3D::StringHash eventType, Urho3D::VariantMap &eventData)
{
    float timeStep = eventData [Urho3D::Update::P_TIMESTEP].GetFloat ();
    if (!activitiesToStop_.Empty ())
        while (!activitiesToStop_.Empty ())
        {
            Urho3D::SharedPtr <Colonization::Activity> activity = activitiesToStop_.Front ();
            activitiesToStop_.Remove (activity);
            assert (activity.NotNull ());
            currentActivities_.Remove (activity);
            activity->Stop ();
        }

    if (!activitiesToSetup_.Empty ())
        while (!activitiesToSetup_.Empty ())
        {
            Urho3D::SharedPtr <Colonization::Activity> activity = activitiesToSetup_.Front ();
            activitiesToSetup_.Remove (activity);
            assert (activity.NotNull ());
            currentActivities_.Push (activity);
            activity->SetApplication (this);
            activity->Start ();
        }

    if (!currentActivities_.Empty ())
        for (int index = 0; index < currentActivities_.Size (); index++)
            currentActivities_.At (index)->Update (timeStep);
}

void LauncherApplication::Stop ()
{
    if (!currentActivities_.Empty ())
        for (int index = 0; index < currentActivities_.Size (); index++)
            currentActivities_.At (index)->Stop ();
}

void LauncherApplication::SetupActivityNextFrame (Urho3D::SharedPtr <Colonization::Activity> activity)
{
    assert (activity.NotNull ());
    activitiesToSetup_.Push (activity);
}

void LauncherApplication::StopActivityNextFrame (Urho3D::SharedPtr <Colonization::Activity> activity)
{
    activitiesToStop_.Push (activity);
}

int LauncherApplication::GetActivitiesCount ()
{
    return currentActivities_.Size ();
}

Urho3D::SharedPtr <Colonization::Activity> LauncherApplication::GetActivityByIndex (int index)
{
    assert (index < currentActivities_.Size ());
    return currentActivities_.At (index);
}
}

