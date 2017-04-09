#include <Colonization/BuildConfiguration.hpp>
#include "BindPlayerInfo.hpp"
#include <Urho3D/ThirdParty/AngelScript/angelscript.h>
#include <Urho3D/AngelScript/APITemplates.h>
#include <Colonization/Core/PlayerInfo/PlayerInfo.hpp>

namespace Colonization
{
Urho3D::CScriptArray *PlayerInfo_GetEnemies (PlayerInfo *object)
{
    Urho3D::PODVector <Urho3D::StringHash> enemies = object->GetEnemies ();
    return Urho3D::VectorToArray (enemies, "Array <StringHash>");
}

void BindPlayerInfo (Urho3D::Script *script)
{
    asIScriptEngine *engine = script->GetScriptEngine ();
    Urho3D::RegisterComponent <PlayerInfo> (engine, "PlayerInfo");
    BindPlayerInfoInterface (script, "PlayerInfo");
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

    engine->RegisterObjectMethod (className.CString (), "bool get_isReadyForStart () const", asMETHOD (PlayerInfo, IsReadyForStart), asCALL_THISCALL);
    engine->RegisterObjectMethod (className.CString (), "void set_isReadyForStart (bool isReadyForStart)", asMETHOD (PlayerInfo, SetIsReadyForStart), asCALL_THISCALL);

    engine->RegisterObjectMethod (className.CString (), "int get_enemiesCount () const", asMETHOD (PlayerInfo, GetEnemiesCount), asCALL_THISCALL);
    engine->RegisterObjectMethod (className.CString (), "Array <StringHash> @get_enemies () const", asFUNCTION (PlayerInfo_GetEnemies), asCALL_CDECL_OBJFIRST);
    engine->RegisterObjectMethod (className.CString (), "StringHash GetEnemyByIndex (int index) const", asMETHOD (PlayerInfo, GetEnemyByIndex), asCALL_THISCALL);
    engine->RegisterObjectMethod (className.CString (), "bool IsAtWarWith (StringHash anotherPlayerNameHash) const", asMETHOD (PlayerInfo, IsAtWarWith), asCALL_THISCALL);
    engine->RegisterObjectMethod (className.CString (), "bool AddEnemy (StringHash anotherPlayerNameHash)", asMETHOD (PlayerInfo, AddEnemy), asCALL_THISCALL);
    engine->RegisterObjectMethod (className.CString (), "bool RemoveEnemy (StringHash anotherPlayerNameHash)", asMETHOD (PlayerInfo, RemoveEnemy), asCALL_THISCALL);
    engine->RegisterObjectMethod (className.CString (), "void RemoveAllEnemies ()", asMETHOD (PlayerInfo, RemoveAllEnemies), asCALL_THISCALL);

    engine->RegisterObjectMethod (className.CString (), "VariantMap get_progressToVictory () const", asMETHOD (PlayerInfo, GetProgressToVictory), asCALL_THISCALL);
    engine->RegisterObjectMethod (className.CString (), "void set_progressToVictory (VariantMap progressToVictory)", asMETHOD (PlayerInfo, SetProgressToVictory), asCALL_THISCALL);

    engine->RegisterObjectMethod (className.CString (), "float GetProgressToVictoryOfType (StringHash victoryType) const", asMETHOD (PlayerInfo, GetProgressToVictoryOfType), asCALL_THISCALL);
    engine->RegisterObjectMethod (className.CString (), "VariantMap GetProgressToVictoryOfTypeInfo (StringHash victoryType) const", asMETHOD (PlayerInfo, GetProgressToVictoryOfTypeInfo), asCALL_THISCALL);
    engine->RegisterObjectMethod (className.CString (), "void SetProgressToVictoryOfType (StringHash victoryType, VariantMap progressToVictoryInfo)", asMETHOD (PlayerInfo, SetProgressToVictoryOfTypeInfo), asCALL_THISCALL);
}
}
