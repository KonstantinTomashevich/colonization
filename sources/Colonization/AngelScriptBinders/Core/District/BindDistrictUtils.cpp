#include <Colonization/BuildConfiguration.hpp>
#include "BindDistrictUtils.hpp"
#include <Urho3D/AngelScript/APITemplates.h>
#include <Urho3D/ThirdParty/AngelScript/angelscript.h>
#include <Colonization/Core/District/DistrictUtils.hpp>

namespace Colonization
{
Urho3D::VariantMap DistrictUtils_CalculateProductionOfFarms (District *district, GameConfiguration *configuration)
{
    return DistrictUtils::CalculateProductionOfFarms (district, configuration).ToVariantMap ();
}

Urho3D::VariantMap DistrictUtils_CalculateProductionOfMines (District *district, GameConfiguration *configuration)
{
    return DistrictUtils::CalculateProductionOfMines (district, configuration).ToVariantMap ();
}

Urho3D::VariantMap DistrictUtils_CalculateProductionOfIndustry (District *district, GameConfiguration *configuration)
{
    return DistrictUtils::CalculateProductionOfIndustry (district, configuration).ToVariantMap ();
}

void BindDistrictUtils (Urho3D::Script *script)
{
    asIScriptEngine *engine = script->GetScriptEngine ();
    engine->RegisterGlobalFunction ("float CalculateDistrictFarmsProductionAmount (District @district, GameConfiguration @configuration)", asFUNCTION (DistrictUtils::CalculateFarmsProductionAmount), asCALL_CDECL);
    engine->RegisterGlobalFunction ("float CalculateDistrictMinesProductionAmount (District @district, GameConfiguration @configuration)", asFUNCTION (DistrictUtils::CalculateMinesProductionAmount), asCALL_CDECL);
    engine->RegisterGlobalFunction ("float CalculateDistrictIndustryProductionAmount (District @district, GameConfiguration @configuration)", asFUNCTION (DistrictUtils::CalculateIndustryProductionAmount), asCALL_CDECL);

    engine->RegisterGlobalFunction ("VariantMap CalculateDistrictProductionOfFarms (District @district, GameConfiguration @configuration)", asFUNCTION (DistrictUtils::CalculateProductionOfFarms), asCALL_CDECL);
    engine->RegisterGlobalFunction ("VariantMap CalculateDistrictProductionOfMines (District @district, GameConfiguration @configuration)", asFUNCTION (DistrictUtils::CalculateProductionOfMines), asCALL_CDECL);
    engine->RegisterGlobalFunction ("VariantMap CalculateDistrictProductionOfIndustry (District @district, GameConfiguration @configuration)", asFUNCTION (DistrictUtils::CalculateProductionOfIndustry), asCALL_CDECL);

    engine->RegisterGlobalFunction ("float CalculateDistrictFarmsProductionRelativePrice (District @district, GameConfiguration @configuration)", asFUNCTION (DistrictUtils::CalculateFarmsProductionRelativePrice), asCALL_CDECL);
    engine->RegisterGlobalFunction ("float CalculateDistrictMinesProductionRelativePrice (District @district, GameConfiguration @configuration)", asFUNCTION (DistrictUtils::CalculateMinesProductionRelativePrice), asCALL_CDECL);
    engine->RegisterGlobalFunction ("float CalculateDistrictIndustryProductionRelativePrice (District @district, GameConfiguration @configuration)", asFUNCTION (DistrictUtils::CalculateIndustryProductionRelativePrice), asCALL_CDECL);

    engine->RegisterGlobalFunction ("float CalculateDistrictFarmsProductionQuality (District @district, GameConfiguration @configuration)", asFUNCTION (DistrictUtils::CalculateFarmsProductionQuality), asCALL_CDECL);
    engine->RegisterGlobalFunction ("float CalculateDistrictMinesProductionQuality (District @district, GameConfiguration @configuration)", asFUNCTION (DistrictUtils::CalculateMinesProductionQuality), asCALL_CDECL);
    engine->RegisterGlobalFunction ("float CalculateDistrictIndustryProductionQuality (District @district, GameConfiguration @configuration)", asFUNCTION (DistrictUtils::CalculateIndustryProductionQuality), asCALL_CDECL);

    engine->RegisterGlobalFunction ("float CalculateDistrictProductionConsumptionOfFarms (District @district, GameConfiguration @configuration)", asFUNCTION (DistrictUtils::CalculateProductionConsumptionOfFarms), asCALL_CDECL);
    engine->RegisterGlobalFunction ("float CalculateDistrictProductionConsumptionOfMines (District @district, GameConfiguration @configuration)", asFUNCTION (DistrictUtils::CalculateProductionConsumptionOfMines), asCALL_CDECL);
    engine->RegisterGlobalFunction ("float CalculateDistrictProductionConsumptionOfIndustry (District @district, GameConfiguration @configuration)", asFUNCTION (DistrictUtils::CalculateProductionConsumptionOfIndustry), asCALL_CDECL);
}
}
