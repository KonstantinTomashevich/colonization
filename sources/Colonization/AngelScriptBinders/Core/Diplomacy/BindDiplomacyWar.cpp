#include <Colonization/BuildConfiguration.hpp>
#include "BindDiplomacyWar.hpp"
#include <Urho3D/ThirdParty/AngelScript/angelscript.h>
#include <Urho3D/AngelScript/APITemplates.h>
#include <Colonization/Core/Diplomacy/DiplomacyWar.hpp>

namespace Colonization
{
Urho3D::CScriptArray *DiplomacyWar_GetAttackersList (DiplomacyWar *object)
{
    return Urho3D::VectorToArray <Urho3D::StringHash> (object->GetAttackersList (), "Array <StringHash>");
}

Urho3D::CScriptArray *DiplomacyWar_GetDefendersList (DiplomacyWar *object)
{
    return Urho3D::VectorToArray <Urho3D::StringHash> (object->GetDefendersList (), "Array <StringHash>");
}

void BindDiplomacyWar (Urho3D::Script *script)
{
    asIScriptEngine *engine = script->GetScriptEngine ();
    Urho3D::RegisterComponent <DiplomacyWar> (engine, "DiplomacyWar");
    BindDiplomacyWarInterface (script, "DiplomacyWar");
}

void BindDiplomacyWarInterface (Urho3D::Script *script, Urho3D::String className)
{
    asIScriptEngine *engine = script->GetScriptEngine ();
    engine->RegisterObjectMethod (className.CString (), "StringHash get_hash () const", asMETHOD (DiplomacyWar, GetHash), asCALL_THISCALL);
    engine->RegisterObjectMethod (className.CString (), "float get_conflictEscalation () const", asMETHOD (DiplomacyWar, GetConflictEscalation), asCALL_THISCALL);
    engine->RegisterObjectMethod (className.CString (), "void set_conflictEscalation (float conflictEscalation)", asMETHOD (DiplomacyWar, SetConflictEscalation), asCALL_THISCALL);

    engine->RegisterObjectMethod (className.CString (), "uint get_attackersCount () const", asMETHOD (DiplomacyWar, GetAttackersCount), asCALL_THISCALL);
    engine->RegisterObjectMethod (className.CString (), "StringHash GetAttackerNameHashByIndex (int index) const", asMETHOD (DiplomacyWar, GetAttackerNameHashByIndex), asCALL_THISCALL);
    engine->RegisterObjectMethod (className.CString (), "bool AddAttackerNameHash (StringHash attackerNameHash)", asMETHOD (DiplomacyWar, AddAttackerNameHash), asCALL_THISCALL);
    engine->RegisterObjectMethod (className.CString (), "bool RemoveAttackerNameHash (StringHash attackerNameHash)", asMETHOD (DiplomacyWar, RemoveAttackerNameHash), asCALL_THISCALL);
    engine->RegisterObjectMethod (className.CString (), "bool IsAttacker (StringHash nameHash) const", asMETHOD (DiplomacyWar, IsAttacker), asCALL_THISCALL);
    engine->RegisterObjectMethod (className.CString (), "Array <StringHash> @get_attackersList () const", asFUNCTION (DiplomacyWar_GetAttackersList), asCALL_CDECL_OBJFIRST);

    engine->RegisterObjectMethod (className.CString (), "uint get_defendersCount () const", asMETHOD (DiplomacyWar, GetDefendersCount), asCALL_THISCALL);
    engine->RegisterObjectMethod (className.CString (), "StringHash GetDefenderNameHashByIndex (int index) const", asMETHOD (DiplomacyWar, GetDefenderNameHashByIndex), asCALL_THISCALL);
    engine->RegisterObjectMethod (className.CString (), "bool AddDefenderNameHash (StringHash defenderNameHash)", asMETHOD (DiplomacyWar, AddDefenderNameHash), asCALL_THISCALL);
    engine->RegisterObjectMethod (className.CString (), "bool RemoveDefenderNameHash (StringHash defenderNameHash)", asMETHOD (DiplomacyWar, RemoveDefenderNameHash), asCALL_THISCALL);
    engine->RegisterObjectMethod (className.CString (), "bool IsDefender (StringHash nameHash) const", asMETHOD (DiplomacyWar, IsDefender), asCALL_THISCALL);
    engine->RegisterObjectMethod (className.CString (), "Array <StringHash> @get_defendersList () const", asFUNCTION (DiplomacyWar_GetDefendersList), asCALL_CDECL_OBJFIRST);
}
}
