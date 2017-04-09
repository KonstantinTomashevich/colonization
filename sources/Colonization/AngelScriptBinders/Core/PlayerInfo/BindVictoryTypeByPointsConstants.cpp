#include <Colonization/BuildConfiguration.hpp>
#include "BindPlayerInfo.hpp"
#include <Urho3D/ThirdParty/AngelScript/angelscript.h>
#include <Urho3D/AngelScript/APITemplates.h>
#include <Colonization/Core/PlayerInfo/VictoryTypeByPointsConstants.hpp>

namespace Colonization
{
Urho3D::StringHash GetConst_VICTORY_TYPE_BY_POINTS ()
{
    return VICTORY_TYPE_BY_POINTS;
}

Urho3D::String GetConst_VICTORY_TYPE_BY_POINTS_NAME ()
{
    return VICTORY_TYPE_BY_POINTS_NAME;
}

Urho3D::String GetConst_VICTORY_TYPE_BY_POINTS_INFO ()
{
    return VICTORY_TYPE_BY_POINTS_INFO;
}

void BindVictoryTypeByPointsConstants (Urho3D::Script *script)
{
    asIScriptEngine *engine = script->GetScriptEngine ();
    engine->RegisterGlobalFunction ("StringHash get_VICTORY_TYPE_BY_POINTS ()", asFUNCTION (GetConst_VICTORY_TYPE_BY_POINTS), asCALL_CDECL);
    engine->RegisterGlobalFunction ("String get_VICTORY_TYPE_BY_POINTS_NAME ()", asFUNCTION (GetConst_VICTORY_TYPE_BY_POINTS_NAME), asCALL_CDECL);
    engine->RegisterGlobalFunction ("String get_VICTORY_TYPE_BY_POINTS_INFO ()", asFUNCTION (GetConst_VICTORY_TYPE_BY_POINTS_INFO), asCALL_CDECL);
}
}
