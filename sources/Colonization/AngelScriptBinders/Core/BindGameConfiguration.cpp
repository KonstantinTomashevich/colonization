#include <Colonization/BuildConfiguration.hpp>
#include "BindGameConfiguration.hpp"
#include <Urho3D/AngelScript/APITemplates.h>
#include <Urho3D/ThirdParty/AngelScript/angelscript.h>
#include <Colonization/Core/GameConfiguration.hpp>

namespace Colonization
{
Urho3D::CScriptArray *GameConfiguration_GetWayToEuropeDistricts (GameConfiguration *object)
{
    Urho3D::PODVector <Urho3D::StringHash> array = object->GetWayToEuropeDistricts ();
    return Urho3D::VectorToArray <Urho3D::StringHash> (array, "Array<StringHash>");
}

void GameConfiguration_SetWayToEuropeDistricts (GameConfiguration *object, Urho3D::CScriptArray *array)
{
    object->SetWayToEuropeDistricts (Urho3D::ArrayToPODVector <Urho3D::StringHash> (array));
}

void BindGameConfiguration (Urho3D::Script *script)
{
    asIScriptEngine *engine = script->GetScriptEngine ();
    Urho3D::RegisterComponent <GameConfiguration> (engine, "GameConfiguration");
    BindGameConfigurationInterface (script, "GameConfiguration");
}

void BindGameConfigurationInterface (Urho3D::Script *script, Urho3D::String className)
{
    asIScriptEngine *engine = script->GetScriptEngine ();
    engine->RegisterObjectMethod (className.CString (), "Array <StringHash> @get_wayToEuropeDistricts ()", asFUNCTION (GameConfiguration_GetWayToEuropeDistricts), asCALL_CDECL_OBJFIRST);
    engine->RegisterObjectMethod (className.CString (), "void set_wayToEuropeDistricts (Array <StringHash> @wayToEuropeDistricts)", asFUNCTION (GameConfiguration_SetWayToEuropeDistricts), asCALL_CDECL_OBJFIRST);

    engine->RegisterObjectMethod (className.CString (), "float get_sailSpeed () const", asMETHOD (GameConfiguration, GetSailSpeed), asCALL_THISCALL);
    engine->RegisterObjectMethod (className.CString (), "void set_sailSpeed (float sailSpeed)", asMETHOD (GameConfiguration, SetSailSpeed), asCALL_THISCALL);

    engine->RegisterObjectMethod (className.CString (), "float get_marchSpeed () const", asMETHOD (GameConfiguration, GetMarchSpeed), asCALL_THISCALL);
    engine->RegisterObjectMethod (className.CString (), "void set_marchSpeed (float marchSpeed)", asMETHOD (GameConfiguration, SetMarchSpeed), asCALL_THISCALL);

    engine->RegisterObjectMethod (className.CString (), "float get_embarkationSpeed () const", asMETHOD (GameConfiguration, GetEmbarkationSpeed), asCALL_THISCALL);
    engine->RegisterObjectMethod (className.CString (), "void set_embarkationSpeed (float embarkationSpeed)", asMETHOD (GameConfiguration, SetEmbarkationSpeed), asCALL_THISCALL);

    engine->RegisterObjectMethod (className.CString (), "float get_disembarkationSpeed () const", asMETHOD (GameConfiguration, GetDisembarkationSpeed), asCALL_THISCALL);
    engine->RegisterObjectMethod (className.CString (), "void set_disembarkationSpeed (float disembarkationSpeed)", asMETHOD (GameConfiguration, SetDisembarkationSpeed), asCALL_THISCALL);

    engine->RegisterObjectMethod (className.CString (), "float get_oneColonistFarmsProductionConsumption () const", asMETHOD (GameConfiguration, GetOneColonistFarmsProductionConsumption), asCALL_THISCALL);
    engine->RegisterObjectMethod (className.CString (), "void set_oneColonistFarmsProductionConsumption (float oneColonistFarmsProductionConsumption)", asMETHOD (GameConfiguration, SetOneColonistFarmsProductionConsumption), asCALL_THISCALL);

    engine->RegisterObjectMethod (className.CString (), "float get_oneSoldierFarmsProductionConsumption () const", asMETHOD (GameConfiguration, GetOneSoldierFarmsProductionConsumption), asCALL_THISCALL);
    engine->RegisterObjectMethod (className.CString (), "void set_oneSoldierFarmsProductionConsumption (float oneSoldierFarmsProductionConsumption)", asMETHOD (GameConfiguration, SetOneSoldierFarmsProductionConsumption), asCALL_THISCALL);

    engine->RegisterObjectMethod (className.CString (), "float get_farmsProductionMinesConsumption () const", asMETHOD (GameConfiguration, GetFarmsProductionMinesConsumption), asCALL_THISCALL);
    engine->RegisterObjectMethod (className.CString (), "void set_farmsProductionMinesConsumption (float farmsProductionMinesConsumption)", asMETHOD (GameConfiguration, SetFarmsProductionMinesConsumption), asCALL_THISCALL);

    engine->RegisterObjectMethod (className.CString (), "float get_farmsProductionIndustryConsumption () const", asMETHOD (GameConfiguration, GetFarmsProductionIndustryConsumption), asCALL_THISCALL);
    engine->RegisterObjectMethod (className.CString (), "void set_farmsProductionIndustryConsumption (float farmsProductionIndustryConsumption)", asMETHOD (GameConfiguration, SetFarmsProductionIndustryConsumption), asCALL_THISCALL);

    engine->RegisterObjectMethod (className.CString (), "float get_oneColonistMinesProductionConsumption () const", asMETHOD (GameConfiguration, GetOneColonistMinesProductionConsumption), asCALL_THISCALL);
    engine->RegisterObjectMethod (className.CString (), "void set_oneColonistMinesProductionConsumption (float oneColonistMinesProductionConsumption)", asMETHOD (GameConfiguration, SetOneColonistMinesProductionConsumption), asCALL_THISCALL);

    engine->RegisterObjectMethod (className.CString (), "float get_oneSoldierMinesProductionConsumption () const", asMETHOD (GameConfiguration, GetOneSoldierMinesProductionConsumption), asCALL_THISCALL);
    engine->RegisterObjectMethod (className.CString (), "void set_oneSoldierMinesProductionConsumption (float oneSoldierMinesProductionConsumption)", asMETHOD (GameConfiguration, SetOneSoldierMinesProductionConsumption), asCALL_THISCALL);

    engine->RegisterObjectMethod (className.CString (), "float get_minesProductionFarmsConsumption () const", asMETHOD (GameConfiguration, GetMinesProductionFarmsConsumption), asCALL_THISCALL);
    engine->RegisterObjectMethod (className.CString (), "void set_minesProductionFarmsConsumption (float minesProductionFarmsConsumption)", asMETHOD (GameConfiguration, SetMinesProductionFarmsConsumption), asCALL_THISCALL);

    engine->RegisterObjectMethod (className.CString (), "float get_minesProductionIndustryConsumption () const", asMETHOD (GameConfiguration, GetMinesProductionIndustryConsumption), asCALL_THISCALL);
    engine->RegisterObjectMethod (className.CString (), "void set_minesProductionIndustryConsumption (float minesProductionIndustryConsumption)", asMETHOD (GameConfiguration, SetMinesProductionIndustryConsumption), asCALL_THISCALL);

    engine->RegisterObjectMethod (className.CString (), "float get_oneColonistIndustryProductionConsumption () const", asMETHOD (GameConfiguration, GetOneColonistIndustryProductionConsumption), asCALL_THISCALL);
    engine->RegisterObjectMethod (className.CString (), "void set_oneColonistIndustryProductionConsumption (float oneColonistIndustryProductionConsumption)", asMETHOD (GameConfiguration, SetOneColonistIndustryProductionConsumption), asCALL_THISCALL);

    engine->RegisterObjectMethod (className.CString (), "float get_oneSoldierIndustryProductionConsumption () const", asMETHOD (GameConfiguration, GetOneSoldierIndustryProductionConsumption), asCALL_THISCALL);
    engine->RegisterObjectMethod (className.CString (), "void set_oneSoldierIndustryProductionConsumption (float oneSoldierIndustryProductionConsumption)", asMETHOD (GameConfiguration, SetOneSoldierIndustryProductionConsumption), asCALL_THISCALL);

    engine->RegisterObjectMethod (className.CString (), "float get_industryProductionMinesConsumption () const", asMETHOD (GameConfiguration, GetIndustryProductionMinesConsumption), asCALL_THISCALL);
    engine->RegisterObjectMethod (className.CString (), "void set_industryProductionMinesConsumption (float industryProductionMinesConsumption)", asMETHOD (GameConfiguration, SetIndustryProductionMinesConsumption), asCALL_THISCALL);

    engine->RegisterObjectMethod (className.CString (), "float get_industryProductionFarmsConsumption () const", asMETHOD (GameConfiguration, GetIndustryProductionFarmsConsumption), asCALL_THISCALL);
    engine->RegisterObjectMethod (className.CString (), "void set_industryProductionFarmsConsumption (float industryProductionFarmsConsumption)", asMETHOD (GameConfiguration, SetIndustryProductionFarmsConsumption), asCALL_THISCALL);

    engine->RegisterObjectMethod (className.CString (), "float get_farmsProductionPerColonist () const", asMETHOD (GameConfiguration, GetFarmsProductionPerColonist), asCALL_THISCALL);
    engine->RegisterObjectMethod (className.CString (), "void set_farmsProductionPerColonist (float farmsProductionPerColonist)", asMETHOD (GameConfiguration, SetFarmsProductionPerColonist), asCALL_THISCALL);

    engine->RegisterObjectMethod (className.CString (), "float get_minesProductionPerColonist () const", asMETHOD (GameConfiguration, GetMinesProductionPerColonist), asCALL_THISCALL);
    engine->RegisterObjectMethod (className.CString (), "void set_minesProductionPerColonist (float minesProductionPerColonist)", asMETHOD (GameConfiguration, SetMinesProductionPerColonist), asCALL_THISCALL);

    engine->RegisterObjectMethod (className.CString (), "float get_industryProductionPerColonist () const", asMETHOD (GameConfiguration, GetIndustryProductionPerColonist), asCALL_THISCALL);
    engine->RegisterObjectMethod (className.CString (), "void set_industryProductionPerColonist (float industryProductionPerColonist)", asMETHOD (GameConfiguration, SetIndustryProductionPerColonist), asCALL_THISCALL);

    engine->RegisterObjectMethod (className.CString (), "float get_farmsProductionInternalCost () const", asMETHOD (GameConfiguration, GetFarmsProductionInternalCost), asCALL_THISCALL);
    engine->RegisterObjectMethod (className.CString (), "void set_farmsProductionInternalCost (float farmsProductionInternalCost)", asMETHOD (GameConfiguration, SetFarmsProductionInternalCost), asCALL_THISCALL);

    engine->RegisterObjectMethod (className.CString (), "float get_minesProductionInternalCost () const", asMETHOD (GameConfiguration, GetMinesProductionInternalCost), asCALL_THISCALL);
    engine->RegisterObjectMethod (className.CString (), "void set_minesProductionInternalCost (float minesProductionInternalCost)", asMETHOD (GameConfiguration, SetMinesProductionInternalCost), asCALL_THISCALL);

    engine->RegisterObjectMethod (className.CString (), "float get_industryProductionInternalCost () const", asMETHOD (GameConfiguration, GetIndustryProductionInternalCost), asCALL_THISCALL);
    engine->RegisterObjectMethod (className.CString (), "void set_industryProductionInternalCost (float industryProductionInternalCost)", asMETHOD (GameConfiguration, SetIndustryProductionInternalCost), asCALL_THISCALL);

    engine->RegisterObjectMethod (className.CString (), "float get_farmsProductionExternalCost () const", asMETHOD (GameConfiguration, GetFarmsProductionExternalCost), asCALL_THISCALL);
    engine->RegisterObjectMethod (className.CString (), "void set_farmsProductionExternalCost (float farmsProductionExternalCost)", asMETHOD (GameConfiguration, SetFarmsProductionExternalCost), asCALL_THISCALL);

    engine->RegisterObjectMethod (className.CString (), "float get_minesProductionExternalCost () const", asMETHOD (GameConfiguration, GetMinesProductionExternalCost), asCALL_THISCALL);
    engine->RegisterObjectMethod (className.CString (), "void set_minesProductionExternalCost (float minesProductionExternalCost)", asMETHOD (GameConfiguration, SetMinesProductionExternalCost), asCALL_THISCALL);

    engine->RegisterObjectMethod (className.CString (), "float get_industryProductionExternalCost () const", asMETHOD (GameConfiguration, GetIndustryProductionExternalCost), asCALL_THISCALL);
    engine->RegisterObjectMethod (className.CString (), "void set_industryProductionExternalCost (float industryProductionExternalCost)", asMETHOD (GameConfiguration, SetIndustryProductionExternalCost), asCALL_THISCALL);

    engine->RegisterObjectMethod (className.CString (), "float get_internalTaxes () const", asMETHOD (GameConfiguration, GetInternalTaxes), asCALL_THISCALL);
    engine->RegisterObjectMethod (className.CString (), "void set_internalTaxes (float internalTaxes)", asMETHOD (GameConfiguration, SetInternalTaxes), asCALL_THISCALL);

    engine->RegisterObjectMethod (className.CString (), "float get_externalTaxes () const", asMETHOD (GameConfiguration, GetExternalTaxes), asCALL_THISCALL);
    engine->RegisterObjectMethod (className.CString (), "void set_externalTaxes (float externalTaxes)", asMETHOD (GameConfiguration, SetExternalTaxes), asCALL_THISCALL);

    engine->RegisterObjectMethod (className.CString (), "float get_coloniesBasicPopulationIncrease () const", asMETHOD (GameConfiguration, GetColoniesBasicPopulationIncrease), asCALL_THISCALL);
    engine->RegisterObjectMethod (className.CString (), "void set_coloniesBasicPopulationIncrease (float coloniesBasicPopulationIncrease)", asMETHOD (GameConfiguration, SetColoniesBasicPopulationIncrease), asCALL_THISCALL);

    engine->RegisterObjectMethod (className.CString (), "float get_coloniesBasicEvolution () const", asMETHOD (GameConfiguration, GetColoniesBasicEvolution), asCALL_THISCALL);
    engine->RegisterObjectMethod (className.CString (), "void set_coloniesBasicEvolution (float coloniesBasicEvolution)", asMETHOD (GameConfiguration, SetColoniesBasicEvolution), asCALL_THISCALL);

    engine->RegisterObjectMethod (className.CString (), "float get_canBePlantedByOneColonist () const", asMETHOD (GameConfiguration, GetCanBePlantedByOneColonist), asCALL_THISCALL);
    engine->RegisterObjectMethod (className.CString (), "void set_canBePlantedByOneColonist (float canBePlantedByOneColonist)", asMETHOD (GameConfiguration, SetCanBePlantedByOneColonist), asCALL_THISCALL);

    engine->RegisterObjectMethod (className.CString (), "float get_investitionsConsumption () const", asMETHOD (GameConfiguration, GetInvestitionsConsumption), asCALL_THISCALL);
    engine->RegisterObjectMethod (className.CString (), "void set_investitionsConsumption (float investitionsConsumption)", asMETHOD (GameConfiguration, SetInvestitionsConsumption), asCALL_THISCALL);

    engine->RegisterObjectMethod (className.CString (), "float get_investitionsEfficiency () const", asMETHOD (GameConfiguration, GetInvestitionsEfficiency), asCALL_THISCALL);
    engine->RegisterObjectMethod (className.CString (), "void set_investitionsEfficiency (float investitionsEfficiency)", asMETHOD (GameConfiguration, SetInvestitionsEfficiency), asCALL_THISCALL);

    engine->RegisterObjectMethod (className.CString (), "float get_farmsProductionTropicalClimateModifer () const", asMETHOD (GameConfiguration, GetFarmsProductionTropicalClimateModifer), asCALL_THISCALL);
    engine->RegisterObjectMethod (className.CString (), "void set_farmsProductionTropicalClimateModifer (float farmsProductionTropicalClimateModifer)", asMETHOD (GameConfiguration, SetFarmsProductionTropicalClimateModifer), asCALL_THISCALL);

    engine->RegisterObjectMethod (className.CString (), "float get_farmsProductionHotClimateModifer () const", asMETHOD (GameConfiguration, GetFarmsProductionHotClimateModifer), asCALL_THISCALL);
    engine->RegisterObjectMethod (className.CString (), "void set_farmsProductionHotClimateModifer (float farmsProductionHotClimateModifer)", asMETHOD (GameConfiguration, SetFarmsProductionHotClimateModifer), asCALL_THISCALL);

    engine->RegisterObjectMethod (className.CString (), "float get_farmsProductionTemperateClimateModifer () const", asMETHOD (GameConfiguration, GetFarmsProductionTemperateClimateModifer), asCALL_THISCALL);
    engine->RegisterObjectMethod (className.CString (), "void set_farmsProductionTemperateClimateModifer (float farmsProductionTemperateClimateModifer)", asMETHOD (GameConfiguration, SetFarmsProductionTemperateClimateModifer), asCALL_THISCALL);

    engine->RegisterObjectMethod (className.CString (), "float get_farmsProductionTemperateContinentalClimateModifer () const", asMETHOD (GameConfiguration, GetFarmsProductionTemperateContinentalClimateModifer), asCALL_THISCALL);
    engine->RegisterObjectMethod (className.CString (), "void set_farmsProductionTemperateContinentalClimateModifer (float farmsProductionTemperateContinentalClimateModifer)", asMETHOD (GameConfiguration, SetFarmsProductionTemperateContinentalClimateModifer), asCALL_THISCALL);

    engine->RegisterObjectMethod (className.CString (), "float get_farmsProductionDesertClimateModifer () const", asMETHOD (GameConfiguration, GetFarmsProductionDesertClimateModifer), asCALL_THISCALL);
    engine->RegisterObjectMethod (className.CString (), "void set_farmsProductionDesertClimateModifer (float farmsProductionDesertClimateModifer)", asMETHOD (GameConfiguration, SetFarmsProductionDesertClimateModifer), asCALL_THISCALL);

    engine->RegisterObjectMethod (className.CString (), "float get_farmsProductionColdClimateModifer () const", asMETHOD (GameConfiguration, GetFarmsProductionColdClimateModifer), asCALL_THISCALL);
    engine->RegisterObjectMethod (className.CString (), "void set_farmsProductionColdClimateModifer (float farmsProductionColdClimateModifer)", asMETHOD (GameConfiguration, SetFarmsProductionColdClimateModifer), asCALL_THISCALL);

    engine->RegisterObjectMethod (className.CString (), "float get_maximumGameDuration () const", asMETHOD (GameConfiguration, GetMaximumGameDuration), asCALL_THISCALL);
    engine->RegisterObjectMethod (className.CString (), "void set_maximumGameDuration (float maximumGameDuration)", asMETHOD (GameConfiguration, SetMaximumGameDuration), asCALL_THISCALL);

    engine->RegisterObjectMethod (className.CString (), "float get_colonyPointsModifer () const", asMETHOD (GameConfiguration, GetColonyPointsModifer), asCALL_THISCALL);
    engine->RegisterObjectMethod (className.CString (), "void set_colonyPointsModifer (float colonyPointsModifer)", asMETHOD (GameConfiguration, SetColonyPointsModifer), asCALL_THISCALL);

    engine->RegisterObjectMethod (className.CString (), "float get_unitFleetPointsModifer () const", asMETHOD (GameConfiguration, GetUnitFleetPointsModifer), asCALL_THISCALL);
    engine->RegisterObjectMethod (className.CString (), "void set_unitFleetPointsModifer (float unitFleetPointsModifer)", asMETHOD (GameConfiguration, SetUnitFleetPointsModifer), asCALL_THISCALL);

    engine->RegisterObjectMethod (className.CString (), "float get_unitTradersPointsModifer () const", asMETHOD (GameConfiguration, GetUnitTradersPointsModifer), asCALL_THISCALL);
    engine->RegisterObjectMethod (className.CString (), "void set_unitTradersPointsModifer (float unitTradersPointsModifer)", asMETHOD (GameConfiguration, SetUnitTradersPointsModifer), asCALL_THISCALL);

    engine->RegisterObjectMethod (className.CString (), "float get_unitColonizatorsPointsModifer () const", asMETHOD (GameConfiguration, GetUnitColonizatorsPointsModifer), asCALL_THISCALL);
    engine->RegisterObjectMethod (className.CString (), "void set_unitColonizatorsPointsModifer (float unitColonizatorsPointsModifer)", asMETHOD (GameConfiguration, SetUnitColonizatorsPointsModifer), asCALL_THISCALL);

    engine->RegisterObjectMethod (className.CString (), "float get_unitArmyPointsModifer () const", asMETHOD (GameConfiguration, GetUnitArmyPointsModifer), asCALL_THISCALL);
    engine->RegisterObjectMethod (className.CString (), "void set_unitArmyPointsModifer (float unitArmyPointsModifer)", asMETHOD (GameConfiguration, SetUnitArmyPointsModifer), asCALL_THISCALL);

    engine->RegisterObjectMethod (className.CString (), "String get_victoryTypesProcessorScriptPath () const", asMETHOD (GameConfiguration, GetVictoryTypesProcessorScriptPath), asCALL_THISCALL);
    engine->RegisterObjectMethod (className.CString (), "void set_victoryTypesProcessorScriptPath (String victoryTypesProcessorScriptPath)", asMETHOD (GameConfiguration, SetVictoryTypesProcessorScriptPath), asCALL_THISCALL);
}
}

