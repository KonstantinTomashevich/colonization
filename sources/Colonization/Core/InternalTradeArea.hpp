#pragma once
#include <Urho3D/Core/Object.h>
#include <Urho3D/Container/Vector.h>
#include <Urho3D/Scene/Component.h>
#include <Colonization/Core/GameConfiguration.hpp>

namespace Colonization
{
// TODO: Maybe split to InternalTradeArea, DistrictProductionInfo, DistrictProductionInfoComparator, TradeDistrictProcessingInfo.
class Map;
class District;

const Urho3D::StringHash DISTRICT_PRODUCTION_AMOUNT_KEY ("DistrictProductionAmount");
const Urho3D::StringHash DISTRICT_PRODUCTION_RELATIVE_PRICE_KEY ("DistrictProductionRelativePrice");
const Urho3D::StringHash DISTRICT_PRODUCTION_QUALITY_KEY ("DistrictProductionQuality");
const Urho3D::StringHash DISTRICT_PRODUCTION_SELLABILITY_KEY ("DistrictProductionSellability");
const Urho3D::StringHash DISTRICT_PRODUCTION_SELLED_KEY ("DistrictProductionSelled");

struct DistrictProductionInfo
{
    Urho3D::StringHash districtHash_;
    float amount_;
    float relativePrice_;
    float quality_;
    float sellability_;
    float selled_;

    void CalculateSellability ();
    Urho3D::VariantMap ToVariantMap ();
};

typedef bool (*DistrictProductionInfoComparator) (DistrictProductionInfo &first, DistrictProductionInfo &second);
namespace DistrictProductionInfoComparators
{
bool HigherSellability (DistrictProductionInfo &first, DistrictProductionInfo &second);
}

class TradeDistrictProcessingInfo : public Urho3D::Object
{
URHO3D_OBJECT (TradeDistrictProcessingInfo, Object)
protected:
    Urho3D::HashMap <Urho3D::StringHash, float> unusedProduction_;
    Urho3D::HashMap <Urho3D::StringHash, Urho3D::VariantMap> districtsProduction_;
    Urho3D::HashMap <Urho3D::StringHash, Urho3D::VariantMap> districtsBalanceAdditions_;
    float unsoldTradeGoodsCost_;
    float soldTradeGoodsCost_;
public:
    TradeDistrictProcessingInfo (Urho3D::Context *context);
    virtual ~TradeDistrictProcessingInfo ();

    float GetUnusedProductionOf (Urho3D::StringHash type) const;
    void SetUnusedProductionOf (Urho3D::StringHash type, float points);

    Urho3D::VariantMap GetDistrictProduction (Urho3D::StringHash districtHash) const;
    void SetDistrictProduction (Urho3D::StringHash districtHash, Urho3D::VariantMap &districtProduction);

    Urho3D::VariantMap GetDistrictBalanceAdditions(Urho3D::StringHash districtHash) const;
    void SetDistrictBalanceAdditions (Urho3D::StringHash districtHash, Urho3D::VariantMap &districtBalanceAdditions);

    float GetUnsoldTradeGoodsCost () const;
    void SetUnsoldTradeGoodsCost (float unsoldTradeGoodsCost);

    float GetSoldTradeGoodsCost () const;
    void SetSoldTradeGoodsCost (float soldTradeGoodsCost);
};

class InternalTradeArea : public Urho3D::Component
{
URHO3D_OBJECT (InternalTradeArea, Component)
protected:
    Urho3D::PODVector <Urho3D::StringHash> districtsHashes_;

    void ConstructVectorOfRealDistricts (Map *map, Urho3D::PODVector <District *> &output);
    int CalculateTotalSoldiersCount ();

    float CalculateDistrictFarmsProductionAmount (District *district, GameConfiguration *configuration);
    float CalculateDistrictMinesProductionAmount (District *district, GameConfiguration *configuration);
    float CalculateDistrictIndustryProductionAmount (District *district, GameConfiguration *configuration);

    DistrictProductionInfo CalculateDistrictProductionOfFarms (District *district, GameConfiguration *configuration);
    DistrictProductionInfo CalculateDistrictProductionOfMines (District *district, GameConfiguration *configuration);
    DistrictProductionInfo CalculateDistrictProductionOfIndustry (District *district, GameConfiguration *configuration);

    float CalculateDistrictFarmsProductionRelativePrice (District *district, GameConfiguration *configuration);
    float CalculateDistrictMinesProductionRelativePrice (District *district, GameConfiguration *configuration);
    float CalculateDistrictIndustryProductionRelativePrice (District *district, GameConfiguration *configuration);

    float CalculateDistrictFarmsProductionQuality (District *district, GameConfiguration *configuration);
    float CalculateDistrictMinesProductionQuality (District *district, GameConfiguration *configuration);
    float CalculateDistrictIndustryProductionQuality (District *district, GameConfiguration *configuration);

    void CalculateTotalProductionOfFarms (Urho3D::PODVector <District *> &realDistricts, GameConfiguration *configuration, Urho3D::Vector <DistrictProductionInfo> &output);
    void CalculateTotalProductionOfMines (Urho3D::PODVector <District *> &realDistricts, GameConfiguration *configuration, Urho3D::Vector <DistrictProductionInfo> &output);
    void CalculateTotalProductionOfIndustry (Urho3D::PODVector <District *> &realDistricts, GameConfiguration *configuration, Urho3D::Vector <DistrictProductionInfo> &output);

