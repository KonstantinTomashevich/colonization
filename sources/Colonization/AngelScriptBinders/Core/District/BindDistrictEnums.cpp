#include <Colonization/BuildConfiguration.hpp>
#include "BindDistrictEnums.hpp"
#include <Urho3D/AngelScript/APITemplates.h>
#include <Urho3D/ThirdParty/AngelScript/angelscript.h>
#include <Colonization/Core/District/DistrictEnums.hpp>

namespace Colonization
{
void BindDistrictEnums (Urho3D::Script *script)
{
    asIScriptEngine *engine = script->GetScriptEngine ();
    engine->RegisterEnum ("ClimateType");
    engine->RegisterEnumValue ("ClimateType", "CLIMATE_TROPICAL", CLIMATE_TROPICAL);
    engine->RegisterEnumValue ("ClimateType", "CLIMATE_HOT", CLIMATE_HOT );
    engine->RegisterEnumValue ("ClimateType", "CLIMATE_TEMPERATE", CLIMATE_TEMPERATE);
    engine->RegisterEnumValue ("ClimateType", "CLIMATE_TEMPERATE_CONTINENTAL", CLIMATE_TEMPERATE_CONTINENTAL);
    engine->RegisterEnumValue ("ClimateType", "CLIMATE_DESERT", CLIMATE_DESERT);
    engine->RegisterEnumValue ("ClimateType", "CLIMATE_COLD", CLIMATE_COLD);

    engine->RegisterEnum ("NativesCharacterType");
    engine->RegisterEnumValue ("NativesCharacterType", "NATIVES_CHARACTER_FRIENDLY", NATIVES_CHARACTER_FRIENDLY);
    engine->RegisterEnumValue ("NativesCharacterType", "NATIVES_CHARACTER_MEDIUM", NATIVES_CHARACTER_MEDIUM);
    engine->RegisterEnumValue ("NativesCharacterType", "NATIVES_CHARACTER_COLD", NATIVES_CHARACTER_COLD);
    engine->RegisterEnumValue ("NativesCharacterType", "NATIVES_CHARACTER_ISOLATIONIST", NATIVES_CHARACTER_ISOLATIONIST);
    engine->RegisterEnumValue ("NativesCharacterType", "NATIVES_CHARATER_AGGRESSIVE", NATIVES_CHARATER_AGGRESSIVE);
}
}
