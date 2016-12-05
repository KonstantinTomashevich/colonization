#include <Colonization/BuildConfiguration.hpp>
#include "BindHostActivity.hpp"
#include <Urho3D/ThirdParty/AngelScript/angelscript.h>
#include <Urho3D/AngelScript/APITemplates.h>
#include <Colonization/Utils/Activity.hpp>
#include <Colonization/Activities/HostActivity.hpp>
#include <Colonization/AngelScriptBinders/BindActivity.hpp>
#include <Colonization/AngelScriptBinders/BindingMacroses.hpp>

namespace Colonization
{
HostActivity *CreateHostActivity ()
{
    return new HostActivity (Urho3D::GetScriptContext ());
}

void BindHostActivity (Urho3D::Script *script)
{
    asIScriptEngine *engine = script->GetScriptEngine ();
    Urho3D::RegisterObject <HostActivity> (engine, "HostActivity");
    Urho3D::RegisterSubclass <Activity, HostActivity> (engine, "Activity", "HostActivity");
    BindActivityInterface (script, "HostActivity");

    CHECK_ANGELSCRIPT_RETURN (
                engine->RegisterObjectMethod ("HostActivity", "int get_serverPort ()",
                                              asMETHOD (HostActivity, GetServerPort), asCALL_THISCALL)
            );

    CHECK_ANGELSCRIPT_RETURN (
                engine->RegisterObjectMethod ("HostActivity", "void set_serverPort (int serverPort)",
                                              asMETHOD (HostActivity, SetServerPort), asCALL_THISCALL)
            );

    CHECK_ANGELSCRIPT_RETURN (
                engine->RegisterGlobalFunction (
                    "HostActivity @CreateHostActivity ()", asFUNCTION (CreateHostActivity), asCALL_CDECL)
                );
}
}
