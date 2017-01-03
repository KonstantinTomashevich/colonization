#pragma once
#include <Urho3D/Core/Object.h>
#include <Urho3D/Container/Vector.h>
#include <Urho3D/Scene/Component.h>
#include <Colonization/Core/GameConfiguration.hpp>

namespace Colonization
{
class Map;
class District;

class TradeDistrictProcessingInfo : public Urho3D::Object
{
URHO3D_OBJECT (TradeDistrictProcessingInfo, Object)
protected:
    Urho3D::HashMap <Urho3D::StringHash, float> unusedEvolutionPoints_;
    float unsoldTradeGoodsCost_;
    float soldTradeGoodsCost_;
    float logisticsBonus_;
    float defenseBonus_;
public:
    TradeDistrictProcessingInfo (Urho3D::Context *context);
    virtual ~TradeDistrictProcessingInfo ();

    float GetUnusedEvolutionPointsOf (Urho3D::StringHash type);
    void SetUnusedEvolutionPointsOf (Urho3D::StringHash type, float points);

    float GetUnsoldTradeGoodsCost ();
    void SetUnsoldTradeGoodsCost (float unsoldTradeGoodsCost);

    float GetSoldTradeGoodsCost ();
    void SetSoldTradeGoodsCost (float soldTradeGoodsCost);

    float GetLogisticsBonus ();
    void SetLogisticsBonus (float logisticsBonus);

    float GetDefenseBonus ();
    void SetDefenseBonus (float defenseBonus);
};

// TODO: Maybe reimplement later to add some features?
class InternalTradeArea : public Urho3D::Component
{
URHO3D_OBJECT (InternalTradeArea, Component)
protected:
    Urho3D::PODVector <Urho3D::StringHash> districtsHashes_;

    void ConstructVectorOfRealDistricts (Map *map, Urho3D::PODVector <District *> &output);
    float CalculateTotalEvolutionOf (Urho3D::StringHash evolutionBranch, Urho3D::PODVector <District *> &realDistricts);
    float CalculateTotalProductionConsumptionOf (GameConfiguration *configuration, Urho3D::StringHash evolutionBranch, Urho3D::PODVector <District *> &realDistricts);

    float CalculateDistrictProductionConsumptionOfFarms (GameConfiguration *configuration, District *district);
    float CalculateDistrictProductionConsumptionOfMines (GameConfiguration *configuration, District *district);
    float CalculateDistrictProductionConsumptionOfIndustry (GameConfiguration *configuration, District *district);

public:
    InternalTradeArea (Urho3D::Context *context);
    virtual ~InternalTradeArea ();

    virtual void DrawDebugGeometry (Urho3D::DebugRenderer *debug, bool depthTest);
    static void RegisterObject (Urho3D::Context *context);
    Urho3D::SharedPtr <TradeDistrictProcessingInfo> ProcessTrade (Map *map);

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
