#pragma once
#include <Urho3D/Core/Object.h>
#include <Urho3D/Container/Vector.h>
#include <Urho3D/Scene/Component.h>

namespace Colonization
{
class Map;
class District;

class TradeDistrictProcessingInfo : public Urho3D::Object
{
URHO3D_OBJECT (TradeDistrictProcessingInfo, Object)
public:
    TradeDistrictProcessingInfo (Urho3D::Context *context);
    virtual ~TradeDistrictProcessingInfo ();

    Urho3D::HashMap <Urho3D::StringHash, float> unusedEvolutionPoints_;
    float unsoldTradeGoodsCost_;
    float soldTradeGoodsCost_;
    float logisticsBonus_;
    float defenseBonus_;
};

// TODO: Maybe reimplement later to add some features?
class InternalTradeArea : public Urho3D::Component
{
URHO3D_OBJECT (InternalTradeArea, Component)
protected:
    Urho3D::PODVector <Urho3D::StringHash> districtsHashes_;

    void ConstructVectorOfRealDistricts (Map *map, Urho3D::PODVector <District *> &output);
    float CalculateTotalEvolutionOf (Urho3D::StringHash evolutionBranch, Urho3D::PODVector <District *> &realDistricts);
    float CalculateTotalProductionConsumptionOf (Urho3D::StringHash evolutionBranch, Urho3D::PODVector <District *> &realDistricts);

    float CalculateDistrictProductionConsumptionOfFarms (District *district);
    float CalculateDistrictProductionConsumptionOfMines (District *district);
    float CalculateDistrictProductionConsumptionOfIndustry (District *district);

public:
    InternalTradeArea (Urho3D::Context *context);
    virtual ~InternalTradeArea ();
    TradeDistrictProcessingInfo ProcessTrade (Map *map);

    int GetDistrictsHashesCount ();
    Urho3D::StringHash GetDistrictHashByIndex (int index);
    Urho3D::PODVector <Urho3D::StringHash> GetDistrictsHashesArray ();
    void AddDistrictHash (Urho3D::StringHash districtHash);
    bool ContainsDistrictHash (Urho3D::StringHash districtHash);
};
}
