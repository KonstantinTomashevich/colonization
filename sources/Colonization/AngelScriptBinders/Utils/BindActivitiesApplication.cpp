#include <Colonization/BuildConfiguration.hpp>
#include "BindActivitiesApplication.hpp"
#include <Urho3D/ThirdParty/AngelScript/angelscript.h>
#include <Urho3D/AngelScript/APITemplates.h>
#include <Colonization/Utils/Activities/ActivitiesApplication.hpp>


namespace Colonization
{
void BindActivitiesApplication (Urho3D::Script *script)
{
    asIScriptEngine *engine = script->GetScriptEngine ();
    Urho3D::RegisterObject <ActivitiesApplication> (engine, "ActivitiesApplication");

    engine->RegisterObjectMethod ("ActivitiesApplication", "void SetupActivityNextFrame (Activity @+ activity)", asMETHOD (ActivitiesApplication, SetupActivityNextFrame), asCALL_THISCALL);
    engine->RegisterObjectMethod ("ActivitiesApplication", "void StopActivityNextFrame (Activity @+ activity)", asMETHOD (ActivitiesApplication, StopActivityNextFrame), asCALL_THISCALL);
    engine->RegisterObjectMethod ("ActivitiesApplication", "uint GetActivitiesCount ()", asMETHOD (ActivitiesApplication, GetActivitiesCount), asCALL_THISCALL);
    engine->RegisterObjectMethod ("ActivitiesApplication", "Activity @+ GetActivityByIndex (int index)", asMETHOD (ActivitiesApplication, GetActivityByIndex), asCALL_THISCALL);
}
}
