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
//@ASBindGen Class ObjectType=Ref
class InternalTradeArea : public Urho3D::Component
{
URHO3D_OBJECT (InternalTradeArea, Component)
protected:
    Urho3D::PODVector <Urho3D::StringHash> districtsHashes_;

    void ConstructVectorOfRealDistricts (Map *map, Urho3D::PODVector <District *> &output);
    int CalculateTotalSoldiersCount ();

    void CalculateTotalProductionOfFarms (Urho3D::PODVector <District *> &realDistricts, GameConfiguration *configuration, Urho3D::Vector <DistrictProductionInfo> &output);
    void CalculateTotalProductionOfMines (Urho3D::PODVector <District *> &realDistricts, GameConfiguration *configuration, Urho3D::Vector <DistrictProductionInfo> &output);
    void CalculateTotalProductionOfIndustry (Urho3D::PODVector <District *> &realDistricts, GameConfiguration *configuration, Urho3D::Vector <DistrictProductionInfo> &output);

    float CalculateTotalProductionConsumptionOfFarms (GameConfiguration *configuration, Urho3D::PODVector <District *> &realDistricts, int soldiersCount);
    float CalculateTotalProductionConsumptionOfMines (GameConfiguration *configuration, Urho3D::PODVector <District *> &realDistricts, int soldiersCount);
    float CalculateTotalProductionConsumptionOfIndustry (GameConfiguration *configuration, Urho3D::PODVector <District *> &realDistricts, int soldiersCount);

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

    void WriteDistrictsBalanceAdditions (Map *map, TradeDistrictProcessingInfo *result,
                                         Urho3D::HashMap <Urho3D::StringHash, Urho3D::VariantMap> &districtsBalanceAdditions,
                                         bool changeDistrictsVars);
    void WriteDistrictsProduction (Map *map, TradeDistrictProcessingInfo *result,
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
    //@ASBindGen Function AddRef_arg-1
    TradeDistrictProcessingInfo *ProcessTrade (Map *map, float updateDelay, bool changeDistrictsVars);

    //@ASBindGen Function OverrideName=get_districtsHashesCount
    int GetDistrictsHashesCount ();
    //@ASBindGen Function
    Urho3D::StringHash GetDistrictHashByIndex (int index);
    //@ASBindGen Function
    Urho3D::PODVector <Urho3D::StringHash> GetDistrictsHashesArray ();
    //@ASBindGen Function
    void AddDistrictHash (Urho3D::StringHash districtHash);
    //@ASBindGen Function
    bool ContainsDistrictHash (Urho3D::StringHash districtHash);
    //@ASBindGen Function
    bool RemoveDistrictHash (Urho3D::StringHash districtHash);

    Urho3D::VariantVector GetDistrictsHashesArrayAttribute () const;
    void SetDistrictsHashesArrayAttribute (const Urho3D::VariantVector &attribute);
};
}
