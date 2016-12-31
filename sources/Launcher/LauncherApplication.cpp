#include <Colonization/BuildConfiguration.hpp>
#include "LauncherApplication.hpp"
#include <Colonization/Activities/MainMenuActivity.hpp>

URHO3D_DEFINE_APPLICATION_MAIN (LauncherApplication)
LauncherApplication::LauncherApplication (Urho3D::Context *context) : Colonization::ActivitiesApplication (context)
{

}

LauncherApplication::~LauncherApplication ()
{

}

void LauncherApplication::Setup ()
{
    ActivitiesApplication::Setup ();
}

void LauncherApplication::Start ()
{
    ActivitiesApplication::Start ();
    Urho3D::SharedPtr <Colonization::MainMenuActivity> mainMenuActivity (new Colonization::MainMenuActivity (context_));
    SetupActivityNextFrame (mainMenuActivity);
}

void LauncherApplication::Stop ()
{
    ActivitiesApplication::Stop ();
}

