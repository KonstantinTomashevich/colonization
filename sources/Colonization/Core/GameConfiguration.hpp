#pragma once
#include <Urho3D/Scene/Component.h>

namespace Colonization
{
class GameConfiguration : public Urho3D::Component
{
URHO3D_OBJECT (GameConfiguration, Component)
protected:
    Urho3D::PODVector <Urho3D::StringHash> wayToEuropeDistricts_;
    float sailSpeed_;
    float marchSpeed_;
    float embarkationSpeed_;
    float disembarkationSpeed_;

    float oneColonistFarmsProductionConsumption_;
    float oneSoldierFarmsProductionConsumption_;
    float farmsProductionMinesConsumption_;
    float farmsProductionIndustryConsumption_;

    float oneColonistMinesProductionConsumption_;
    float oneSoldierMinesProductionConsumption_;
    float minesProductionFarmsConsumption_;
    float minesProductionIndustryConsumption_;

    float oneColonistIndustryProductionConsumption_;
    float oneSoldierIndustryProductionConsumption_;
    float industryProductionFarmsConsumption_;
    float industryProductionMinesConsumption_;

    float farmsProductionPerColonist_;
    float minesProductionPerColonist_;
    float industryProductionPerColonist_;

    float farmsProductionInternalCost_;
    float minesProductionInternalCost_;
    float industryProductionInternalCost_;

    float farmsProductionExternalCost_;
    float minesProductionExternalCost_;
    float industryProductionExternalCost_;

    float tradeAreaLogisticsBonusModifer_;
    float tradeAreaDefenseBonusModifer_;
    float tradeAreaInternalProfitToBalance_;
    float tradeAreaExternalProfitToBalance_;

    float tradeAreaFarmsLogisticsExpenses_;
    float tradeAreaMinesLogisticsExpenses_;
    float tradeAreaIndustryLogisticsExpenses_;

    float tradeAreaFarmsDefenseExpenses_;
    float tradeAreaMinesDefenseExpenses_;
    float tradeAreaIndustryDefenseExpenses_;

    float evolutionCostPerLevel_;
    float degradationCostPerLevel_;
    float basicEvolutionSpeed_;
    float basicDegradationSpeed_;

    // TODO: Move taxes to player. In next game versions player should have ability to set them.
    float internalTaxes_;
    float externalTaxes_;

    float coloniesBasicPopulationIncrease_;
    float coloniesBasicEvolution_;
    float canBePlantedByOneColonist_;

    float farmsProductionTropicalClimateModifer_;
    float farmsProductionHotClimateModifer_;
    float farmsProductionTemperateClimateModifer_;
    float farmsProductionTemperateContinentalClimateModifer_;
    float farmsProductionDesertClimateModifer_;
    float farmsProductionColdClimateModifer_;

    float maximumGameDuration_;
    float colonyPointsModifer_;
    float unitFleetPointsModifer_;
    float unitTradersPointsModifer_;
    float unitColonizatorsPointsModifer_;
    float unitArmyPointsModifer_;
    Urho3D::String victoryTypesProcessorScriptPath_;

public:
    GameConfiguration (Urho3D::Context *context);
    virtual ~GameConfiguration ();
    static void RegisterObject (Urho3D::Context *context);

    Urho3D::PODVector <Urho3D::StringHash> GetWayToEuropeDistricts () const;
    void SetWayToEuropeDistricts (Urho3D::PODVector <Urho3D::StringHash> wayToEuropeDistricts);
    Urho3D::VariantVector GetWayToEuropeDistrictsAttribute () const;
    void SetWayToEuropeDistrictsAttribute (const Urho3D::VariantVector &wayToEuropeDistricts);

    float GetSailSpeed () const;
    void SetSailSpeed (float sailSpeed);

    float GetMarchSpeed () const;
    void SetMarchSpeed (float marchSpeed);

    float GetEmbarkationSpeed () const;
    void SetEmbarkationSpeed (float embarkationSpeed);

