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

    engine->RegisterObjectMethod ("PlayerInfo", "String get_name () const", asMETHOD (PlayerInfo, GetName), asCALL_THISCALL);
    engine->RegisterObjectMethod ("PlayerInfo", "void set_name (String name)", asMETHOD (PlayerInfo, SetName), asCALL_THISCALL);

    engine->RegisterObjectMethod ("PlayerInfo", "float get_points () const", asMETHOD (PlayerInfo, GetPoints), asCALL_THISCALL);
    engine->RegisterObjectMethod ("PlayerInfo", "void set_points (float points)", asMETHOD (PlayerInfo, SetPoints), asCALL_THISCALL);

    engine->RegisterObjectMethod ("PlayerInfo", "Color get_color () const", asMETHOD (PlayerInfo, GetColor), asCALL_THISCALL);
    engine->RegisterObjectMethod ("PlayerInfo", "void set_color (Color color)", asMETHOD (PlayerInfo, SetColor), asCALL_THISCALL);

    engine->RegisterObjectMethod ("PlayerInfo", "bool get_isReadyForStart () const", asMETHOD (PlayerInfo, IsReadyForStart), asCALL_THISCALL);
    engine->RegisterObjectMethod ("PlayerInfo", "void set_isReadyForStart (bool isReadyForStart)", asMETHOD (PlayerInfo, SetIsReadyForStart), asCALL_THISCALL);

    engine->RegisterObjectMethod ("PlayerInfo", "int get_enemiesCount () const", asMETHOD (PlayerInfo, GetEnemiesCount), asCALL_THISCALL);
    engine->RegisterObjectMethod ("PlayerInfo", "Array <StringHash> @get_enemies () const", asFUNCTION (PlayerInfo_GetEnemies), asCALL_CDECL_OBJFIRST);
    engine->RegisterObjectMethod ("PlayerInfo", "StringHash GetEnemyByIndex (int index) const", asMETHOD (PlayerInfo, GetEnemyByIndex), asCALL_THISCALL);
    engine->RegisterObjectMethod ("PlayerInfo", "bool IsAtWarWith (StringHash anotherPlayerNameHash) const", asMETHOD (PlayerInfo, IsAtWarWith), asCALL_THISCALL);
    engine->RegisterObjectMethod ("PlayerInfo", "bool AddEnemy (StringHash anotherPlayerNameHash)", asMETHOD (PlayerInfo, AddEnemy), asCALL_THISCALL);
    engine->RegisterObjectMethod ("PlayerInfo", "bool RemoveEnemy (StringHash anotherPlayerNameHash)", asMETHOD (PlayerInfo, RemoveEnemy), asCALL_THISCALL);
    engine->RegisterObjectMethod ("PlayerInfo", "void RemoveAllEnemies ()", asMETHOD (PlayerInfo, RemoveAllEnemies), asCALL_THISCALL);

    engine->RegisterObjectMethod ("PlayerInfo", "VariantMap get_progressToVictory () const", asMETHOD (PlayerInfo, GetProgressToVictory), asCALL_THISCALL);
    engine->RegisterObjectMethod ("PlayerInfo", "void set_progressToVictory (VariantMap progressToVictory)", asMETHOD (PlayerInfo, SetProgressToVictory), asCALL_THISCALL);

    engine->RegisterObjectMethod ("PlayerInfo", "float GetProgressToVictoryOfType (StringHash victoryType) const", asMETHOD (PlayerInfo, GetProgressToVictoryOfType), asCALL_THISCALL);
    engine->RegisterObjectMethod ("PlayerInfo", "VariantMap GetProgressToVictoryOfTypeInfo (StringHash victoryType) const", asMETHOD (PlayerInfo, GetProgressToVictoryOfTypeInfo), asCALL_THISCALL);
    engine->RegisterObjectMethod ("PlayerInfo", "void SetProgressToVictoryOfType (StringHash victoryType, VariantMap progressToVictoryInfo)", asMETHOD (PlayerInfo, SetProgressToVictoryOfTypeInfo), asCALL_THISCALL);
}
}
