#include <Colonization/BuildConfiguration.hpp>
#include "BindPlayerInfo.hpp"
#include <Urho3D/ThirdParty/AngelScript/angelscript.h>
#include <Urho3D/AngelScript/APITemplates.h>
#include <Colonization/Core/PlayerInfo/PlayerInfoConstants.hpp>

namespace Colonization
{
Urho3D::StringHash GetConst_PLAYER_INFO_VICTORY_TYPE_NAME_KEY ()
{
    return PLAYER_INFO_VICTORY_TYPE_NAME_KEY;
}

Urho3D::StringHash GetConst_PLAYER_INFO_VICTORY_TYPE_INFO_KEY ()
{
    return PLAYER_INFO_VICTORY_TYPE_INFO_KEY;
}

Urho3D::StringHash GetConst_PLAYER_INFO_VICTORY_TYPE_PROGRESS_KEY ()
{
    return PLAYER_INFO_VICTORY_TYPE_PROGRESS_KEY;
}

void BindPlayerInfoConstants (Urho3D::Script *script)
{
    asIScriptEngine *engine = script->GetScriptEngine ();
    engine->RegisterGlobalFunction ("StringHash get_PLAYER_INFO_VICTORY_TYPE_NAME_KEY ()", asFUNCTION (GetConst_PLAYER_INFO_VICTORY_TYPE_NAME_KEY), asCALL_CDECL);
    engine->RegisterGlobalFunction ("StringHash get_PLAYER_INFO_VICTORY_TYPE_INFO_KEY ()", asFUNCTION (GetConst_PLAYER_INFO_VICTORY_TYPE_INFO_KEY), asCALL_CDECL);
    engine->RegisterGlobalFunction ("StringHash get_PLAYER_INFO_VICTORY_TYPE_PROGRESS_KEY ()", asFUNCTION (GetConst_PLAYER_INFO_VICTORY_TYPE_PROGRESS_KEY), asCALL_CDECL);}
}