    float GetDisembarkationSpeed () const;
    void SetDisembarkationSpeed (float disembarkationSpeed);

    float GetOneColonistFarmsProductionConsumption () const;
    void SetOneColonistFarmsProductionConsumption (float oneColonistFarmsProductionConsumption);

    float GetOneSoldierFarmsProductionConsumption () const;
    void SetOneSoldierFarmsProductionConsumption (float oneSoldierFarmsProductionConsumption);

    float GetFarmsProductionMinesConsumption () const;
    void SetFarmsProductionMinesConsumption (float farmsProductionMinesConsumption);

    float GetFarmsProductionIndustryConsumption () const;
    void SetFarmsProductionIndustryConsumption (float farmsProductionIndustryConsumption);

    float GetOneColonistMinesProductionConsumption () const;
    void SetOneColonistMinesProductionConsumption (float oneColonistMinesProductionConsumption);

    float GetOneSoldierMinesProductionConsumption () const;
    void SetOneSoldierMinesProductionConsumption (float oneSoldierMinesProductionConsumption);

    float GetMinesProductionFarmsConsumption () const;
    void SetMinesProductionFarmsConsumption (float minesProductionFarmsConsumption);

    float GetMinesProductionIndustryConsumption () const;
    void SetMinesProductionIndustryConsumption (float minesProductionIndustryConsumption);

    float GetOneColonistIndustryProductionConsumption () const;
    void SetOneColonistIndustryProductionConsumption (float oneColonistIndustryProductionConsumption);

    float GetOneSoldierIndustryProductionConsumption () const;
    void SetOneSoldierIndustryProductionConsumption (float oneSoldierIndustryProductionConsumption);

    float GetIndustryProductionFarmsConsumption () const;
    void SetIndustryProductionFarmsConsumption (float industryProductionFarmsConsumption);

    float GetIndustryProductionMinesConsumption () const;
    void SetIndustryProductionMinesConsumption (float industryProductionMinesConsumption);

    float GetFarmsProductionPerColonist () const;
    void SetFarmsProductionPerColonist (float farmsProductionPerColonist);

    float GetMinesProductionPerColonist () const;
    void SetMinesProductionPerColonist (float minesProductionPerColonist);

    float GetIndustryProductionPerColonist () const;
    void SetIndustryProductionPerColonist (float industryProductionPerColonist);

    float GetFarmsProductionInternalCost () const;
    void SetFarmsProductionInternalCost (float farmsProductionInternalCost);

    float GetMinesProductionInternalCost () const;
    void SetMinesProductionInternalCost (float minesProductionInternalCost);

    float GetIndustryProductionInternalCost () const;
    void SetIndustryProductionInternalCost (float industryProductionInternalCost);

    float GetFarmsProductionExternalCost () const;
    void SetFarmsProductionExternalCost (float farmsProductionExternalCost);

    float GetMinesProductionExternalCost () const;
    void SetMinesProductionExternalCost (float minesProductionExternalCost);

    float GetIndustryProductionExternalCost () const;
    void SetIndustryProductionExternalCost (float industryProductionExternalCost);

    float GetTradeAreaLogisticsBonusModifer () const;
    void SetTradeAreaLogisticsBonusModifer (float tradeAreaLogisticsBonusModifer);

    float GetTradeAreaDefenseBonusModifer () const;
    void SetTradeAreaDefenseBonusModifer (float tradeAreaDefenseBonusModifer);

    float GetTradeAreaInternalProfitToBalance () const;
    void SetTradeAreaInternalProfitToBalance (float tradeAreaInternalProfitToBalance);

    float GetTradeAreaExternalProfitToBalance () const;
    void SetTradeAreaExternalProfitToBalance (float tradeAreaExternalProfitToBalance);

    float GetTradeAreaFarmsLogisticsExpenses () const;
    void SetTradeAreaFarmsLogisticsExpenses (float tradeAreaFarmsLogisticsExpenses);

