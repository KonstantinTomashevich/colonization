#include <Colonization/BuildConfiguration.hpp>
#include "BindLauncherApplication.hpp"
#include <Urho3D/ThirdParty/AngelScript/angelscript.h>
#include <Urho3D/AngelScript/APITemplates.h>
#include <Launcher/LauncherApplication.hpp>


namespace ColonizationLauncher
{
void BindLauncherApplication (Urho3D::Script *script)
{
    asIScriptEngine *engine = script->GetScriptEngine ();
    Urho3D::RegisterObject <LauncherApplication> (engine, "LauncherApplication");

    engine->RegisterObjectMethod ("LauncherApplication", "void SetupActivityNextFrame (Activity @+ activity)", asMETHOD (LauncherApplication, SetupActivityNextFrame), asCALL_THISCALL);
    engine->RegisterObjectMethod ("LauncherApplication", "void StopActivityNextFrame (Activity @+ activity)", asMETHOD (LauncherApplication, StopActivityNextFrame), asCALL_THISCALL);
    engine->RegisterObjectMethod ("LauncherApplication", "int GetActivitiesCount ()", asMETHOD (LauncherApplication, GetActivitiesCount), asCALL_THISCALL);
    engine->RegisterObjectMethod ("LauncherApplication", "Activity @+ GetActivityByIndex (int index)", asMETHOD (LauncherApplication, GetActivityByIndex), asCALL_THISCALL);
}
}
