#include <Colonization/BuildConfiguration.hpp>
#include "BindUnitType.hpp"
#include <Urho3D/ThirdParty/AngelScript/angelscript.h>
#include <Urho3D/AngelScript/APITemplates.h>
#include <Colonization/Core/Unit/UnitType.hpp>

namespace Colonization
{
void BindUnitType (Urho3D::Script *script)
{
    asIScriptEngine *engine = script->GetScriptEngine ();
    engine->RegisterEnum ("UnitType");
    engine->RegisterEnumValue ("UnitType", "UNIT_FLEET", UNIT_FLEET);
    engine->RegisterEnumValue ("UnitType", "UNIT_TRADERS", UNIT_TRADERS);
    engine->RegisterEnumValue ("UnitType", "UNIT_COLONIZATORS", UNIT_COLONIZATORS);
    engine->RegisterEnumValue ("UnitType", "UNIT_ARMY", UNIT_ARMY);
}
}
