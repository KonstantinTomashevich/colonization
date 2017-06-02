#include <Colonization/BuildConfiguration.hpp>
#include "BindDistrict.hpp"
#include <Urho3D/AngelScript/APITemplates.h>
#include <Urho3D/ThirdParty/AngelScript/angelscript.h>
#include <Colonization/Core/District/District.hpp>

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

Urho3D::StringHash District_GetColonyActionTypeByIndex (District *district, int index)
{
    return district->GetColonyActionByIndex (index).first_;
}

Urho3D::VariantMap District_GetColonyActionDataByIndex (District *district, int index)
{
    return district->GetColonyActionByIndex (index).second_;
}

Urho3D::StringHash District_GetColonyActionTypeById (District *district, Urho3D::StringHash id)
{
    return district->GetColonyActionById (id).first_;
}

Urho3D::VariantMap District_GetColonyActionDataById (District *district, Urho3D::StringHash id)
{
    return district->GetColonyActionById (id).second_;
}

void BindDistrict (Urho3D::Script *script)
{
    asIScriptEngine *engine = script->GetScriptEngine ();
    Urho3D::RegisterComponent <District> (engine, "District");

    engine->RegisterObjectMethod ("District", "StringHash get_hash () const", asMETHOD (District, GetHash), asCALL_THISCALL);
    engine->RegisterObjectMethod ("District", "void AddColonyAction (StringHash actionType, VariantMap actionData)", asMETHOD (District, AddColonyAction), asCALL_THISCALL);
    engine->RegisterObjectMethod ("District", "int get_colonyActionsCount () const", asMETHOD (District, GetColonyActionsCount), asCALL_THISCALL);
    engine->RegisterObjectMethod ("District", "StringHash GetColonyActionTypeByIndex (int index) const", asFUNCTION (District_GetColonyActionTypeByIndex), asCALL_CDECL_OBJFIRST);
    engine->RegisterObjectMethod ("District", "VariantMap GetColonyActionDataByIndex (int index) const", asFUNCTION (District_GetColonyActionDataByIndex), asCALL_CDECL_OBJFIRST);

    engine->RegisterObjectMethod ("District", "StringHash GetColonyActionTypeById (StringHash id) const", asFUNCTION (District_GetColonyActionTypeById), asCALL_CDECL_OBJFIRST);
    engine->RegisterObjectMethod ("District", "VariantMap GetColonyActionDataById (StringHash id) const", asFUNCTION (District_GetColonyActionDataById), asCALL_CDECL_OBJFIRST);
    engine->RegisterObjectMethod ("District", "bool SetColonyActionAtIndexData (int index, VariantMap actionData)", asMETHOD (District, SetColonyActionAtIndexData), asCALL_THISCALL);
    engine->RegisterObjectMethod ("District", "bool SetColonyActionWithIdData (VariantMap actionData)", asMETHOD (District, SetColonyActionWithIdData), asCALL_THISCALL);
    engine->RegisterObjectMethod ("District", "void RemoveColonyActionByIndex (int index)", asMETHOD (District, RemoveColonyActionByIndex), asCALL_THISCALL);
    engine->RegisterObjectMethod ("District", "bool RemoveColonyActionById (StringHash id)", asMETHOD (District, RemoveColonyActionById), asCALL_THISCALL);

    engine->RegisterObjectMethod ("District", "bool get_isSea () const", asMETHOD (District, GetIsSea), asCALL_THISCALL);
    engine->RegisterObjectMethod ("District", "void set_isSea (bool isSea)", asMETHOD (District, SetIsSea), asCALL_THISCALL);

    engine->RegisterObjectMethod ("District", "bool get_isImpassable () const", asMETHOD (District, GetIsImpassable), asCALL_THISCALL);
    engine->RegisterObjectMethod ("District", "void set_isImpassable (bool isImpassable)", asMETHOD (District, SetIsImpassable), asCALL_THISCALL);

    engine->RegisterObjectMethod ("District", "String get_name () const", asMETHOD (District, GetName), asCALL_THISCALL);
    engine->RegisterObjectMethod ("District", "void set_name (String name)", asMETHOD (District, SetName), asCALL_THISCALL);

    engine->RegisterObjectMethod ("District", "Array <Vector3> @get_polygonPoints () const", asFUNCTION (District_GetPolygonPoints), asCALL_CDECL_OBJFIRST);
    engine->RegisterObjectMethod ("District", "void set_polygonPoints (Array <Vector3> @polygonPoints)", asFUNCTION (District_SetPolygonPoints), asCALL_CDECL_OBJFIRST);

    engine->RegisterObjectMethod ("District", "Vector3 get_unitPosition () const", asMETHOD (District, GetUnitPosition), asCALL_THISCALL);
    engine->RegisterObjectMethod ("District", "void set_unitPosition (Vector3 unitPosition)", asMETHOD (District, SetUnitPosition), asCALL_THISCALL);

    engine->RegisterObjectMethod ("District", "Vector3 get_colonyPosition () const", asMETHOD (District, GetColonyPosition), asCALL_THISCALL);
    engine->RegisterObjectMethod ("District", "void set_colonyPosition (Vector3 colonyPosition)", asMETHOD (District, SetColonyPosition), asCALL_THISCALL);

    engine->RegisterObjectMethod ("District", "bool IsNeighborsWith (StringHash otherDistrict) const", asMETHOD (District, IsNeighborsWith), asCALL_THISCALL);
    engine->RegisterObjectMethod ("District", "Array <StringHash> @get_neighborsHashes () const", asFUNCTION (District_GetNeighborsHashes), asCALL_CDECL_OBJFIRST);
    engine->RegisterObjectMethod ("District", "void set_neighborsHashes (Array <StringHash> @neighborsHashes)", asFUNCTION (District_SetNeighborsHashes), asCALL_CDECL_OBJFIRST);

    engine->RegisterObjectMethod ("District", "float get_farmingSquare () const", asMETHOD (District, GetFarmingSquare), asCALL_THISCALL);
    engine->RegisterObjectMethod ("District", "void set_farmingSquare (float farmingSquare)", asMETHOD (District, SetFarmingSquare), asCALL_THISCALL);

    engine->RegisterObjectMethod ("District", "float get_forestsSquare () const", asMETHOD (District, GetForestsSquare), asCALL_THISCALL);
    engine->RegisterObjectMethod ("District", "void set_forestsSquare (float forestsSquare)", asMETHOD (District, SetForestsSquare), asCALL_THISCALL);

    engine->RegisterObjectMethod ("District", "float get_landAverageFertility () const", asMETHOD (District, GetLandAverageFertility), asCALL_THISCALL);
    engine->RegisterObjectMethod ("District", "void set_landAverageFertility (float landAverageFertility)", asMETHOD (District, SetLandAverageFertility), asCALL_THISCALL);

    engine->RegisterObjectMethod ("District", "ClimateType get_climate () const", asMETHOD (District, GetClimate), asCALL_THISCALL);
    engine->RegisterObjectMethod ("District", "void set_climate (ClimateType climate)", asMETHOD (District, SetClimate), asCALL_THISCALL);

    engine->RegisterObjectMethod ("District", "bool get_hasCoalDeposits () const", asMETHOD (District, GetHasCoalDeposits), asCALL_THISCALL);
    engine->RegisterObjectMethod ("District", "void set_hasCoalDeposits (bool hasCoalDeposits)", asMETHOD (District, SetHasCoalDeposits), asCALL_THISCALL);

    engine->RegisterObjectMethod ("District", "bool get_hasIronDeposits () const", asMETHOD (District, GetHasIronDeposits), asCALL_THISCALL);
    engine->RegisterObjectMethod ("District", "void set_hasIronDeposits (bool hasIronDeposits)", asMETHOD (District, SetHasIronDeposits), asCALL_THISCALL);

    engine->RegisterObjectMethod ("District", "bool get_hasSilverDeposits () const", asMETHOD (District, GetHasSilverDeposits), asCALL_THISCALL);
    engine->RegisterObjectMethod ("District", "void set_hasSilverDeposits (bool hasSilverDeposits)", asMETHOD (District, SetHasSilverDeposits), asCALL_THISCALL);

    engine->RegisterObjectMethod ("District", "bool get_hasGoldDeposits () const", asMETHOD (District, GetHasGoldDeposits), asCALL_THISCALL);
    engine->RegisterObjectMethod ("District", "void set_hasGoldDeposits (bool hasGoldDeposits)", asMETHOD (District, SetHasGoldDeposits), asCALL_THISCALL);

    engine->RegisterObjectMethod ("District", "float get_nativesCount () const", asMETHOD (District, GetNativesCount), asCALL_THISCALL);
    engine->RegisterObjectMethod ("District", "void set_nativesCount (float nativesCount)", asMETHOD (District, SetNativesCount), asCALL_THISCALL);

    engine->RegisterObjectMethod ("District", "float get_nativesFightingTechnologyLevel () const", asMETHOD (District, GetNativesFightingTechnologyLevel), asCALL_THISCALL);
    engine->RegisterObjectMethod ("District", "void set_nativesFightingTechnologyLevel (float nativesFightingTechnologyLevel)", asMETHOD (District, SetNativesFightingTechnologyLevel), asCALL_THISCALL);

    engine->RegisterObjectMethod ("District", "float get_nativesAggressiveness () const", asMETHOD (District, GetNativesAggressiveness), asCALL_THISCALL);
    engine->RegisterObjectMethod ("District", "void set_nativesAggressiveness (float nativesAggressiveness)", asMETHOD (District, SetNativesAggressiveness), asCALL_THISCALL);

    engine->RegisterObjectMethod ("District", "NativesCharacterType get_nativesCharacter () const", asMETHOD (District, GetNativesCharacter), asCALL_THISCALL);
    engine->RegisterObjectMethod ("District", "void set_nativesCharacter (NativesCharacterType nativesCharacter)", asMETHOD (District, SetNativesCharacter), asCALL_THISCALL);

    engine->RegisterObjectMethod ("District", "bool get_hasColony () const", asMETHOD (District, GetHasColony), asCALL_THISCALL);
    engine->RegisterObjectMethod ("District", "void set_hasColony (bool hasColony)", asMETHOD (District, SetHasColony), asCALL_THISCALL);

    engine->RegisterObjectMethod ("District", "String get_colonyOwnerName () const", asMETHOD (District, GetColonyOwnerName), asCALL_THISCALL);
    engine->RegisterObjectMethod ("District", "void set_colonyOwnerName (String colonyOwnerName)", asMETHOD (District, SetColonyOwnerName), asCALL_THISCALL);

    engine->RegisterObjectMethod ("District", "float get_menCount () const", asMETHOD (District, GetMenCount), asCALL_THISCALL);
    engine->RegisterObjectMethod ("District", "void set_menCount (float menCount)", asMETHOD (District, SetMenCount), asCALL_THISCALL);

    engine->RegisterObjectMethod ("District", "float get_womenCount () const", asMETHOD (District, GetWomenCount), asCALL_THISCALL);
    engine->RegisterObjectMethod ("District", "void set_womenCount (float womenCount)", asMETHOD (District, SetWomenCount), asCALL_THISCALL);

    engine->RegisterObjectMethod ("District", "float get_farmsEvolutionPoints () const", asMETHOD (District, GetFarmsEvolutionPoints), asCALL_THISCALL);
    engine->RegisterObjectMethod ("District", "void set_farmsEvolutionPoints (float farmsEvolutionPoints)", asMETHOD (District, SetFarmsEvolutionPoints), asCALL_THISCALL);

    engine->RegisterObjectMethod ("District", "float get_minesEvolutionPoints () const", asMETHOD (District, GetMinesEvolutionPoints), asCALL_THISCALL);
    engine->RegisterObjectMethod ("District", "void set_minesEvolutionPoints (float minesEvolutionPoints)", asMETHOD (District, SetMinesEvolutionPoints), asCALL_THISCALL);

    engine->RegisterObjectMethod ("District", "float get_industryEvolutionPoints () const", asMETHOD (District, GetIndustryEvolutionPoints), asCALL_THISCALL);
    engine->RegisterObjectMethod ("District", "void set_industryEvolutionPoints (float industryEvolutionPoints)", asMETHOD (District, SetIndustryEvolutionPoints), asCALL_THISCALL);

    engine->RegisterObjectMethod ("District", "float get_logisticsEvolutionPoints () const", asMETHOD (District, GetLogisticsEvolutionPoints), asCALL_THISCALL);
    engine->RegisterObjectMethod ("District", "void set_logisticsEvolutionPoints (float logisticsEvolutionPoints)", asMETHOD (District, SetLogisticsEvolutionPoints), asCALL_THISCALL);

    engine->RegisterObjectMethod ("District", "float get_defenseEvolutionPoints () const", asMETHOD (District, GetDefenseEvolutionPoints), asCALL_THISCALL);
    engine->RegisterObjectMethod ("District", "void set_defenseEvolutionPoints (float defenseEvolutionPoints)", asMETHOD (District, SetDefenseEvolutionPoints), asCALL_THISCALL);

    engine->RegisterObjectMethod ("District", "float get_farmsBalance () const", asMETHOD (District, GetFarmsBalance), asCALL_THISCALL);
    engine->RegisterObjectMethod ("District", "void set_farmsBalance (float farmsBalance)", asMETHOD (District, SetFarmsBalance), asCALL_THISCALL);

    engine->RegisterObjectMethod ("District", "float get_minesBalance () const", asMETHOD (District, GetMinesBalance), asCALL_THISCALL);
    engine->RegisterObjectMethod ("District", "void set_minesBalance (float minesBalance)", asMETHOD (District, SetMinesBalance), asCALL_THISCALL);

    engine->RegisterObjectMethod ("District", "float get_industryBalance () const", asMETHOD (District, GetIndustryBalance), asCALL_THISCALL);
    engine->RegisterObjectMethod ("District", "void set_industryBalance (float industryBalance)", asMETHOD (District, SetIndustryBalance), asCALL_THISCALL);

    engine->RegisterObjectMethod ("District", "float get_logisticsBalance () const", asMETHOD (District, GetLogisticsBalance), asCALL_THISCALL);
    engine->RegisterObjectMethod ("District", "void set_logisticsBalance (float logisticsBalance)", asMETHOD (District, SetLogisticsBalance), asCALL_THISCALL);

    engine->RegisterObjectMethod ("District", "float get_defenseBalance () const", asMETHOD (District, GetDefenseBalance), asCALL_THISCALL);
    engine->RegisterObjectMethod ("District", "void set_defenseBalance (float defenseBalance)", asMETHOD (District, SetDefenseBalance), asCALL_THISCALL);

    engine->RegisterObjectMethod ("District", "float get_averageLevelOfLifePoints () const", asMETHOD (District, GetAverageLevelOfLifePoints), asCALL_THISCALL);
    engine->RegisterObjectMethod ("District", "void set_averageLevelOfLifePoints (float averageLevelOfLifePoints)", asMETHOD (District, SetAverageLevelOfLifePoints), asCALL_THISCALL);

    engine->RegisterObjectMethod ("District", "VariantMap get_lastTradeFarmsProduction () const", asMETHOD (District, GetLastTradeFarmsProduction), asCALL_THISCALL);
    engine->RegisterObjectMethod ("District", "void set_lastTradeFarmsProduction (VariantMap lastTradeFarmsProduction)", asMETHOD (District, SetLastTradeFarmsProduction), asCALL_THISCALL);

    engine->RegisterObjectMethod ("District", "VariantMap get_lastTradeMinesProduction () const", asMETHOD (District, GetLastTradeMinesProduction), asCALL_THISCALL);
    engine->RegisterObjectMethod ("District", "void set_lastTradeMinesProduction (VariantMap lastTradeMinesProduction)", asMETHOD (District, SetLastTradeMinesProduction), asCALL_THISCALL);

    engine->RegisterObjectMethod ("District", "VariantMap get_lastTradeIndustryProduction () const", asMETHOD (District, GetLastTradeIndustryProduction), asCALL_THISCALL);
    engine->RegisterObjectMethod ("District", "void set_lastTradeIndustryProduction (VariantMap lastTradeIndustryProduction)", asMETHOD (District, SetLastTradeIndustryProduction), asCALL_THISCALL);

    engine->RegisterObjectMethod ("District", "float get_lastTradeFarmsBalanceAddition () const", asMETHOD (District, GetLastTradeFarmsBalanceAddition), asCALL_THISCALL);
    engine->RegisterObjectMethod ("District", "void set_lastTradeFarmsBalanceAddition (float lastTradeFarmsBalanceAddition)", asMETHOD (District, SetLastTradeFarmsBalanceAddition), asCALL_THISCALL);

    engine->RegisterObjectMethod ("District", "float get_lastTradeMinesBalanceAddition () const", asMETHOD (District, GetLastTradeMinesBalanceAddition), asCALL_THISCALL);
    engine->RegisterObjectMethod ("District", "void set_lastTradeMinesBalanceAddition (float lastTradeMinesBalanceAddition)", asMETHOD (District, SetLastTradeMinesBalanceAddition), asCALL_THISCALL);

    engine->RegisterObjectMethod ("District", "float get_lastTradeIndustryBalanceAddition () const", asMETHOD (District, GetLastTradeIndustryBalanceAddition), asCALL_THISCALL);
    engine->RegisterObjectMethod ("District", "void set_lastTradeIndustryBalanceAddition (float lastTradeIndustryBalanceAddition)", asMETHOD (District, SetLastTradeIndustryBalanceAddition), asCALL_THISCALL);

    engine->RegisterObjectMethod ("District", "float get_lastTradeLogisticsBalanceAddition () const", asMETHOD (District, GetLastTradeLogisticsBalanceAddition), asCALL_THISCALL);
    engine->RegisterObjectMethod ("District", "void set_lastTradeLogisticsBalanceAddition (float lastTradeLogisticsBalanceAddition)", asMETHOD (District, SetLastTradeLogisticsBalanceAddition), asCALL_THISCALL);

    engine->RegisterObjectMethod ("District", "float get_lastTradeDefenseBalanceAddition () const", asMETHOD (District, GetLastTradeDefenseBalanceAddition), asCALL_THISCALL);
    engine->RegisterObjectMethod ("District", "void set_lastTradeDefenseBalanceAddition (float lastTradeDefenseBalanceAddition)", asMETHOD (District, SetLastTradeDefenseBalanceAddition), asCALL_THISCALL);
}
}
