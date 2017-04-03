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
    engine->RegisterObjectMethod (className.CString (), "StringHash GetHeuristicNearestWayToEuropeDistrict (Map @map, District @wayStartDistrict) const", asMETHOD (GameConfiguration, GetHeuristicNearestWayToEuropeDistrict), asCALL_THISCALL);

    engine->RegisterObjectMethod (className.CString (), "float get_sailSpeed () const", asMETHOD (GameConfiguration, GetSailSpeed), asCALL_THISCALL);
    engine->RegisterObjectMethod (className.CString (), "void set_sailSpeed (float sailSpeed)", asMETHOD (GameConfiguration, SetSailSpeed), asCALL_THISCALL);

    engine->RegisterObjectMethod (className.CString (), "float get_playerStartGold () const", asMETHOD (GameConfiguration, GetPlayerStartGold), asCALL_THISCALL);
    engine->RegisterObjectMethod (className.CString (), "void set_playerStartGold (float playerStartGold)", asMETHOD (GameConfiguration, SetPlayerStartGold), asCALL_THISCALL);

    engine->RegisterObjectMethod (className.CString (), "float get_marchSpeed () const", asMETHOD (GameConfiguration, GetMarchSpeed), asCALL_THISCALL);
    engine->RegisterObjectMethod (className.CString (), "void set_marchSpeed (float marchSpeed)", asMETHOD (GameConfiguration, SetMarchSpeed), asCALL_THISCALL);

    engine->RegisterObjectMethod (className.CString (), "float get_embarkationSpeed () const", asMETHOD (GameConfiguration, GetEmbarkationSpeed), asCALL_THISCALL);
    engine->RegisterObjectMethod (className.CString (), "void set_embarkationSpeed (float embarkationSpeed)", asMETHOD (GameConfiguration, SetEmbarkationSpeed), asCALL_THISCALL);

    engine->RegisterObjectMethod (className.CString (), "float get_disembarkationSpeed () const", asMETHOD (GameConfiguration, GetDisembarkationSpeed), asCALL_THISCALL);
    engine->RegisterObjectMethod (className.CString (), "void set_disembarkationSpeed (float disembarkationSpeed)", asMETHOD (GameConfiguration, SetDisembarkationSpeed), asCALL_THISCALL);

    engine->RegisterObjectMethod (className.CString (), "float get_oneColonizatorSendingCost () const", asMETHOD (GameConfiguration, GetOneColonizatorSendingCost), asCALL_THISCALL);
    engine->RegisterObjectMethod (className.CString (), "void set_oneColonizatorSendingCost (float oneColonizatorSendingCost)", asMETHOD (GameConfiguration, SetOneColonizatorSendingCost), asCALL_THISCALL);

    engine->RegisterObjectMethod (className.CString (), "float get_oneWarShipBuildingCost () const", asMETHOD (GameConfiguration, GetOneWarShipBuildingCost), asCALL_THISCALL);
    engine->RegisterObjectMethod (className.CString (), "void set_oneWarShipBuildingCost (float oneWarShipBuildingCost)", asMETHOD (GameConfiguration, SetOneWarShipBuildingCost), asCALL_THISCALL);

    engine->RegisterObjectMethod (className.CString (), "float get_oneWarShipCrew () const", asMETHOD (GameConfiguration, GetOneWarShipCrew), asCALL_THISCALL);
    engine->RegisterObjectMethod (className.CString (), "void set_oneWarShipCrew (float oneWarShipCrew)", asMETHOD (GameConfiguration, SetOneWarShipCrew), asCALL_THISCALL);

    engine->RegisterObjectMethod (className.CString (), "float get_oneWarShipBasicBuildTime () const", asMETHOD (GameConfiguration, GetOneWarShipBasicBuildTime), asCALL_THISCALL);
    engine->RegisterObjectMethod (className.CString (), "void set_oneWarShipBasicBuildTime (float oneWarShipBasicBuildTime)", asMETHOD (GameConfiguration, SetOneWarShipBasicBuildTime), asCALL_THISCALL);

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

    engine->RegisterObjectMethod (className.CString (), "float get_tradeAreaInternalProfitToBalance () const", asMETHOD (GameConfiguration, GetTradeAreaInternalProfitToBalance), asCALL_THISCALL);
    engine->RegisterObjectMethod (className.CString (), "void set_tradeAreaInternalProfitToBalance (float tradeAreaInternalProfitToBalance)", asMETHOD (GameConfiguration, SetTradeAreaInternalProfitToBalance), asCALL_THISCALL);

    engine->RegisterObjectMethod (className.CString (), "float get_tradeAreaExternalProfitToBalance () const", asMETHOD (GameConfiguration, GetTradeAreaExternalProfitToBalance), asCALL_THISCALL);
    engine->RegisterObjectMethod (className.CString (), "void set_tradeAreaExternalProfitToBalance (float tradeAreaExternalProfitToBalance)", asMETHOD (GameConfiguration, SetTradeAreaExternalProfitToBalance), asCALL_THISCALL);

    engine->RegisterObjectMethod (className.CString (), "float get_tradeAreaFarmsLogisticsExpenses () const", asMETHOD (GameConfiguration, GetTradeAreaFarmsLogisticsExpenses), asCALL_THISCALL);
    engine->RegisterObjectMethod (className.CString (), "void set_tradeAreaFarmsLogisticsExpenses (float tradeAreaFarmsLogisticsExpenses)", asMETHOD (GameConfiguration, SetTradeAreaFarmsLogisticsExpenses), asCALL_THISCALL);

    engine->RegisterObjectMethod (className.CString (), "float get_tradeAreaMinesLogisticsExpenses () const", asMETHOD (GameConfiguration, GetTradeAreaMinesLogisticsExpenses), asCALL_THISCALL);
    engine->RegisterObjectMethod (className.CString (), "void set_tradeAreaMinesLogisticsExpenses (float tradeAreaMinesLogisticsExpenses)", asMETHOD (GameConfiguration, SetTradeAreaMinesLogisticsExpenses), asCALL_THISCALL);

    engine->RegisterObjectMethod (className.CString (), "float get_tradeAreaIndustryLogisticsExpenses () const", asMETHOD (GameConfiguration, GetTradeAreaIndustryLogisticsExpenses), asCALL_THISCALL);
    engine->RegisterObjectMethod (className.CString (), "void set_tradeAreaIndustryLogisticsExpenses (float tradeAreaIndustryLogisticsExpenses)", asMETHOD (GameConfiguration, SetTradeAreaIndustryLogisticsExpenses), asCALL_THISCALL);

    engine->RegisterObjectMethod (className.CString (), "float get_tradeAreaFarmsDefenseExpenses () const", asMETHOD (GameConfiguration, GetTradeAreaFarmsDefenseExpenses), asCALL_THISCALL);
    engine->RegisterObjectMethod (className.CString (), "void set_tradeAreaFarmsDefenseExpenses (float tradeAreaFarmsDefenseExpenses)", asMETHOD (GameConfiguration, SetTradeAreaFarmsDefenseExpenses), asCALL_THISCALL);

    engine->RegisterObjectMethod (className.CString (), "float get_tradeAreaMinesDefenseExpenses () const", asMETHOD (GameConfiguration, GetTradeAreaMinesDefenseExpenses), asCALL_THISCALL);
    engine->RegisterObjectMethod (className.CString (), "void set_tradeAreaMinesDefenseExpenses (float tradeAreaMinesDefenseExpenses)", asMETHOD (GameConfiguration, SetTradeAreaMinesDefenseExpenses), asCALL_THISCALL);

    engine->RegisterObjectMethod (className.CString (), "float get_tradeAreaIndustryDefenseExpenses () const", asMETHOD (GameConfiguration, GetTradeAreaIndustryDefenseExpenses), asCALL_THISCALL);
    engine->RegisterObjectMethod (className.CString (), "void set_tradeAreaIndustryDefenseExpenses (float tradeAreaIndustryDefenseExpenses)", asMETHOD (GameConfiguration, SetTradeAreaIndustryDefenseExpenses), asCALL_THISCALL);

    engine->RegisterObjectMethod (className.CString (), "float get_evolutionCostPerLevel () const", asMETHOD (GameConfiguration, GetEvolutionCostPerLevel), asCALL_THISCALL);
    engine->RegisterObjectMethod (className.CString (), "void set_evolutionCostPerLevel (float evolutionCostPerLevel)", asMETHOD (GameConfiguration, SetEvolutionCostPerLevel), asCALL_THISCALL);

    engine->RegisterObjectMethod (className.CString (), "float get_degradationCostPerLevel () const", asMETHOD (GameConfiguration, GetDegradationCostPerLevel), asCALL_THISCALL);
    engine->RegisterObjectMethod (className.CString (), "void set_degradationCostPerLevel (float degradationCostPerLevel)", asMETHOD (GameConfiguration, SetDegradationCostPerLevel), asCALL_THISCALL);

    engine->RegisterObjectMethod (className.CString (), "float get_basicEvolutionSpeed () const", asMETHOD (GameConfiguration, GetBasicEvolutionSpeed), asCALL_THISCALL);
    engine->RegisterObjectMethod (className.CString (), "void set_basicEvolutionSpeed (float basicEvolutionSpeed)", asMETHOD (GameConfiguration, SetBasicEvolutionSpeed), asCALL_THISCALL);

    engine->RegisterObjectMethod (className.CString (), "float get_basicDegradationSpeed () const", asMETHOD (GameConfiguration, GetBasicDegradationSpeed), asCALL_THISCALL);
    engine->RegisterObjectMethod (className.CString (), "void set_basicDegradationSpeed (float basicDegradationSpeed)", asMETHOD (GameConfiguration, SetBasicDegradationSpeed), asCALL_THISCALL);

    engine->RegisterObjectMethod (className.CString (), "float get_internalTaxes () const", asMETHOD (GameConfiguration, GetInternalTaxes), asCALL_THISCALL);
    engine->RegisterObjectMethod (className.CString (), "void set_internalTaxes (float internalTaxes)", asMETHOD (GameConfiguration, SetInternalTaxes), asCALL_THISCALL);

    engine->RegisterObjectMethod (className.CString (), "float get_externalTaxes () const", asMETHOD (GameConfiguration, GetExternalTaxes), asCALL_THISCALL);
    engine->RegisterObjectMethod (className.CString (), "void set_externalTaxes (float externalTaxes)", asMETHOD (GameConfiguration, SetExternalTaxes), asCALL_THISCALL);

    engine->RegisterObjectMethod (className.CString (), "float get_coloniesBasicPopulationIncrease () const", asMETHOD (GameConfiguration, GetColoniesBasicPopulationIncrease), asCALL_THISCALL);
    engine->RegisterObjectMethod (className.CString (), "void set_coloniesBasicPopulationIncrease (float coloniesBasicPopulationIncrease)", asMETHOD (GameConfiguration, SetColoniesBasicPopulationIncrease), asCALL_THISCALL);

    engine->RegisterObjectMethod (className.CString (), "float get_canBePlantedByOneColonist () const", asMETHOD (GameConfiguration, GetCanBePlantedByOneColonist), asCALL_THISCALL);
    engine->RegisterObjectMethod (className.CString (), "void set_canBePlantedByOneColonist (float canBePlantedByOneColonist)", asMETHOD (GameConfiguration, SetCanBePlantedByOneColonist), asCALL_THISCALL);

    engine->RegisterObjectMethod (className.CString (), "float get_minimumLogisticsToUniteDistrictsToTradeArea () const", asMETHOD (GameConfiguration, GetMinimumLogisticsToUniteDistrictsToTradeArea), asCALL_THISCALL);
    engine->RegisterObjectMethod (className.CString (), "void set_minimumLogisticsToUniteDistrictsToTradeArea (float minimumLogisticsToUniteDistrictsToTradeArea)", asMETHOD (GameConfiguration, SetMinimumLogisticsToUniteDistrictsToTradeArea), asCALL_THISCALL);

    engine->RegisterObjectMethod (className.CString (), "float get_minimumLogisticsToUniteDistrictsToTradeAreaBySea () const", asMETHOD (GameConfiguration, GetMinimumLogisticsToUniteDistrictsToTradeAreaBySea), asCALL_THISCALL);
    engine->RegisterObjectMethod (className.CString (), "void set_minimumLogisticsToUniteDistrictsToTradeAreaBySea (float minimumLogisticsToUniteDistrictsToTradeAreaBySea)", asMETHOD (GameConfiguration, SetMinimumLogisticsToUniteDistrictsToTradeAreaBySea), asCALL_THISCALL);

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

    engine->RegisterObjectMethod (className.CString (), "float get_farmsProductionRelativePriceTropicalClimateModifer () const", asMETHOD (GameConfiguration, GetFarmsProductionRelativePriceTropicalClimateModifer), asCALL_THISCALL);
    engine->RegisterObjectMethod (className.CString (), "void set_farmsProductionRelativePriceTropicalClimateModifer (float farmsProductionRelativePriceTropicalClimateModifer)", asMETHOD (GameConfiguration, SetFarmsProductionRelativePriceTropicalClimateModifer), asCALL_THISCALL);

    engine->RegisterObjectMethod (className.CString (), "float get_farmsProductionRelativePriceHotClimateModifer () const", asMETHOD (GameConfiguration, GetFarmsProductionRelativePriceHotClimateModifer), asCALL_THISCALL);
    engine->RegisterObjectMethod (className.CString (), "void set_farmsProductionRelativePriceHotClimateModifer (float farmsProductionRelativePriceHotClimateModifer)", asMETHOD (GameConfiguration, SetFarmsProductionRelativePriceHotClimateModifer), asCALL_THISCALL);

    engine->RegisterObjectMethod (className.CString (), "float get_farmsProductionRelativePriceTemperateClimateModifer () const", asMETHOD (GameConfiguration, GetFarmsProductionRelativePriceTemperateClimateModifer), asCALL_THISCALL);
    engine->RegisterObjectMethod (className.CString (), "void set_farmsProductionRelativePriceTemperateClimateModifer (float farmsProductionRelativePriceTemperateClimateModifer)", asMETHOD (GameConfiguration, SetFarmsProductionRelativePriceTemperateClimateModifer), asCALL_THISCALL);

    engine->RegisterObjectMethod (className.CString (), "float get_farmsProductionRelativePriceTemperateContinentalClimateModifer () const", asMETHOD (GameConfiguration, GetFarmsProductionRelativePriceTemperateContinentalClimateModifer), asCALL_THISCALL);
    engine->RegisterObjectMethod (className.CString (), "void set_farmsProductionRelativePriceTemperateContinentalClimateModifer (float farmsProductionRelativePriceTemperateContinentalClimateModifer)", asMETHOD (GameConfiguration, SetFarmsProductionRelativePriceTemperateContinentalClimateModifer), asCALL_THISCALL);

    engine->RegisterObjectMethod (className.CString (), "float get_farmsProductionRelativePriceDesertClimateModifer () const", asMETHOD (GameConfiguration, GetFarmsProductionRelativePriceDesertClimateModifer), asCALL_THISCALL);
    engine->RegisterObjectMethod (className.CString (), "void set_farmsProductionRelativePriceDesertClimateModifer (float farmsProductionRelativePriceDesertClimateModifer)", asMETHOD (GameConfiguration, SetFarmsProductionRelativePriceDesertClimateModifer), asCALL_THISCALL);

    engine->RegisterObjectMethod (className.CString (), "float get_farmsProductionRelativePriceColdClimateModifer () const", asMETHOD (GameConfiguration, GetFarmsProductionRelativePriceColdClimateModifer), asCALL_THISCALL);
    engine->RegisterObjectMethod (className.CString (), "void set_farmsProductionRelativePriceColdClimateModifer (float farmsProductionRelativePriceColdClimateModifer)", asMETHOD (GameConfiguration, SetFarmsProductionRelativePriceColdClimateModifer), asCALL_THISCALL);

    engine->RegisterObjectMethod (className.CString (), "float get_farmsProductionQualityTropicalClimateModifer () const", asMETHOD (GameConfiguration, GetFarmsProductionQualityTropicalClimateModifer), asCALL_THISCALL);
    engine->RegisterObjectMethod (className.CString (), "void set_farmsProductionQualityTropicalClimateModifer (float farmsProductionQualityTropicalClimateModifer)", asMETHOD (GameConfiguration, SetFarmsProductionQualityTropicalClimateModifer), asCALL_THISCALL);

    engine->RegisterObjectMethod (className.CString (), "float get_farmsProductionQualityHotClimateModifer () const", asMETHOD (GameConfiguration, GetFarmsProductionQualityHotClimateModifer), asCALL_THISCALL);
    engine->RegisterObjectMethod (className.CString (), "void set_farmsProductionQualityHotClimateModifer (float farmsProductionQualityHotClimateModifer)", asMETHOD (GameConfiguration, SetFarmsProductionQualityHotClimateModifer), asCALL_THISCALL);

    engine->RegisterObjectMethod (className.CString (), "float get_farmsProductionQualityTemperateClimateModifer () const", asMETHOD (GameConfiguration, GetFarmsProductionQualityTemperateClimateModifer), asCALL_THISCALL);
    engine->RegisterObjectMethod (className.CString (), "void set_farmsProductionQualityTemperateClimateModifer (float farmsProductionQualityTemperateClimateModifer)", asMETHOD (GameConfiguration, SetFarmsProductionQualityTemperateClimateModifer), asCALL_THISCALL);

    engine->RegisterObjectMethod (className.CString (), "float get_farmsProductionQualityTemperateContinentalClimateModifer () const", asMETHOD (GameConfiguration, GetFarmsProductionQualityTemperateContinentalClimateModifer), asCALL_THISCALL);
    engine->RegisterObjectMethod (className.CString (), "void set_farmsProductionQualityTemperateContinentalClimateModifer (float farmsProductionQualityTemperateContinentalClimateModifer)", asMETHOD (GameConfiguration, SetFarmsProductionQualityTemperateContinentalClimateModifer), asCALL_THISCALL);

    engine->RegisterObjectMethod (className.CString (), "float get_farmsProductionQualityDesertClimateModifer () const", asMETHOD (GameConfiguration, GetFarmsProductionQualityDesertClimateModifer), asCALL_THISCALL);
    engine->RegisterObjectMethod (className.CString (), "void set_farmsProductionQualityDesertClimateModifer (float farmsProductionQualityDesertClimateModifer)", asMETHOD (GameConfiguration, SetFarmsProductionQualityDesertClimateModifer), asCALL_THISCALL);

    engine->RegisterObjectMethod (className.CString (), "float get_farmsProductionQualityColdClimateModifer () const", asMETHOD (GameConfiguration, GetFarmsProductionQualityColdClimateModifer), asCALL_THISCALL);
    engine->RegisterObjectMethod (className.CString (), "void set_farmsProductionQualityColdClimateModifer (float farmsProductionQualityColdClimateModifer)", asMETHOD (GameConfiguration, SetFarmsProductionQualityColdClimateModifer), asCALL_THISCALL);

    engine->RegisterObjectMethod (className.CString (), "float get_minesProductionRelativePriceHasCoalModifer () const", asMETHOD (GameConfiguration, GetMinesProductionRelativePriceHasCoalModifer), asCALL_THISCALL);
    engine->RegisterObjectMethod (className.CString (), "void set_minesProductionRelativePriceHasCoalModifer (float minesProductionRelativePriceHasCoalModifer)", asMETHOD (GameConfiguration, SetMinesProductionRelativePriceHasCoalModifer), asCALL_THISCALL);

    engine->RegisterObjectMethod (className.CString (), "float get_minesProductionRelativePriceHasIronModifer () const", asMETHOD (GameConfiguration, GetMinesProductionRelativePriceHasIronModifer), asCALL_THISCALL);
    engine->RegisterObjectMethod (className.CString (), "void set_minesProductionRelativePriceHasIronModifer (float minesProductionRelativePriceHasIronModifer)", asMETHOD (GameConfiguration, SetMinesProductionRelativePriceHasIronModifer), asCALL_THISCALL);

    engine->RegisterObjectMethod (className.CString (), "float get_minesProductionRelativePriceHasSilverModifer () const", asMETHOD (GameConfiguration, GetMinesProductionRelativePriceHasSilverModifer), asCALL_THISCALL);
    engine->RegisterObjectMethod (className.CString (), "void set_minesProductionRelativePriceHasSilverModifer (float minesProductionRelativePriceHasSilverModifer)", asMETHOD (GameConfiguration, SetMinesProductionRelativePriceHasSilverModifer), asCALL_THISCALL);

    engine->RegisterObjectMethod (className.CString (), "float get_minesProductionRelativePriceHasGoldModifer () const", asMETHOD (GameConfiguration, GetMinesProductionRelativePriceHasGoldModifer), asCALL_THISCALL);
    engine->RegisterObjectMethod (className.CString (), "void set_minesProductionRelativePriceHasGoldModifer (float minesProductionRelativePriceHasGoldModifer)", asMETHOD (GameConfiguration, SetMinesProductionRelativePriceHasGoldModifer), asCALL_THISCALL);

    engine->RegisterObjectMethod (className.CString (), "float get_minesProductionQualityHasCoalModifer () const", asMETHOD (GameConfiguration, GetMinesProductionQualityHasCoalModifer), asCALL_THISCALL);
    engine->RegisterObjectMethod (className.CString (), "void set_minesProductionQualityHasCoalModifer (float minesProductionQualityHasCoalModifer)", asMETHOD (GameConfiguration, SetMinesProductionQualityHasCoalModifer), asCALL_THISCALL);

    engine->RegisterObjectMethod (className.CString (), "float get_minesProductionQualityHasIronModifer () const", asMETHOD (GameConfiguration, GetMinesProductionQualityHasIronModifer), asCALL_THISCALL);
    engine->RegisterObjectMethod (className.CString (), "void set_minesProductionQualityHasIronModifer (float minesProductionQualityHasIronModifer)", asMETHOD (GameConfiguration, SetMinesProductionQualityHasIronModifer), asCALL_THISCALL);

    engine->RegisterObjectMethod (className.CString (), "float get_minesProductionQualityHasSilverModifer () const", asMETHOD (GameConfiguration, GetMinesProductionQualityHasSilverModifer), asCALL_THISCALL);
    engine->RegisterObjectMethod (className.CString (), "void set_minesProductionQualityHasSilverModifer (float minesProductionQualityHasSilverModifer)", asMETHOD (GameConfiguration, SetMinesProductionQualityHasSilverModifer), asCALL_THISCALL);

    engine->RegisterObjectMethod (className.CString (), "float get_minesProductionQualityHasGoldModifer () const", asMETHOD (GameConfiguration, GetMinesProductionQualityHasGoldModifer), asCALL_THISCALL);
    engine->RegisterObjectMethod (className.CString (), "void set_minesProductionQualityHasGoldModifer (float minesProductionQualityHasGoldModifer)", asMETHOD (GameConfiguration, SetMinesProductionQualityHasGoldModifer), asCALL_THISCALL);

    engine->RegisterObjectMethod (className.CString (), "float get_industryProductionRelativePriceHasCoalModifer () const", asMETHOD (GameConfiguration, GetIndustryProductionRelativePriceHasCoalModifer), asCALL_THISCALL);
    engine->RegisterObjectMethod (className.CString (), "void set_industryProductionRelativePriceHasCoalModifer (float industryProductionRelativePriceHasCoalModifer)", asMETHOD (GameConfiguration, SetIndustryProductionRelativePriceHasCoalModifer), asCALL_THISCALL);

    engine->RegisterObjectMethod (className.CString (), "float get_industryProductionRelativePriceHasIronModifer () const", asMETHOD (GameConfiguration, GetIndustryProductionRelativePriceHasIronModifer), asCALL_THISCALL);
    engine->RegisterObjectMethod (className.CString (), "void set_industryProductionRelativePriceHasIronModifer (float industryProductionRelativePriceHasIronModifer)", asMETHOD (GameConfiguration, SetIndustryProductionRelativePriceHasIronModifer), asCALL_THISCALL);

    engine->RegisterObjectMethod (className.CString (), "float get_industryProductionRelativePriceHasSilverModifer () const", asMETHOD (GameConfiguration, GetIndustryProductionRelativePriceHasSilverModifer), asCALL_THISCALL);
    engine->RegisterObjectMethod (className.CString (), "void set_industryProductionRelativePriceHasSilverModifer (float industryProductionRelativePriceHasSilverModifer)", asMETHOD (GameConfiguration, SetIndustryProductionRelativePriceHasSilverModifer), asCALL_THISCALL);

    engine->RegisterObjectMethod (className.CString (), "float get_industryProductionRelativePriceHasGoldModifer () const", asMETHOD (GameConfiguration, GetIndustryProductionRelativePriceHasGoldModifer), asCALL_THISCALL);
    engine->RegisterObjectMethod (className.CString (), "void set_industryProductionRelativePriceHasGoldModifer (float industryProductionRelativePriceHasGoldModifer)", asMETHOD (GameConfiguration, SetIndustryProductionRelativePriceHasGoldModifer), asCALL_THISCALL);

    engine->RegisterObjectMethod (className.CString (), "float get_industryProductionQualityHasCoalModifer () const", asMETHOD (GameConfiguration, GetIndustryProductionQualityHasCoalModifer), asCALL_THISCALL);
    engine->RegisterObjectMethod (className.CString (), "void set_industryProductionQualityHasCoalModifer (float industryProductionQualityHasCoalModifer)", asMETHOD (GameConfiguration, SetIndustryProductionQualityHasCoalModifer), asCALL_THISCALL);

    engine->RegisterObjectMethod (className.CString (), "float get_industryProductionQualityHasIronModifer () const", asMETHOD (GameConfiguration, GetIndustryProductionQualityHasIronModifer), asCALL_THISCALL);
    engine->RegisterObjectMethod (className.CString (), "void set_industryProductionQualityHasIronModifer (float industryProductionQualityHasIronModifer)", asMETHOD (GameConfiguration, SetIndustryProductionQualityHasIronModifer), asCALL_THISCALL);

    engine->RegisterObjectMethod (className.CString (), "float get_industryProductionQualityHasSilverModifer () const", asMETHOD (GameConfiguration, GetIndustryProductionQualityHasSilverModifer), asCALL_THISCALL);
    engine->RegisterObjectMethod (className.CString (), "void set_industryProductionQualityHasSilverModifer (float industryProductionQualityHasSilverModifer)", asMETHOD (GameConfiguration, SetIndustryProductionQualityHasSilverModifer), asCALL_THISCALL);

    engine->RegisterObjectMethod (className.CString (), "float get_industryProductionQualityHasGoldModifer () const", asMETHOD (GameConfiguration, GetIndustryProductionQualityHasGoldModifer), asCALL_THISCALL);
    engine->RegisterObjectMethod (className.CString (), "void set_industryProductionQualityHasGoldModifer (float industryProductionQualityHasGoldModifer)", asMETHOD (GameConfiguration, SetIndustryProductionQualityHasGoldModifer), asCALL_THISCALL);

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

