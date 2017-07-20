#include <Colonization/BuildConfiguration.hpp>
#include "ColonizationLauncher.hpp"
#include <Colonization/Activities/MainMenuActivity.hpp>
#include <Urho3D/Input/Input.h>

URHO3D_DEFINE_APPLICATION_MAIN (ColonizationLauncher)
ColonizationLauncher::ColonizationLauncher (Urho3D::Context *context) : Colonization::ActivitiesApplication (context)
{

}

ColonizationLauncher::~ColonizationLauncher ()
{

}

void ColonizationLauncher::Setup ()
{
    engineParameters_ ["FullScreen"] = false;
    engineParameters_ ["WindowResizable"] = true;
    engineParameters_ ["LogName"] = "Colonization.log";
    engineParameters_ ["WindowTitle"] = "Colonization";
    engineParameters_ ["ResourcePrefixPaths"] = "..;.";
}

void ColonizationLauncher::Start ()
{
    // Set mouse to free mode.
    Urho3D::Input *input = GetSubsystem <Urho3D::Input> ();
    input->SetMouseVisible (true);
    input->SetMouseMode (Urho3D::MM_FREE);

    // Generate random seed from system time.
    Urho3D::SetRandomSeed (Urho3D::Time::GetTimeSinceEpoch ());

    // Init activities application.
    ActivitiesApplication::Start ();

    // Setup main menu activity.
    Urho3D::SharedPtr <Colonization::MainMenuActivity> mainMenuActivity (new Colonization::MainMenuActivity (context_));
    SetupActivityNextFrame (mainMenuActivity);
}

void ColonizationLauncher::Stop ()
{
    ActivitiesApplication::Stop ();
}

