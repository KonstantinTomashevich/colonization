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
void BindGameStateType(Urho3D::Script *script)
{
    asIScriptEngine *engine = script->GetScriptEngine ();
    engine->RegisterEnum ("GameStateType");
    engine->RegisterEnumValue ("GameStateType", "GAME_STATE_UNITIALIZED", GAME_STATE_UNITIALIZED);
    engine->RegisterEnumValue ("GameStateType", "GAME_STATE_WAITING_FOR_PLAYERS", GAME_STATE_WAITING_FOR_PLAYERS);
    engine->RegisterEnumValue ("GameStateType", "GAME_STATE_PLAYING", GAME_STATE_PLAYING);
    engine->RegisterEnumValue ("GameStateType", "GAME_STATE_FINISHED", GAME_STATE_FINISHED);
}

void BindHostActivity (Urho3D::Script *script)
{
    asIScriptEngine *engine = script->GetScriptEngine ();
    BindGameStateType (script);
    Urho3D::RegisterObject <HostActivity> (engine, "HostActivity");
    Urho3D::RegisterObjectConstructor <HostActivity> (engine, "HostActivity");
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
}
}
