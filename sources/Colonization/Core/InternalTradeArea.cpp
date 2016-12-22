#include <Colonization/BuildConfiguration.hpp>
#include "InternalTradeArea.hpp"
#include <Colonization/Core/District.hpp>
#include <Colonization/Core/Map.hpp>
#include <Urho3D/Core/Context.h>

namespace Colonization
{
void InternalTradeArea::ConstructVectorOfRealDistricts (Map *map, Urho3D::PODVector <District *> &output)
{
    for (int index = 0; index < districtsHashes_.Size (); index++)
        output.Push (map->GetDistrictByHash (districtsHashes_.At (index)));
}

float InternalTradeArea::CalculateTotalEvolutionOf (Urho3D::StringHash evolutionBranch, Urho3D::PODVector<District *> &realDistricts)
{
    float totalEvolution = 0.0f;
    for (int index = 0; index < realDistricts.Size (); index++)
    {
        District *district = realDistricts.At (index);
        assert (district);
        if (evolutionBranch == Urho3D::StringHash ("farms"))
            totalEvolution += district->farmsEvolutionPoints_;
        else if (evolutionBranch == Urho3D::StringHash ("mines"))
            totalEvolution += district->minesEvolutionPoints_;
        else if (evolutionBranch == Urho3D::StringHash ("industry"))
            totalEvolution += district->industryEvolutionPoints_;
        else if (evolutionBranch == Urho3D::StringHash ("logistics"))
            totalEvolution += district->logisticsEvolutionPoints_;
        else if (evolutionBranch == Urho3D::StringHash ("defense"))
            totalEvolution += district->defenseEvolutionPoints_;
    }
    return totalEvolution;
}

float InternalTradeArea::CalculateTotalProductionConsumptionOf (Urho3D::StringHash evolutionBranch, Urho3D::PODVector<District *> &realDistricts)
{
    float totalConsumption = 0.0f;
    for (int index = 0; index < realDistricts.Size (); index++)
    {
        District *district = realDistricts.At (index);
        assert (district);
        if (evolutionBranch == Urho3D::StringHash ("farms"))
            totalConsumption += CalculateDistrictProductionConsumptionOfFarms (district);
        else if (evolutionBranch == Urho3D::StringHash ("mines"))
            totalConsumption += CalculateDistrictProductionConsumptionOfMines (district);
        else if (evolutionBranch == Urho3D::StringHash ("industry"))
            totalConsumption += CalculateDistrictProductionConsumptionOfIndustry (district);
    }
    return totalConsumption;
}

float InternalTradeArea::CalculateDistrictProductionConsumptionOfFarms (District *district)
{
    float oneColinistConsumption = context_->GetGlobalVar ("oneColonistFarmsProductionConsumption").GetFloat ();
    float minesConsumption = context_->GetGlobalVar ("farmsProductionMinesConsumption").GetFloat ();
    float industryConsumption = context_->GetGlobalVar ("farmsProductionIndustryConsumption").GetFloat ();

    float totalConsumption = oneColinistConsumption * (district->mansCount_ + district->womenCount_);
    totalConsumption += minesConsumption * district->minesEvolutionPoints_;
    totalConsumption += industryConsumption * district->industryEvolutionPoints_;
    return totalConsumption * Urho3D::Random (0.9f, 1.1f);
}

float InternalTradeArea::CalculateDistrictProductionConsumptionOfMines (District *district)
{
    float oneColinistConsumption = context_->GetGlobalVar ("oneColonistMinesProductionConsumption").GetFloat ();
    float farmsConsumption = context_->GetGlobalVar ("minesProductionFarmsConsumption").GetFloat ();
    float industryConsumption = context_->GetGlobalVar ("minesProductionIndustryConsumption").GetFloat ();

    float totalConsumption = oneColinistConsumption * (district->mansCount_ + district->womenCount_);
    totalConsumption += farmsConsumption * district->farmsEvolutionPoints_;
    totalConsumption += industryConsumption * district->industryEvolutionPoints_;
    return totalConsumption * Urho3D::Random (0.9f, 1.1f);
}

float InternalTradeArea::CalculateDistrictProductionConsumptionOfIndustry (District *district)
{
    float oneColinistConsumption = context_->GetGlobalVar ("oneColonistIndustryProductionConsumption").GetFloat ();
    float farmsConsumption = context_->GetGlobalVar ("industryProductionFarmsConsumption").GetFloat ();
    float minesConsumption = context_->GetGlobalVar ("industryProductionMinesConsumption").GetFloat ();

    float totalConsumption = oneColinistConsumption * (district->mansCount_ + district->womenCount_);
    totalConsumption += farmsConsumption * district->farmsEvolutionPoints_;
    totalConsumption += minesConsumption * district->minesEvolutionPoints_;
    return totalConsumption * Urho3D::Random (0.9f, 1.1f);
}

InternalTradeArea::InternalTradeArea (Urho3D::Context *context) : Urho3D::Object (context),
    districtsHashes_ ()
{

}

InternalTradeArea::~InternalTradeArea ()
{

}

TradeDistrictProcessingInfo InternalTradeArea::ProcessTradeDistrict (Map *map)
{
    Urho3D::PODVector <District *> realDistricts;
    ConstructVectorOfRealDistricts (map, realDistricts);

    float totalFarmsEvolution = CalculateTotalEvolutionOf ("farms", realDistricts);
    float totalMinesEvolution = CalculateTotalEvolutionOf ("mines", realDistricts);
    float totalIndustryEvolution = CalculateTotalEvolutionOf ("industry", realDistricts);
    float totalLogisticsEvolution = CalculateTotalEvolutionOf ("logistics", realDistricts);
    float totalDefenseEvolution = CalculateTotalEvolutionOf ("defense", realDistricts);

    float totalFarmsConsumption = CalculateTotalProductionConsumptionOf ("farms", realDistricts);
    float totalMinesConsumption = CalculateTotalProductionConsumptionOf ("mines", realDistricts);
    float totalIndustryConsumption = CalculateTotalProductionConsumptionOf ("industry", realDistricts);

    float farmsProductionCost = context_->GetGlobalVar ("farmsProductionInternalCost").GetFloat ();
    float minesProductionCost = context_->GetGlobalVar ("minesProductionInternalCost").GetFloat ();
    float industryProductionCost = context_->GetGlobalVar ("industryProductionInternalCost").GetFloat ();

    TradeDistrictProcessingInfo result;
    result.unusedEvolutionPoints_ ["farms"] = totalFarmsEvolution - totalFarmsConsumption;
    result.unusedEvolutionPoints_ ["mines"] = totalMinesEvolution - totalMinesConsumption;
    result.unusedEvolutionPoints_ ["industry"] = totalIndustryEvolution - totalIndustryConsumption;

    float soldFarmsProduction = (totalFarmsConsumption > totalFarmsEvolution) ? totalFarmsEvolution : totalFarmsConsumption;
    float soldMinesProduction = (totalMinesConsumption > totalMinesEvolution) ? totalMinesEvolution : totalMinesConsumption;
    float soldIndustryProduction = (totalIndustryConsumption > totalIndustryEvolution) ? totalIndustryEvolution : totalIndustryConsumption;

    result.soldTradeGoodsCost_ = soldFarmsProduction * farmsProductionCost +
            soldMinesProduction * minesProductionCost +
            soldIndustryProduction * industryProductionCost;

    result.unsoldTradeGoodsCost_ = totalFarmsEvolution * farmsProductionCost +
            totalMinesEvolution * minesProductionCost +
            totalIndustryConsumption * industryProductionCost -
            result.soldTradeGoodsCost_;

    result.logisticsBonus_ = totalLogisticsEvolution / districtsHashes_.Size () * 6.5f;
    result.defenseBonus_ = totalDefenseEvolution / districtsHashes_.Size () * 5.0f;
    return result;
}

int InternalTradeArea::GetDistrictsHashesCount ()
{
    return districtsHashes_.Size ();
}

Urho3D::StringHash InternalTradeArea::GetDistrictHashByIndex (int index)
{
    assert (index < districtsHashes_.Size ());
    return districtsHashes_.At (index);
}

Urho3D::PODVector<Urho3D::StringHash> InternalTradeArea::GetDistrictsHashesArray ()
{
    return districtsHashes_;
}

void InternalTradeArea::AddDistrictHash (Urho3D::StringHash districtHash)
{
    assert (!districtsHashes_.Contains (districtHash));
    districtsHashes_.Push (districtHash);
}

bool InternalTradeArea::ContainsDistrictHash (Urho3D::StringHash districtHash)
{
    return districtsHashes_.Contains (districtHash);
}
}
