#include <Colonization/BuildConfiguration.hpp>
#include "BindLauncherApplication.hpp"
#include <Urho3D/ThirdParty/AngelScript/angelscript.h>
#include <Urho3D/AngelScript/APITemplates.h>
#include <Launcher/LauncherApplication.hpp>
#include <Colonization/AngelScriptBinders/BindingMacroses.hpp>

namespace ColonizationLauncher
{
void LauncherApplication_SetupActivity (LauncherApplication *app, Colonization::Activity *activity)
{
    assert (activity);
    Urho3D::SharedPtr <Colonization::Activity> activityPtr (activity);
    app->SetupActivity (activityPtr);
}

void LauncherApplication_StopActivity (LauncherApplication *app, Colonization::Activity *activity)
{
    assert (activity);
    Urho3D::SharedPtr <Colonization::Activity> activityPtr (activity);
    app->StopActivity (activityPtr);
}

Colonization::Activity *LauncherApplication_GetActivityByIndex (LauncherApplication *app, int index)
{
    Colonization::Activity *activity = app->GetActivityByIndex (index).Get ();
    activity->AddRef ();
    return activity;
}

void BindLauncherApplication (Urho3D::Script *script)
{
    asIScriptEngine *engine = script->GetScriptEngine ();
    Urho3D::RegisterObject <LauncherApplication> (engine, "LauncherApplication");

    CHECK_ANGELSCRIPT_RETURN (
                engine->RegisterObjectMethod ("LauncherApplication", "void SetupActivity (Activity @activity)",
                                              asFUNCTION (LauncherApplication_SetupActivity), asCALL_CDECL_OBJFIRST)
            );

    CHECK_ANGELSCRIPT_RETURN (
                engine->RegisterObjectMethod ("LauncherApplication", "void StopActivity (Activity @activity)",
                                              asFUNCTION (LauncherApplication_StopActivity), asCALL_CDECL_OBJFIRST)
            );

    CHECK_ANGELSCRIPT_RETURN (
                engine->RegisterObjectMethod ("LauncherApplication", "int GetActivitiesCount ()",
                                              asMETHOD (LauncherApplication, GetActivitiesCount), asCALL_THISCALL)
            );

    CHECK_ANGELSCRIPT_RETURN (
                engine->RegisterObjectMethod ("LauncherApplication", "Activity @GetActivityByIndex (int index)",
                                              asFUNCTION (LauncherApplication_GetActivityByIndex), asCALL_CDECL_OBJFIRST)
            );
}
}
