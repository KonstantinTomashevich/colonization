#include <Colonization/BuildConfiguration.hpp>
#include "BindIngamePlayerActivity.hpp"
#include <Urho3D/ThirdParty/AngelScript/angelscript.h>
#include <Urho3D/AngelScript/APITemplates.h>
#include <Colonization/Utils/Activity.hpp>
#include <Colonization/Activities/IngamePlayerActivity.hpp>
#include <Colonization/AngelScriptBinders/BindActivity.hpp>
#include <Colonization/AngelScriptBinders/BindingMacroses.hpp>

namespace Colonization
{
IngamePlayerActivity *CreateIngamePlayerActivity ()
{
    return new IngamePlayerActivity (Urho3D::GetScriptContext ());
}

void BindIngamePlayerActivity (Urho3D::Script *script)
{
    asIScriptEngine *engine = script->GetScriptEngine ();
    Urho3D::RegisterObject <IngamePlayerActivity> (engine, "IngamePlayerActivity");
    Urho3D::RegisterSubclass <Activity, IngamePlayerActivity> (engine, "Activity", "IngamePlayerActivity");
    BindActivityInterface (script, "IngamePlayerActivity");

    CHECK_ANGELSCRIPT_RETURN (
                engine->RegisterObjectMethod ("IngamePlayerActivity", "String get_serverAdress ()",
                                              asMETHOD (IngamePlayerActivity, GetServerAdress), asCALL_THISCALL)
            );

    CHECK_ANGELSCRIPT_RETURN (
                engine->RegisterObjectMethod ("IngamePlayerActivity", "void set_serverAdress (String serverAdress)",
                                              asMETHOD (IngamePlayerActivity, SetServerAdress), asCALL_THISCALL)
            );

    CHECK_ANGELSCRIPT_RETURN (
                engine->RegisterObjectMethod ("IngamePlayerActivity", "int get_serverPort ()",
                                              asMETHOD (IngamePlayerActivity, GetServerPort), asCALL_THISCALL)
            );

    CHECK_ANGELSCRIPT_RETURN (
                engine->RegisterObjectMethod ("IngamePlayerActivity", "void set_serverPort (int serverPort)",
                                              asMETHOD (IngamePlayerActivity, SetServerPort), asCALL_THISCALL)
            );

    CHECK_ANGELSCRIPT_RETURN (
                engine->RegisterObjectMethod ("IngamePlayerActivity", "String get_playerName ()",
                                              asMETHOD (IngamePlayerActivity, GetPlayerName), asCALL_THISCALL)
            );

    CHECK_ANGELSCRIPT_RETURN (
                engine->RegisterObjectMethod ("IngamePlayerActivity", "void set_playerName (String playerName)",
                                              asMETHOD (IngamePlayerActivity, SetPlayerName), asCALL_THISCALL)
            );

    CHECK_ANGELSCRIPT_RETURN (
                engine->RegisterGlobalFunction (
                    "IngamePlayerActivity @CreateIngamePlayerActivity ()", asFUNCTION (CreateIngamePlayerActivity), asCALL_CDECL)
                );
}
}
