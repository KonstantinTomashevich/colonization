#include <Colonization/BuildConfiguration.hpp>
#include "BindApplication.hpp"
#include <Urho3D/ThirdParty/AngelScript/angelscript.h>
#include <Urho3D/AngelScript/APITemplates.h>
#include <Launcher/Application.hpp>
#include <Colonization/AngelScriptBinders/BindingMacroses.hpp>

namespace ColonizationLauncher
{
void BindApplication (Urho3D::Script *script)
{
    asIScriptEngine *engine = script->GetScriptEngine ();
    Urho3D::RegisterObject <ColonizationLauncher::Application> (engine, "LauncherApplication");

    CHECK_ANGELSCRIPT_RETURN (
                engine->RegisterObjectMethod ("LauncherApplication", "void SetupActivity (Activity @activity)",
                                              asMETHOD (ColonizationLauncher::Application, SetupActivity), asCALL_THISCALL)
            );

    CHECK_ANGELSCRIPT_RETURN (
                engine->RegisterObjectMethod ("LauncherApplication", "void StopActivity (Activity @activity)",
                                              asMETHOD (ColonizationLauncher::Application, StopActivity), asCALL_THISCALL)
            );

    CHECK_ANGELSCRIPT_RETURN (
                engine->RegisterObjectMethod ("LauncherApplication", "int GetActivitiesCount ()",
                                              asMETHOD (ColonizationLauncher::Application, GetActivitiesCount), asCALL_THISCALL)
            );

    CHECK_ANGELSCRIPT_RETURN (
                engine->RegisterObjectMethod ("LauncherApplication", "Activity @GetActivityByIndex (int index)",
                                              asMETHOD (ColonizationLauncher::Application, GetActivityByIndex), asCALL_THISCALL)
            );
}
}
