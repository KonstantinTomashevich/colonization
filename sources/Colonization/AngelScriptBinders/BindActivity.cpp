#include <Colonization/BuildConfiguration.hpp>
#include "BindActivity.hpp"
#include <Urho3D/ThirdParty/AngelScript/angelscript.h>
#include <Urho3D/AngelScript/APITemplates.h>
#include <Colonization/Utils/Activity.hpp>
#include <Colonization/AngelScriptBinders/BindingMacroses.hpp>

namespace Colonization
{

void BindActivity (Urho3D::Script *script)
{
    asIScriptEngine *engine = script->GetScriptEngine ();
    Urho3D::RegisterObject <Activity> (engine, "Activity");
    BindActivityInterface (script, "Activity");
}

void BindActivityInterface (Urho3D::Script *script, Urho3D::String className)
{
    asIScriptEngine *engine = script->GetScriptEngine ();
    CHECK_ANGELSCRIPT_RETURN (
                engine->RegisterObjectMethod (className.CString (), "void Start ()",
                                              asMETHOD (Activity, Start), asCALL_THISCALL)
                );

    CHECK_ANGELSCRIPT_RETURN (
                engine->RegisterObjectMethod (className.CString (), "void Update (float timeStep)",
                                              asMETHOD (Activity, Update), asCALL_THISCALL)
                );

    CHECK_ANGELSCRIPT_RETURN (
                engine->RegisterObjectMethod (className.CString (), "void Stop ()",
                                              asMETHOD (Activity, Stop), asCALL_THISCALL)
                );
}
}
