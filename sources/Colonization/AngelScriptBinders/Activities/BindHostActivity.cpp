#include <Colonization/BuildConfiguration.hpp>
#include "BindHostActivity.hpp"
#include <Urho3D/ThirdParty/AngelScript/angelscript.h>
#include <Urho3D/AngelScript/APITemplates.h>
#include <Colonization/Utils/Activities/Activity.hpp>
#include <Colonization/Activities/HostActivity.hpp>
#include <Colonization/AngelScriptBinders/Activities/BindActivity.hpp>

namespace Colonization
{
Urho3D::String get_EVENT_HOST_REQUEST_GAME_START ()
{
    return EVENT_HOST_REQUEST_GAME_START;
}

Urho3D::String get_EVENT_HOST_REQUEST_KICK_PLAYER ()
{
    return EVENT_HOST_REQUEST_KICK_PLAYER;
}

Urho3D::String get_HostRequestKickPlayer_PLAYER_NAME ()
{
    return HostRequestKickPlayer::PLAYER_NAME;
}

void BindGameStateType(Urho3D::Script *script)
{
    asIScriptEngine *engine = script->GetScriptEngine ();
    engine->RegisterEnum ("GameStateType");
    engine->RegisterEnumValue ("GameStateType", "GAME_STATE_UNITIALIZED", GAME_STATE_UNITIALIZED);
    engine->RegisterEnumValue ("GameStateType", "GAME_STATE_WAITING_FOR_START", GAME_STATE_WAITING_FOR_START);
    engine->RegisterEnumValue ("GameStateType", "GAME_STATE_PLAYING", GAME_STATE_PLAYING);
    engine->RegisterEnumValue ("GameStateType", "GAME_STATE_FINISHED", GAME_STATE_FINISHED);
}

void BindHostActivity (Urho3D::Script *script)
{
    asIScriptEngine *engine = script->GetScriptEngine ();
    BindGameStateType (script);
    engine->RegisterGlobalFunction ("String get_EVENT_HOST_REQUEST_GAME_START ()", asFUNCTION (get_EVENT_HOST_REQUEST_GAME_START), asCALL_CDECL);
    engine->RegisterGlobalFunction ("String get_EVENT_HOST_REQUEST_KICK_PLAYER ()", asFUNCTION (get_EVENT_HOST_REQUEST_KICK_PLAYER), asCALL_CDECL);
    engine->RegisterGlobalFunction ("String get_HostRequestKickPlayer_PLAYER_NAME ()", asFUNCTION (get_HostRequestKickPlayer_PLAYER_NAME), asCALL_CDECL);

    Urho3D::RegisterObject <HostActivity> (engine, "HostActivity");
    Urho3D::RegisterObjectConstructor <HostActivity> (engine, "HostActivity");
    Urho3D::RegisterSubclass <Activity, HostActivity> (engine, "Activity", "HostActivity");

    BindActivityInterface (script, "HostActivity");
    BindHostActivityInterface (script, "HostActivity");
}

void BindHostActivityInterface (Urho3D::Script *script, Urho3D::String className)
{
    asIScriptEngine *engine = script->GetScriptEngine ();
    engine->RegisterObjectMethod (className.CString (), "int get_serverPort ()", asMETHOD (HostActivity, GetServerPort), asCALL_THISCALL);
    engine->RegisterObjectMethod (className.CString (), "void set_serverPort (int serverPort)", asMETHOD (HostActivity, SetServerPort), asCALL_THISCALL);

    engine->RegisterObjectMethod (className.CString (), "String get_mapFolder () const", asMETHOD (HostActivity, GetMapFolder), asCALL_THISCALL);
    engine->RegisterObjectMethod (className.CString (), "void set_mapFolder (String mapFolder)", asMETHOD (HostActivity, SetMapFolder), asCALL_THISCALL);

    engine->RegisterObjectMethod (className.CString (), "String get_mapInfoPath () const", asMETHOD (HostActivity, GetMapInfoPath), asCALL_THISCALL);
    engine->RegisterObjectMethod (className.CString (), "void set_mapInfoPath (String mapInfoPath)", asMETHOD (HostActivity, SetMapInfoPath), asCALL_THISCALL);
    engine->RegisterObjectMethod (className.CString (), "bool IsStartRequested () const", asMETHOD (HostActivity, IsStartRequested), asCALL_THISCALL);
}
}
