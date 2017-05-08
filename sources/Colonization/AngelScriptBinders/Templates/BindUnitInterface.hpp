#include <Colonization/BuildConfiguration.hpp>
#include <Colonization/AngelScriptBinders/Core/Unit/UnitGettersAndSetters.hpp>
#include <Urho3D/ThirdParty/AngelScript/angelscript.h>
#include <Urho3D/AngelScript/APITemplates.h>
#include <Colonization/Core/Unit/Unit.hpp>

namespace Colonization
{
template <class T> void BindUnitInterface (Urho3D::Script *script, Urho3D::String className)
{
    asIScriptEngine *engine = script->GetScriptEngine ();
    Urho3D::RegisterSubclass <Unit, T> (engine, "Unit", className.CString ());
    engine->RegisterObjectMethod (className.CString (), "bool IsCanGoTo (const District @district, const Map @map) const", asMETHOD (T, IsCanGoTo), asCALL_THISCALL);
    engine->RegisterObjectMethod (className.CString (), "StringHash get_hash () const", asMETHOD (T, GetHash), asCALL_THISCALL);

    engine->RegisterObjectMethod (className.CString (), "UnitType get_unitType () const", asMETHOD (T, GetUnitType), asCALL_THISCALL);
    engine->RegisterObjectMethod (className.CString (), "void set_unitType (UnitType unitType)", asMETHOD (T, SetUnitType), asCALL_THISCALL);

    engine->RegisterObjectMethod (className.CString (), "String get_ownerPlayerName () const", asMETHOD (T, GetOwnerPlayerName), asCALL_THISCALL);
    engine->RegisterObjectMethod (className.CString (), "void set_ownerPlayerName (String ownerPlayerName)", asMETHOD (T, SetOwnerPlayerName), asCALL_THISCALL);

    engine->RegisterObjectMethod (className.CString (), "StringHash get_positionHash () const", asMETHOD (T, GetPositionHash), asCALL_THISCALL);
    engine->RegisterObjectMethod (className.CString (), "void set_positionHash (StringHash positionHash)", asMETHOD (T, SetPositionHash), asCALL_THISCALL);

    engine->RegisterObjectMethod (className.CString (), "Array <StringHash> @GetWay () const", asFUNCTION (Unit_GetWay), asCALL_CDECL_OBJFIRST);
    engine->RegisterObjectMethod (className.CString (), "void SetWay (Array <StringHash> @way)", asFUNCTION (Unit_SetWay), asCALL_CDECL_OBJFIRST);

    engine->RegisterObjectMethod (className.CString (), "float get_wayToNextDistrictProgressInPercents () const", asMETHOD (T, GetWayToNextDistrictProgressInPercents), asCALL_THISCALL);
    engine->RegisterObjectMethod (className.CString (), "void set_wayToNextDistrictProgressInPercents (float wayToNextDistrictProgressInPercents)", asMETHOD (T, SetWayToNextDistrictProgressInPercents), asCALL_THISCALL);

    engine->RegisterObjectMethod (className.CString (), "float GetBattleAttackForce (GameConfiguration @configuration, bool isNaval) const", asMETHOD (T, GetBattleAttackForce), asCALL_THISCALL);
    engine->RegisterObjectMethod (className.CString (), "bool ApplyDamage (GameConfiguration @configuration, float damage)", asMETHOD (T, ApplyDamage), asCALL_THISCALL);
}
}
