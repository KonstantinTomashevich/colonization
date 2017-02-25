#include <Colonization/BuildConfiguration.hpp>
#include "BindPlayerInfo.hpp"
#include <Urho3D/ThirdParty/AngelScript/angelscript.h>
#include <Urho3D/AngelScript/APITemplates.h>
#include <Colonization/Core/PlayerInfo.hpp>

namespace Colonization
{
// To register constants to AngelScript, we have to create getters for them.
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

void BindPlayerInfo (Urho3D::Script *script)
{
    asIScriptEngine *engine = script->GetScriptEngine ();
    Urho3D::RegisterComponent <PlayerInfo> (engine, "PlayerInfo");
    BindPlayerInfoInterface (script, "PlayerInfo");

    engine->RegisterGlobalFunction ("StringHash get_PLAYER_INFO_VICTORY_TYPE_NAME_KEY ()", asFUNCTION (GetConst_PLAYER_INFO_VICTORY_TYPE_NAME_KEY), asCALL_CDECL);
    engine->RegisterGlobalFunction ("StringHash get_PLAYER_INFO_VICTORY_TYPE_INFO_KEY ()", asFUNCTION (GetConst_PLAYER_INFO_VICTORY_TYPE_INFO_KEY), asCALL_CDECL);
    engine->RegisterGlobalFunction ("StringHash get_PLAYER_INFO_VICTORY_TYPE_PROGRESS_KEY ()", asFUNCTION (GetConst_PLAYER_INFO_VICTORY_TYPE_PROGRESS_KEY), asCALL_CDECL);
    engine->RegisterGlobalFunction ("StringHash get_VICTORY_TYPE_BY_POINTS ()", asFUNCTION (GetConst_VICTORY_TYPE_BY_POINTS), asCALL_CDECL);
    engine->RegisterGlobalFunction ("String get_VICTORY_TYPE_BY_POINTS_NAME ()", asFUNCTION (GetConst_VICTORY_TYPE_BY_POINTS_NAME), asCALL_CDECL);
    engine->RegisterGlobalFunction ("String get_VICTORY_TYPE_BY_POINTS_INFO ()", asFUNCTION (GetConst_VICTORY_TYPE_BY_POINTS_INFO), asCALL_CDECL);
}

void BindPlayerInfoInterface (Urho3D::Script *script, Urho3D::String className)
{
    asIScriptEngine *engine = script->GetScriptEngine ();
    engine->RegisterObjectMethod (className.CString (), "String get_name () const", asMETHOD (PlayerInfo, GetName), asCALL_THISCALL);
    engine->RegisterObjectMethod (className.CString (), "void set_name (String name)", asMETHOD (PlayerInfo, SetName), asCALL_THISCALL);

    engine->RegisterObjectMethod (className.CString (), "float get_points () const", asMETHOD (PlayerInfo, GetPoints), asCALL_THISCALL);
    engine->RegisterObjectMethod (className.CString (), "void set_points (float points)", asMETHOD (PlayerInfo, SetPoints), asCALL_THISCALL);

    engine->RegisterObjectMethod (className.CString (), "Color get_color () const", asMETHOD (PlayerInfo, GetColor), asCALL_THISCALL);
    engine->RegisterObjectMethod (className.CString (), "void set_color (Color color)", asMETHOD (PlayerInfo, SetColor), asCALL_THISCALL);

    engine->RegisterObjectMethod (className.CString (), "VariantMap get_progressToVictory () const", asMETHOD (PlayerInfo, GetProgressToVictory), asCALL_THISCALL);
    engine->RegisterObjectMethod (className.CString (), "void set_progressToVictory (VariantMap progressToVictory)", asMETHOD (PlayerInfo, SetProgressToVictory), asCALL_THISCALL);

    engine->RegisterObjectMethod (className.CString (), "float GetProgressToVictoryOfType (StringHash victoryType) const", asMETHOD (PlayerInfo, GetProgressToVictoryOfType), asCALL_THISCALL);
    engine->RegisterObjectMethod (className.CString (), "VariantMap GetProgressToVictoryOfTypeInfo (StringHash victoryType) const", asMETHOD (PlayerInfo, GetProgressToVictoryOfTypeInfo), asCALL_THISCALL);
    engine->RegisterObjectMethod (className.CString (), "void SetProgressToVictoryOfType (StringHash victoryType, VariantMap progressToVictoryInfo)", asMETHOD (PlayerInfo, SetProgressToVictoryOfTypeInfo), asCALL_THISCALL);

    engine->RegisterObjectMethod (className.CString (), "bool get_isReadyForStart () const", asMETHOD (PlayerInfo, IsReadyForStart), asCALL_THISCALL);
    engine->RegisterObjectMethod (className.CString (), "void set_isReadyForStart (bool isReadyForStart)", asMETHOD (PlayerInfo, SetIsReadyForStart), asCALL_THISCALL);
}
}