    float CalculateTotalProductionConsumptionOfFarms (GameConfiguration *configuration, Urho3D::PODVector <District *> &realDistricts, int soldiersCount);
    float CalculateTotalProductionConsumptionOfMines (GameConfiguration *configuration, Urho3D::PODVector <District *> &realDistricts, int soldiersCount);
    float CalculateTotalProductionConsumptionOfIndustry (GameConfiguration *configuration, Urho3D::PODVector <District *> &realDistricts, int soldiersCount);

    float CalculateDistrictProductionConsumptionOfFarms (GameConfiguration *configuration, District *district);
    float CalculateDistrictProductionConsumptionOfMines (GameConfiguration *configuration, District *district);
    float CalculateDistrictProductionConsumptionOfIndustry (GameConfiguration *configuration, District *district);

    /// This function return unsatisfied consumption or 0.0f.
    float ConsumeProduction (float consumption, Urho3D::Vector <DistrictProductionInfo> &production);
    float CalculateTotalProduction (Urho3D::Vector <DistrictProductionInfo> &production);

    void ProcessFarmsBalance (Map *map, GameConfiguration *configuration, Urho3D::Vector <DistrictProductionInfo> &farmsProduction, float updateDelay,
                              bool writeDistrictsBalance, Urho3D::HashMap <Urho3D::StringHash, Urho3D::VariantMap> &districtsBalanceAdditions);
    void ProcessMinesBalance (Map *map, GameConfiguration *configuration, Urho3D::Vector <DistrictProductionInfo> &minesProduction, float updateDelay,
                              bool writeDistrictsBalance, Urho3D::HashMap <Urho3D::StringHash, Urho3D::VariantMap> &districtsBalanceAdditions);
    void ProcessIndustryBalance (Map *map, GameConfiguration *configuration, Urho3D::Vector <DistrictProductionInfo> &industryProduction, float updateDelay,
                                 bool writeDistrictsBalance, Urho3D::HashMap <Urho3D::StringHash, Urho3D::VariantMap> &districtsBalanceAdditions);
    void ProcessDistrictsProductionInfo (Urho3D::Vector <DistrictProductionInfo> &production, Urho3D::StringHash productionType,
                                         Urho3D::HashMap <Urho3D::StringHash, Urho3D::VariantMap> &districtsProductionInfo,
                                         float updateDelay);

    void WriteDistrictsBalanceAdditions (Map *map, Urho3D::SharedPtr <TradeDistrictProcessingInfo> result,
                                         Urho3D::HashMap <Urho3D::StringHash, Urho3D::VariantMap> &districtsBalanceAdditions,
                                         bool changeDistrictsVars);
    void WriteDistrictsProduction (Map *map, Urho3D::SharedPtr <TradeDistrictProcessingInfo> result,
                                   Urho3D::HashMap <Urho3D::StringHash, Urho3D::VariantMap> &districtsProduction, bool changeDistrictsVars);
    void CallDistrictsUpdate (Urho3D::PODVector<District *> &districts);

    float CalculateSoldFarmsProductionCost (Urho3D::Vector <DistrictProductionInfo> &production, GameConfiguration *configuration);
    float CalculateSoldMinesProductionCost (Urho3D::Vector <DistrictProductionInfo> &production, GameConfiguration *configuration);
    float CalculateSoldIndustryProductionCost (Urho3D::Vector <DistrictProductionInfo> &production, GameConfiguration *configuration);

    float CalculateUnsoldFarmsProductionCost (Urho3D::Vector <DistrictProductionInfo> &production, GameConfiguration *configuration);
    float CalculateUnsoldMinesProductionCost (Urho3D::Vector <DistrictProductionInfo> &production, GameConfiguration *configuration);
    float CalculateUnsoldIndustryProductionCost (Urho3D::Vector <DistrictProductionInfo> &production, GameConfiguration *configuration);

public:
    InternalTradeArea (Urho3D::Context *context);
    virtual ~InternalTradeArea ();

    virtual void DrawDebugGeometry (Urho3D::DebugRenderer *debug, bool depthTest);
    static void RegisterObject (Urho3D::Context *context);
    Urho3D::SharedPtr <TradeDistrictProcessingInfo> ProcessTrade (Map *map, float updateDelay, bool changeDistrictsVars);

    int GetDistrictsHashesCount ();
    Urho3D::StringHash GetDistrictHashByIndex (int index);
    Urho3D::PODVector <Urho3D::StringHash> GetDistrictsHashesArray ();
    void AddDistrictHash (Urho3D::StringHash districtHash);
    bool ContainsDistrictHash (Urho3D::StringHash districtHash);
    bool RemoveDistrictHash (Urho3D::StringHash districtHash);

    Urho3D::VariantVector GetDistrictsHashesArrayAttribute () const;
    void SetDistrictsHashesArrayAttribute (const Urho3D::VariantVector &attribute);
};
}
