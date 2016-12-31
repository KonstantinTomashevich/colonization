#include <Colonization/BuildConfiguration.hpp>
#include "BindUnit.hpp"
#include <Urho3D/ThirdParty/AngelScript/angelscript.h>
#include <Urho3D/AngelScript/APITemplates.h>
#include <Colonization/Core/Unit.hpp>

namespace Colonization
{
// TODO: Not all functions are binded. Some server-side functions aren't binded.
Urho3D::CScriptArray *Unit_GetWay (Unit *unit)
{
    return Urho3D::VectorToArray <Urho3D::StringHash> (unit->GetWay (), "Array<StringHash>");
}

void Unit_SetWay (Unit *unit, Urho3D::CScriptArray *array)
{
    unit->SetWay (Urho3D::ArrayToPODVector <Urho3D::StringHash> (array));
}

void BindUnit (Urho3D::Script *script)
{
    BindUnitType (script);
    asIScriptEngine *engine = script->GetScriptEngine ();
    Urho3D::RegisterComponent <Unit> (engine, "Unit");
    BindUnitInterface (script, "Unit");
}

void BindUnitType(Urho3D::Script *script)
{
    asIScriptEngine *engine = script->GetScriptEngine ();
    engine->RegisterEnum ("UnitType");
    engine->RegisterEnumValue ("UnitType", "UNIT_FLEET", UNIT_FLEET);
    engine->RegisterEnumValue ("UnitType", "UNIT_TRADERS", UNIT_TRADERS);
    engine->RegisterEnumValue ("UnitType", "UNIT_COLONIZATORS", UNIT_COLONIZATORS);
    engine->RegisterEnumValue ("UnitType", "UNIT_ARMY", UNIT_ARMY);
}

void BindUnitInterface (Urho3D::Script *script, Urho3D::String className)
{
    asIScriptEngine *engine = script->GetScriptEngine ();
    engine->RegisterObjectMethod (className.CString (), "StringHash get_hash ()", asMETHOD (Unit, GetHash), asCALL_THISCALL);

    engine->RegisterObjectMethod (className.CString (), "UnitType get_unitType ()", asMETHOD (Unit, GetUnitType), asCALL_THISCALL);
    engine->RegisterObjectMethod (className.CString (), "void set_unitType (UnitType unitType)", asMETHOD (Unit, SetUnitType), asCALL_THISCALL);

    engine->RegisterObjectMethod (className.CString (), "String get_ownerPlayerName ()", asMETHOD (Unit, GetOwnerPlayerName), asCALL_THISCALL);
    engine->RegisterObjectMethod (className.CString (), "void set_ownerPlayerName (String ownerPlayerName)", asMETHOD (Unit, SetOwnerPlayerName), asCALL_THISCALL);

    engine->RegisterObjectMethod (className.CString (), "StringHash get_positionHash ()", asMETHOD (Unit, GetPositionHash), asCALL_THISCALL);
    engine->RegisterObjectMethod (className.CString (), "void set_positionHash (StringHash positionHash)", asMETHOD (Unit, SetPositionHash), asCALL_THISCALL);

    engine->RegisterObjectMethod (className.CString (), "Array <StringHash> @GetWay ()", asFUNCTION (Unit_GetWay), asCALL_CDECL_OBJFIRST);
    engine->RegisterObjectMethod (className.CString (), "void SetWay (Array <StringHash> @way)", asFUNCTION (Unit_SetWay), asCALL_CDECL_OBJFIRST);

    engine->RegisterObjectMethod (className.CString (), "float get_wayToNextDistrictProgressInPercents ()", asMETHOD (Unit, GetWayToNextDistrictProgressInPercents), asCALL_THISCALL);
    engine->RegisterObjectMethod (className.CString (), "void set_wayToNextDistrictProgressInPercents (float wayToNextDistrictProgressInPercents)", asMETHOD (Unit, SetWayToNextDistrictProgressInPercents), asCALL_THISCALL);

    engine->RegisterObjectMethod (className.CString (), "int get_fleetUnitWarShipsCount ()", asMETHOD (Unit, FleetUnitGetWarShipsCount), asCALL_THISCALL);
    engine->RegisterObjectMethod (className.CString (), "void set_fleetUnitWarShipsCount (int fleetUnitWarShipsCount)", asMETHOD (Unit, FleetUnitSetWarShipsCount), asCALL_THISCALL);

    engine->RegisterObjectMethod (className.CString (), "float get_tradersUnitTradeGoodsCost ()", asMETHOD (Unit, TradersUnitGetTradeGoodsCost), asCALL_THISCALL);
    engine->RegisterObjectMethod (className.CString (), "void set_tradersUnitTradeGoodsCost (float tradersUnitTradeGoodsCost)", asMETHOD (Unit, TradersUnitSetTradeGoodsCost), asCALL_THISCALL);

    engine->RegisterObjectMethod (className.CString (), "int get_colonizatorsUnitColonizatorsCount ()", asMETHOD (Unit, ColonizatorsUnitGetColonizatorsCount), asCALL_THISCALL);
    engine->RegisterObjectMethod (className.CString (), "void set_colonizatorsUnitColonizatorsCount (int colonizatorsUnitColonizatorsCount)", asMETHOD (Unit, ColonizatorsUnitSetColonizatorsCount), asCALL_THISCALL);

    engine->RegisterObjectMethod (className.CString (), "int get_armyUnitSoldiersCount ()", asMETHOD (Unit, ArmyUnitGetSoldiersCount), asCALL_THISCALL);
    engine->RegisterObjectMethod (className.CString (), "void set_armyUnitSoldiersCount (int armyUnitSoldiersCount)", asMETHOD (Unit, ArmyUnitSetSoldiersCount), asCALL_THISCALL);
}
}
