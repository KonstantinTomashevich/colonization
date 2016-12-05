#include <Colonization/BuildConfiguration.hpp>
#include "BindMainMenuActivity.hpp"
#include <Urho3D/ThirdParty/AngelScript/angelscript.h>
#include <Urho3D/AngelScript/APITemplates.h>
#include <Colonization/Utils/Activity.hpp>
#include <Colonization/Activities/MainMenuActivity.hpp>
#include <Colonization/AngelScriptBinders/BindActivity.hpp>
#include <Colonization/AngelScriptBinders/BindingMacroses.hpp>

namespace Colonization
{
MainMenuActivity *CreateMainMenuActivity ()
{
    return new MainMenuActivity (Urho3D::GetScriptContext ());
}

void BindMainMenuActivity (Urho3D::Script *script)
{
    asIScriptEngine *engine = script->GetScriptEngine ();
    Urho3D::RegisterObject <MainMenuActivity> (engine, "MainMenuActivity");
    Urho3D::RegisterSubclass <Activity, MainMenuActivity> (engine, "Activity", "MainMenuActivity");
    BindActivityInterface (script, "MainMenuActivity");

    CHECK_ANGELSCRIPT_RETURN (
                engine->RegisterGlobalFunction (
                    "MainMenuActivity @CreateMainMenuActivity ()", asFUNCTION (CreateMainMenuActivity), asCALL_CDECL)
                );
}
}