    float GetTradeAreaMinesLogisticsExpenses () const;
    void SetTradeAreaMinesLogisticsExpenses (float tradeAreaMinesLogisticsExpenses);

    float GetTradeAreaIndustryLogisticsExpenses () const;
    void SetTradeAreaIndustryLogisticsExpenses (float tradeAreaIndustryLogisticsExpenses);

    float GetTradeAreaFarmsDefenseExpenses () const;
    void SetTradeAreaFarmsDefenseExpenses (float tradeAreaFarmsDefenseExpenses);

    float GetTradeAreaMinesDefenseExpenses () const;
    void SetTradeAreaMinesDefenseExpenses (float tradeAreaMinesDefenseExpenses);

    float GetTradeAreaIndustryDefenseExpenses () const;
    void SetTradeAreaIndustryDefenseExpenses (float tradeAreaIndustryDefenseExpenses);

    float GetEvolutionCostPerLevel () const;
    void SetEvolutionCostPerLevel (float evolutionCostPerLevel);

    float GetDegradationCostPerLevel () const;
    void SetDegradationCostPerLevel (float degradationCostPerLevel);

    float GetBasicEvolutionSpeed () const;
    void SetBasicEvolutionSpeed (float basicEvolutionSpeed);

    float GetBasicDegradationSpeed () const;
    void SetBasicDegradationSpeed (float basicDegradationSpeed);

    float GetInternalTaxes () const;
    void SetInternalTaxes (float internalTaxes);

    float GetExternalTaxes () const;
    void SetExternalTaxes (float externalTaxes);

    float GetColoniesBasicPopulationIncrease () const;
    void SetColoniesBasicPopulationIncrease (float coloniesBasicPopulationIncrease);

    float GetColoniesBasicEvolution () const;
    void SetColoniesBasicEvolution (float coloniesBasicEvolution);

    float GetCanBePlantedByOneColonist () const;
    void SetCanBePlantedByOneColonist (float canBePlantedByOneColonist);

    float GetFarmsProductionTropicalClimateModifer () const;
    void SetFarmsProductionTropicalClimateModifer (float farmsProductionTropicalClimateModifer);

    float GetFarmsProductionHotClimateModifer () const;
    void SetFarmsProductionHotClimateModifer (float farmsProductionHotClimateModifer);

    float GetFarmsProductionTemperateClimateModifer () const;
    void SetFarmsProductionTemperateClimateModifer (float farmsProductionTemperateClimateModifer);

    float GetFarmsProductionTemperateContinentalClimateModifer () const;
    void SetFarmsProductionTemperateContinentalClimateModifer (float farmsProductionTemperateContinentalClimateModifer);

    float GetFarmsProductionDesertClimateModifer () const;
    void SetFarmsProductionDesertClimateModifer (float farmsProductionDesertClimateModifer);

    float GetFarmsProductionColdClimateModifer () const;
    void SetFarmsProductionColdClimateModifer (float farmsProductionColdClimateModifer);

    float GetMaximumGameDuration () const;
    void SetMaximumGameDuration (float maximumGameDuration);

    float GetColonyPointsModifer () const;
    void SetColonyPointsModifer (float colonyPointsModifer);

    float GetUnitFleetPointsModifer () const;
    void SetUnitFleetPointsModifer (float unitFleetPointsModifer);

    float GetUnitTradersPointsModifer () const;
    void SetUnitTradersPointsModifer (float unitTradersPointsModifer);

    float GetUnitColonizatorsPointsModifer () const;
    void SetUnitColonizatorsPointsModifer (float unitColonizatorsPointsModifer);

    float GetUnitArmyPointsModifer () const;
    void SetUnitArmyPointsModifer (float unitArmyPointsModifer);

    Urho3D::String GetVictoryTypesProcessorScriptPath () const;
    void SetVictoryTypesProcessorScriptPath (const Urho3D::String &victoryTypesProcessorScriptPath);
};
}
