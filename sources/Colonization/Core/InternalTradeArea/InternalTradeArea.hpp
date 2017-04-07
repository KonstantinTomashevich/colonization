#pragma once
#include <Urho3D/Core/Object.h>
#include <Urho3D/Container/Vector.h>
#include <Urho3D/Scene/Component.h>

#include <Colonization/Core/GameConfiguration.hpp>
#include <Colonization/Core/InternalTradeArea/DistrictProductionInfo.hpp>
#include <Colonization/Core/InternalTradeArea/DistrictProductionInfoComparator.hpp>
#include <Colonization/Core/InternalTradeArea/TradeDistrictProcessingInfo.hpp>

namespace Colonization
{
class InternalTradeArea : public Urho3D::Component
{
URHO3D_OBJECT (InternalTradeArea, Component)
protected:
    Urho3D::PODVector <Urho3D::StringHash> districtsHashes_;

    void ConstructVectorOfRealDistricts (Map *map, Urho3D::PODVector <District *> &output);
    int CalculateTotalSoldiersCount ();

    // TODO: Maybe create DistrictUtils and move these functions to it?
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
