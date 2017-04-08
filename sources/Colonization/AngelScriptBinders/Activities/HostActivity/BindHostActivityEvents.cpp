#include <Colonization/BuildConfiguration.hpp>
#include "BindHostActivity.hpp"
#include <Urho3D/ThirdParty/AngelScript/angelscript.h>
#include <Urho3D/AngelScript/APITemplates.h>
#include <Colonization/Utils/Activities/Activity.hpp>
#include <Colonization/Activities/HostActivity/HostActivityEvents.hpp>

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

Urho3D::String get_EVENT_HOST_REQUEST_SELECT_MAP ()
{
    return EVENT_HOST_REQUEST_SELECT_MAP;
}

Urho3D::String get_HostRequestSelectMap_MAP_FOLDER ()
{
    return HostRequestSelectMap::MAP_FOLDER;
}

Urho3D::String get_HostRequestSelectMap_MAP_INFO_PATH ()
{
    return HostRequestSelectMap::MAP_INFO_PATH;
}

void BindHostActivityEvents (Urho3D::Script *script)
{
    asIScriptEngine *engine = script->GetScriptEngine ();
    engine->RegisterGlobalFunction ("String get_EVENT_HOST_REQUEST_GAME_START ()", asFUNCTION (get_EVENT_HOST_REQUEST_GAME_START), asCALL_CDECL);
    engine->RegisterGlobalFunction ("String get_EVENT_HOST_REQUEST_KICK_PLAYER ()", asFUNCTION (get_EVENT_HOST_REQUEST_KICK_PLAYER), asCALL_CDECL);
    engine->RegisterGlobalFunction ("String get_HostRequestKickPlayer_PLAYER_NAME ()", asFUNCTION (get_HostRequestKickPlayer_PLAYER_NAME), asCALL_CDECL);

    engine->RegisterGlobalFunction ("String get_EVENT_HOST_REQUEST_SELECT_MAP ()", asFUNCTION (get_EVENT_HOST_REQUEST_SELECT_MAP), asCALL_CDECL);
    engine->RegisterGlobalFunction ("String get_HostRequestSelectMap_MAP_FOLDER ()", asFUNCTION (get_HostRequestSelectMap_MAP_FOLDER), asCALL_CDECL);
    engine->RegisterGlobalFunction ("String get_HostRequestSelectMap_MAP_INFO_PATH ()", asFUNCTION (get_HostRequestSelectMap_MAP_INFO_PATH), asCALL_CDECL);
}
}
