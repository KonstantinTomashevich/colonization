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

    CHECK_ANGELSCRIPT_RETURN (
                engine->RegisterObjectMethod ("Activity", "void Start ()",
                                              asMETHOD (Activity, Start), asCALL_THISCALL)
            );

    CHECK_ANGELSCRIPT_RETURN (
                engine->RegisterObjectMethod ("Activity", "void Update (float timeStep)",
                                              asMETHOD (Activity, Update), asCALL_THISCALL)
            );

    CHECK_ANGELSCRIPT_RETURN (
                engine->RegisterObjectMethod ("Activity", "void Stop ()",
                                              asMETHOD (Activity, Stop), asCALL_THISCALL)
            );
}
}
