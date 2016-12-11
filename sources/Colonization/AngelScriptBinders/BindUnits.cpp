#include <Colonization/BuildConfiguration.hpp>
#include "BindUnits.hpp"
#include <Urho3D/ThirdParty/AngelScript/angelscript.h>
#include <Urho3D/AngelScript/APITemplates.h>
#include <Colonization/Core/Units.hpp>
#include <Colonization/AngelScriptBinders/BindingMacroses.hpp>

namespace Colonization
{
OBJECT_PROPERTY_GETTER_AND_SETTER (Unit ,Urho3D::String, ownerPlayer_)
OBJECT_PROPERTY_GETTER_AND_SETTER (Unit, UnitType, unitType_)
OBJECT_PROPERTY_GETTER_AND_SETTER (Unit, District*, position_)
OBJECT_ARRAY_OF_POINTERS_PROPERTY_ACESSOR (Unit, PODVector, District*, way_, "Array<District@>")
OBJECT_PROPERTY_GETTER_AND_SETTER (Unit, float, wayToNextDistrictProgressInPercents_)

OBJECT_PROPERTY_GETTER_AND_SETTER (FleetUnit, int, warShipsCount_)

OBJECT_PROPERTY_GETTER_AND_SETTER (TradersUnit, float, tradeGoodsCost_)

OBJECT_PROPERTY_GETTER_AND_SETTER (ColonizatorsUnit, int, colonizatorsCount_)

OBJECT_PROPERTY_GETTER_AND_SETTER (ArmyUnit, int, soldiersCount_)

void BindUnits (Urho3D::Script *script)
{
    BindUnitType (script);
    BindUnit (script);
    BindFleetUnit (script);
    BindTradersUnit (script);
    BindColonizatorsUnit (script);
    BindArmyUnit (script);
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


void BindUnit(Urho3D::Script *script)
{
    asIScriptEngine *engine = script->GetScriptEngine ();
    Urho3D::RegisterObject <Unit> (engine, "Unit");
    BindUnitInterface (script, "Unit");
}

void BindUnitInterface (Urho3D::Script *script, Urho3D::String className)
{
    asIScriptEngine *engine = script->GetScriptEngine ();
    BIND_OBJECT_PROPERTY_GETTER_AND_SETTER (engine, Unit, className.CString (), String, ownerPlayer_);
    BIND_OBJECT_PROPERTY_GETTER_AND_SETTER (engine, Unit, className.CString (), UnitType, unitType_);
    BIND_OBJECT_PROPERTY_GETTER_AND_SETTER (engine, Unit, className.CString (), District@, position_);
    BIND_OBJECT_ARRAY_PROPERTY (engine, Unit, className.CString (), District@, way_);
    BIND_OBJECT_PROPERTY_GETTER_AND_SETTER (engine, Unit, className.CString (), float, wayToNextDistrictProgressInPercents_);
}

void BindFleetUnit (Urho3D::Script *script)
{
    asIScriptEngine *engine = script->GetScriptEngine ();
    Urho3D::RegisterObject <FleetUnit> (engine, "FleetUnit");
    Urho3D::RegisterObjectConstructor <FleetUnit> (engine, "FleetUnit");
    Urho3D::RegisterSubclass <Unit, FleetUnit> (engine, "Unit", "FleetUnit");
    BindUnitInterface (script, "FleetUnit");
    BIND_OBJECT_PROPERTY_GETTER_AND_SETTER (engine, FleetUnit, "FleetUnit", int, warShipsCount_);
}

void BindTradersUnit (Urho3D::Script *script)
{
    asIScriptEngine *engine = script->GetScriptEngine ();
    Urho3D::RegisterObject <TradersUnit> (engine, "TradersUnit");
    Urho3D::RegisterObjectConstructor <TradersUnit> (engine, "TradersUnit");
    Urho3D::RegisterSubclass <Unit, TradersUnit> (engine, "Unit", "TradersUnit");
    BindUnitInterface (script, "TradersUnit");
    BIND_OBJECT_PROPERTY_GETTER_AND_SETTER (engine, TradersUnit, "TradersUnit", float, tradeGoodsCost_);
}

void BindColonizatorsUnit (Urho3D::Script *script)
{
    asIScriptEngine *engine = script->GetScriptEngine ();
    Urho3D::RegisterObject <ColonizatorsUnit> (engine, "ColonizatorsUnit");
    Urho3D::RegisterObjectConstructor <ColonizatorsUnit> (engine, "ColonizatorsUnit");
    Urho3D::RegisterSubclass <Unit, ColonizatorsUnit> (engine, "Unit", "ColonizatorsUnit");
    BindUnitInterface (script, "ColonizatorsUnit");
    BIND_OBJECT_PROPERTY_GETTER_AND_SETTER (engine, ColonizatorsUnit, "ColonizatorsUnit", int, colonizatorsCount_);
}

void BindArmyUnit (Urho3D::Script *script)
{
    asIScriptEngine *engine = script->GetScriptEngine ();
    Urho3D::RegisterObject <ArmyUnit> (engine, "ArmyUnit");
    Urho3D::RegisterObjectConstructor <ArmyUnit> (engine, "ArmyUnit");
    Urho3D::RegisterSubclass <Unit, ArmyUnit> (engine, "Unit", "ArmyUnit");
    BindUnitInterface (script, "ArmyUnit");
    BIND_OBJECT_PROPERTY_GETTER_AND_SETTER (engine, ArmyUnit, "ArmyUnit", int, soldiersCount_);
}
}
