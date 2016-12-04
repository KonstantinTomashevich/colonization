#include <Colonization/BuildConfiguration.hpp>
#include "BindHostActivity.hpp"
#include <Urho3D/ThirdParty/AngelScript/angelscript.h>
#include <Urho3D/AngelScript/APITemplates.h>
#include <Colonization/Utils/Activity.hpp>
#include <Colonization/Activities/HostActivity.hpp>
#include <Colonization/AngelScriptBinders/BindingMacroses.hpp>

namespace Colonization
{
void BindHostActivity (Urho3D::Script *script)
{
    asIScriptEngine *engine = script->GetScriptEngine ();
    Urho3D::RegisterObjectWithInheritance <HostActivity, Activity> (engine, "HostActivity", "Activity");

    CHECK_ANGELSCRIPT_RETURN (
                engine->RegisterObjectMethod ("HostActivity", "int GetServerPort ()",
                                              asMETHOD (HostActivity, GetServerPort), asCALL_THISCALL)
            );

    CHECK_ANGELSCRIPT_RETURN (
                engine->RegisterObjectMethod ("HostActivity", "void SetServerPort (int serverPort)",
                                              asMETHOD (HostActivity, SetServerPort), asCALL_THISCALL)
            );
}
}
