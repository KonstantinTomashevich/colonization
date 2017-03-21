#include <Colonization/BuildConfiguration.hpp>
#include "BindDistrict.hpp"
#include <Urho3D/AngelScript/APITemplates.h>
#include <Urho3D/ThirdParty/AngelScript/angelscript.h>
#include <Colonization/Core/District.hpp>

namespace Colonization
{
Urho3D::CScriptArray *District_GetPolygonPoints (District *district)
{
    return Urho3D::VectorToArray <Urho3D::Vector3> (district->GetPolygonPoints (), "Array<Vector3>");
}

void District_SetPolygonPoints (District *district, Urho3D::CScriptArray *array)
{
    district->SetPolygonPoints (Urho3D::ArrayToPODVector <Urho3D::Vector3> (array));
}

Urho3D::CScriptArray *District_GetNeighborsHashes (District *district)
{
    return Urho3D::VectorToArray <Urho3D::StringHash> (district->GetNeighborsHashes (), "Array<StringHash>");
}

void District_SetNeighborsHashes (District *district, Urho3D::CScriptArray *array)
{
    district->SetNeighborsHashes (Urho3D::ArrayToPODVector <Urho3D::StringHash> (array));
}

void BindDistrict (Urho3D::Script *script)
{
    asIScriptEngine *engine = script->GetScriptEngine ();
    Urho3D::RegisterComponent <District> (engine, "District");
    BindDistrictEnums (script);
    BindDistrictInterface (script, "District");
}

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

void BindDistrictInterface (Urho3D::Script *script, Urho3D::String className)
{
    asIScriptEngine *engine = script->GetScriptEngine ();
    engine->RegisterObjectMethod (className.CString (), "StringHash get_hash () const", asMETHOD (District, GetHash), asCALL_THISCALL);

    engine->RegisterObjectMethod (className.CString (), "bool get_isSea () const", asMETHOD (District, IsSea), asCALL_THISCALL);
    engine->RegisterObjectMethod (className.CString (), "void set_isSea (bool isSea)", asMETHOD (District, SetIsSea), asCALL_THISCALL);

    engine->RegisterObjectMethod (className.CString (), "bool get_isImpassable () const", asMETHOD (District, IsImpassable), asCALL_THISCALL);
    engine->RegisterObjectMethod (className.CString (), "void set_isImpassable (bool isImpassable)", asMETHOD (District, SetIsImpassable), asCALL_THISCALL);

    engine->RegisterObjectMethod (className.CString (), "String get_name () const", asMETHOD (District, GetName), asCALL_THISCALL);
    engine->RegisterObjectMethod (className.CString (), "void set_name (String name)", asMETHOD (District, SetName), asCALL_THISCALL);

    engine->RegisterObjectMethod (className.CString (), "Array <Vector3> @get_polygonPoints () const", asFUNCTION (District_GetPolygonPoints), asCALL_CDECL_OBJFIRST);
    engine->RegisterObjectMethod (className.CString (), "void set_polygonPoints (Array <Vector3> @polygonPoints)", asFUNCTION (District_SetPolygonPoints), asCALL_CDECL_OBJFIRST);

    engine->RegisterObjectMethod (className.CString (), "Vector3 get_unitPosition () const", asMETHOD (District, GetUnitPosition), asCALL_THISCALL);
    engine->RegisterObjectMethod (className.CString (), "void set_unitPosition (Vector3 unitPosition)", asMETHOD (District, SetUnitPosition), asCALL_THISCALL);

    engine->RegisterObjectMethod (className.CString (), "Vector3 get_colonyPosition () const", asMETHOD (District, GetColonyPosition), asCALL_THISCALL);
    engine->RegisterObjectMethod (className.CString (), "void set_colonyPosition (Vector3 colonyPosition)", asMETHOD (District, SetColonyPosition), asCALL_THISCALL);

    engine->RegisterObjectMethod (className.CString (), "bool IsNeighborsWith (StringHash otherDistrict) const", asMETHOD (District, IsNeighborsWith), asCALL_THISCALL);
    engine->RegisterObjectMethod (className.CString (), "Array <StringHash> @get_neighborsHashes () const", asFUNCTION (District_GetNeighborsHashes), asCALL_CDECL_OBJFIRST);
    engine->RegisterObjectMethod (className.CString (), "void set_neighborsHashes (Array <StringHash> @neighborsHashes)", asFUNCTION (District_SetNeighborsHashes), asCALL_CDECL_OBJFIRST);

    engine->RegisterObjectMethod (className.CString (), "float get_farmingSquare () const", asMETHOD (District, GetFarmingSquare), asCALL_THISCALL);
    engine->RegisterObjectMethod (className.CString (), "void set_farmingSquare (float farmingSquare)", asMETHOD (District, SetFarmingSquare), asCALL_THISCALL);

    engine->RegisterObjectMethod (className.CString (), "float get_forestsSquare () const", asMETHOD (District, GetForestsSquare), asCALL_THISCALL);
    engine->RegisterObjectMethod (className.CString (), "void set_forestsSquare (float forestsSquare)", asMETHOD (District, SetForestsSquare), asCALL_THISCALL);

    engine->RegisterObjectMethod (className.CString (), "float get_landAverageFertility () const", asMETHOD (District, GetLandAverageFertility), asCALL_THISCALL);
    engine->RegisterObjectMethod (className.CString (), "void set_landAverageFertility (float landAverageFertility)", asMETHOD (District, SetLandAverageFertility), asCALL_THISCALL);

    engine->RegisterObjectMethod (className.CString (), "ClimateType get_climate () const", asMETHOD (District, GetClimate), asCALL_THISCALL);
    engine->RegisterObjectMethod (className.CString (), "void set_climate (ClimateType climate)", asMETHOD (District, SetClimate), asCALL_THISCALL);

    engine->RegisterObjectMethod (className.CString (), "float get_forestsReproductivity () const", asMETHOD (District, GetForestsReproductivity), asCALL_THISCALL);
    engine->RegisterObjectMethod (className.CString (), "void set_forestsReproductivity (float forestsReproductivity)", asMETHOD (District, SetForestsReproductivity), asCALL_THISCALL);

    engine->RegisterObjectMethod (className.CString (), "bool get_hasCoalDeposits () const", asMETHOD (District, HasCoalDeposits), asCALL_THISCALL);
    engine->RegisterObjectMethod (className.CString (), "void set_hasCoalDeposits (bool hasCoalDeposits)", asMETHOD (District, SetCoalDeposits), asCALL_THISCALL);

    engine->RegisterObjectMethod (className.CString (), "bool get_hasIronDeposits () const", asMETHOD (District, HasIronDeposits), asCALL_THISCALL);
    engine->RegisterObjectMethod (className.CString (), "void set_hasIronDeposits (bool hasIronDeposits)", asMETHOD (District, SetIronDeposits), asCALL_THISCALL);

    engine->RegisterObjectMethod (className.CString (), "bool get_hasSilverDeposits () const", asMETHOD (District, HasSilverDeposits), asCALL_THISCALL);
    engine->RegisterObjectMethod (className.CString (), "void set_hasSilverDeposits (bool hasSilverDeposits)", asMETHOD (District, SetSilverDeposits), asCALL_THISCALL);

    engine->RegisterObjectMethod (className.CString (), "bool get_hasGoldDeposits () const", asMETHOD (District, HasGoldDeposits), asCALL_THISCALL);
    engine->RegisterObjectMethod (className.CString (), "void set_hasGoldDeposits (bool hasGoldDeposits)", asMETHOD (District, SetGoldDeposits), asCALL_THISCALL);

    engine->RegisterObjectMethod (className.CString (), "float get_nativesCount () const", asMETHOD (District, GetNativesCount), asCALL_THISCALL);
    engine->RegisterObjectMethod (className.CString (), "void set_nativesCount (float nativesCount)", asMETHOD (District, SetNativesCount), asCALL_THISCALL);

    engine->RegisterObjectMethod (className.CString (), "float get_nativesFightingTechnologyLevel () const", asMETHOD (District, GetNativesFightingTechnologyLevel), asCALL_THISCALL);
    engine->RegisterObjectMethod (className.CString (), "void set_nativesFightingTechnologyLevel (float nativesFightingTechnologyLevel)", asMETHOD (District, SetNativesFightingTechnologyLevel), asCALL_THISCALL);

    engine->RegisterObjectMethod (className.CString (), "float get_nativesAggressiveness () const", asMETHOD (District, GetNativesAggressiveness), asCALL_THISCALL);
    engine->RegisterObjectMethod (className.CString (), "void set_nativesAggressiveness (float nativesAggressiveness)", asMETHOD (District, SetNativesAggressiveness), asCALL_THISCALL);

    engine->RegisterObjectMethod (className.CString (), "NativesCharacterType get_nativesCharacter () const", asMETHOD (District, GetNativesCharacter), asCALL_THISCALL);
    engine->RegisterObjectMethod (className.CString (), "void set_nativesCharacter (NativesCharacterType nativesCharacter)", asMETHOD (District, SetNativesCharacter), asCALL_THISCALL);

    engine->RegisterObjectMethod (className.CString (), "bool get_hasColony () const", asMETHOD (District, HasColony), asCALL_THISCALL);
    engine->RegisterObjectMethod (className.CString (), "void set_hasColony (bool hasColony)", asMETHOD (District, SetColony), asCALL_THISCALL);

    engine->RegisterObjectMethod (className.CString (), "String get_colonyOwnerName () const", asMETHOD (District, GetColonyOwnerName), asCALL_THISCALL);
    engine->RegisterObjectMethod (className.CString (), "void set_colonyOwnerName (String colonyOwnerName)", asMETHOD (District, SetColonyOwnerName), asCALL_THISCALL);

    engine->RegisterObjectMethod (className.CString (), "float get_menCount () const", asMETHOD (District, GetMenCount), asCALL_THISCALL);
    engine->RegisterObjectMethod (className.CString (), "void set_menCount (float menCount)", asMETHOD (District, SetMenCount), asCALL_THISCALL);

    engine->RegisterObjectMethod (className.CString (), "float get_womenCount () const", asMETHOD (District, GetWomenCount), asCALL_THISCALL);
    engine->RegisterObjectMethod (className.CString (), "void set_womenCount (float womenCount)", asMETHOD (District, SetWomenCount), asCALL_THISCALL);

    engine->RegisterObjectMethod (className.CString (), "float get_farmsEvolutionPoints () const", asMETHOD (District, GetFarmsEvolutionPoints), asCALL_THISCALL);
    engine->RegisterObjectMethod (className.CString (), "void set_farmsEvolutionPoints (float farmsEvolutionPoints)", asMETHOD (District, SetFarmsEvolutionPoints), asCALL_THISCALL);

    engine->RegisterObjectMethod (className.CString (), "float get_minesEvolutionPoints () const", asMETHOD (District, GetMinesEvolutionPoints), asCALL_THISCALL);
    engine->RegisterObjectMethod (className.CString (), "void set_minesEvolutionPoints (float minesEvolutionPoints)", asMETHOD (District, SetMinesEvolutionPoints), asCALL_THISCALL);

    engine->RegisterObjectMethod (className.CString (), "float get_industryEvolutionPoints () const", asMETHOD (District, GetIndustryEvolutionPoints), asCALL_THISCALL);
    engine->RegisterObjectMethod (className.CString (), "void set_industryEvolutionPoints (float industryEvolutionPoints)", asMETHOD (District, SetIndustryEvolutionPoints), asCALL_THISCALL);

    engine->RegisterObjectMethod (className.CString (), "float get_logisticsEvolutionPoints () const", asMETHOD (District, GetLogisticsEvolutionPoints), asCALL_THISCALL);
    engine->RegisterObjectMethod (className.CString (), "void set_logisticsEvolutionPoints (float logisticsEvolutionPoints)", asMETHOD (District, SetLogisticsEvolutionPoints), asCALL_THISCALL);

    engine->RegisterObjectMethod (className.CString (), "float get_defenseEvolutionPoints () const", asMETHOD (District, GetDefenseEvolutionPoints), asCALL_THISCALL);
    engine->RegisterObjectMethod (className.CString (), "void set_defenseEvolutionPoints (float defenseEvolutionPoints)", asMETHOD (District, SetDefenseEvolutionPoints), asCALL_THISCALL);

    engine->RegisterObjectMethod (className.CString (), "float get_averageLevelOfLifePoints () const", asMETHOD (District, GetAverageLevelOfLifePoints), asCALL_THISCALL);
    engine->RegisterObjectMethod (className.CString (), "void set_averageLevelOfLifePoints (float averageLevelOfLifePoints)", asMETHOD (District, SetAverageLevelOfLifePoints), asCALL_THISCALL);
}
}
