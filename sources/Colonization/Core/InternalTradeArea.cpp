#include <Colonization/BuildConfiguration.hpp>
#include "InternalTradeArea.hpp"
#include <Colonization/Core/District.hpp>
#include <Colonization/Core/Map.hpp>
#include <Colonization/Utils/Categories.hpp>

#include <Urho3D/Core/Context.h>
#include <Urho3D/IO/Log.h>
#include <Colonization/Utils/AttributeMacro.hpp>

namespace Colonization
{
const char *districtsHashesStructureElementsNames [] =
{
    "Districts In Area Count",
    "***District Hash",
    0
};

void InternalTradeArea::ConstructVectorOfRealDistricts (Map *map, Urho3D::PODVector <District *> &output)
{
    for (int index = 0; index < districtsHashes_.Size (); index++)
        output.Push (map->GetDistrictByHash (districtsHashes_.At (index)));
}

float InternalTradeArea::CalculateTotalEvolutionOf (Urho3D::StringHash evolutionBranch, Urho3D::PODVector <District *> &realDistricts)
{
    float totalEvolution = 0.0f;
    for (int index = 0; index < realDistricts.Size (); index++)
    {
        District *district = realDistricts.At (index);
        assert (district);
        if (evolutionBranch == Urho3D::StringHash ("farms"))
            totalEvolution += district->GetFarmsEvolutionPoints ();
        else if (evolutionBranch == Urho3D::StringHash ("mines"))
            totalEvolution += district->GetMinesEvolutionPoints ();
        else if (evolutionBranch == Urho3D::StringHash ("industry"))
            totalEvolution += district->GetIndustryEvolutionPoints ();
        else if (evolutionBranch == Urho3D::StringHash ("logistics"))
            totalEvolution += district->GetLogisticsEvolutionPoints ();
        else if (evolutionBranch == Urho3D::StringHash ("defense"))
            totalEvolution += district->GetDefenseEvolutionPoints ();
    }
    return totalEvolution;
}

float InternalTradeArea::CalculateTotalProductionConsumptionOf (Urho3D::StringHash evolutionBranch, Urho3D::PODVector <District *> &realDistricts)
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

    float totalConsumption = oneColinistConsumption * (district->GetMenCount () + district->GetWomenCount ());
    totalConsumption += minesConsumption * district->GetMinesEvolutionPoints ();
    totalConsumption += industryConsumption * district->GetIndustryEvolutionPoints ();
    return totalConsumption * Urho3D::Random (0.9f, 1.1f);
}

float InternalTradeArea::CalculateDistrictProductionConsumptionOfMines (District *district)
{
    float oneColinistConsumption = context_->GetGlobalVar ("oneColonistMinesProductionConsumption").GetFloat ();
    float farmsConsumption = context_->GetGlobalVar ("minesProductionFarmsConsumption").GetFloat ();
    float industryConsumption = context_->GetGlobalVar ("minesProductionIndustryConsumption").GetFloat ();

    float totalConsumption = oneColinistConsumption * (district->GetMenCount () + district->GetWomenCount ());
    totalConsumption += farmsConsumption * district->GetFarmsEvolutionPoints ();
    totalConsumption += industryConsumption * district->GetIndustryEvolutionPoints ();
    return totalConsumption * Urho3D::Random (0.9f, 1.1f);
}

float InternalTradeArea::CalculateDistrictProductionConsumptionOfIndustry (District *district)
{
    float oneColinistConsumption = context_->GetGlobalVar ("oneColonistIndustryProductionConsumption").GetFloat ();
    float farmsConsumption = context_->GetGlobalVar ("industryProductionFarmsConsumption").GetFloat ();
    float minesConsumption = context_->GetGlobalVar ("industryProductionMinesConsumption").GetFloat ();

    float totalConsumption = oneColinistConsumption * (district->GetMenCount () + district->GetWomenCount ());
    totalConsumption += farmsConsumption * district->GetFarmsEvolutionPoints ();
    totalConsumption += minesConsumption * district->GetMinesEvolutionPoints ();
    return totalConsumption * Urho3D::Random (0.9f, 1.1f);
}

InternalTradeArea::InternalTradeArea (Urho3D::Context *context) : Urho3D::Component (context),
    districtsHashes_ ()
{

}

InternalTradeArea::~InternalTradeArea ()
{

}

void InternalTradeArea::RegisterObject (Urho3D::Context *context)
{
    context->RegisterFactory <InternalTradeArea> (COLONIZATION_CORE_CATEGORY);
    URHO3D_MIXED_ACCESSOR_VARIANT_VECTOR_STRUCTURE_ATTRIBUTE ("Districts hashes", GetDistrictsHashesArrayAttribute, SetDistrictsHashesArrayAttribute,
                                                              Urho3D::VariantVector, Urho3D::Variant::emptyVariantVector,
                                                              districtsHashesStructureElementsNames, Urho3D::AM_DEFAULT);
}

Urho3D::SharedPtr <TradeDistrictProcessingInfo> InternalTradeArea::ProcessTrade(Map *map)
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

    float farmsInternalProductionCost = context_->GetGlobalVar ("farmsProductionInternalCost").GetFloat ();
    float minesInternalProductionCost = context_->GetGlobalVar ("minesProductionInternalCost").GetFloat ();
    float industryInternalProductionCost = context_->GetGlobalVar ("industryProductionInternalCost").GetFloat ();

    float farmsExternalProductionCost = context_->GetGlobalVar ("farmsProductionExternalCost").GetFloat ();
    float minesExternalProductionCost = context_->GetGlobalVar ("minesProductionExternalCost").GetFloat ();
    float industryExternalProductionCost = context_->GetGlobalVar ("industryProductionExternalCost").GetFloat ();

    Urho3D::SharedPtr <TradeDistrictProcessingInfo> result (new TradeDistrictProcessingInfo (context_));
    result->SetUnusedEvolutionPointsOf ("farms", totalFarmsEvolution - totalFarmsConsumption);
    result->SetUnusedEvolutionPointsOf ("mines", totalMinesEvolution - totalMinesConsumption);
    result->SetUnusedEvolutionPointsOf ("industry", totalIndustryEvolution - totalIndustryConsumption);

    float soldFarmsProduction = (totalFarmsConsumption > totalFarmsEvolution) ? totalFarmsEvolution : totalFarmsConsumption;
    float soldMinesProduction = (totalMinesConsumption > totalMinesEvolution) ? totalMinesEvolution : totalMinesConsumption;
    float soldIndustryProduction = (totalIndustryConsumption > totalIndustryEvolution) ? totalIndustryEvolution : totalIndustryConsumption;

    result->SetSoldTradeGoodsCost (soldFarmsProduction * farmsInternalProductionCost +
                                   soldMinesProduction * minesInternalProductionCost +
                                   soldIndustryProduction * industryInternalProductionCost);

    result->SetUnsoldTradeGoodsCost ( (totalFarmsEvolution - soldFarmsProduction) * farmsExternalProductionCost +
                                      (totalMinesEvolution - soldMinesProduction) * minesExternalProductionCost +
                                      (totalIndustryEvolution - soldIndustryProduction) * industryExternalProductionCost);

    result->SetLogisticsBonus (totalLogisticsEvolution / (districtsHashes_.Size () * 6.5f));
    result->SetDefenseBonus (totalDefenseEvolution / (districtsHashes_.Size () * 5.0f));
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

bool InternalTradeArea::RemoveDistrictHash (Urho3D::StringHash districtHash)
{
    return districtsHashes_.Remove (districtHash);
}

Urho3D::VariantVector InternalTradeArea::GetDistrictsHashesArrayAttribute () const
{
    Urho3D::VariantVector variantVector;
    variantVector.Push (districtsHashes_.Size ());
    for (int index = 0; index < districtsHashes_.Size (); index++)
        variantVector.Push (Urho3D::Variant (districtsHashes_.At (index)));
    return variantVector;
}

void InternalTradeArea::SetDistrictsHashesArrayAttribute (const Urho3D::VariantVector &attribute)
{
    districtsHashes_.Clear ();
    if (attribute.Size () > 1)
        for (int index = 1; index < attribute.Size (); index++)
            districtsHashes_.Push (attribute.At (index).GetStringHash ());
}

TradeDistrictProcessingInfo::TradeDistrictProcessingInfo (Urho3D::Context *context) : Urho3D::Object (context),
    unusedEvolutionPoints_ (),
    unsoldTradeGoodsCost_ (0.0f),
    soldTradeGoodsCost_ (0.0f),
    logisticsBonus_ (1.0f),
    defenseBonus_ (1.0f)
{

}

TradeDistrictProcessingInfo::~TradeDistrictProcessingInfo ()
{

}

float TradeDistrictProcessingInfo::GetUnusedEvolutionPointsOf (Urho3D::StringHash type)
{
    return unusedEvolutionPoints_ [type];
}

void TradeDistrictProcessingInfo::SetUnusedEvolutionPointsOf (Urho3D::StringHash type, float points)
{
    unusedEvolutionPoints_ [type] = points;
}

float TradeDistrictProcessingInfo::GetUnsoldTradeGoodsCost ()
{
    return unsoldTradeGoodsCost_;
}

void TradeDistrictProcessingInfo::SetUnsoldTradeGoodsCost (float unsoldTradeGoodsCost)
{
    unsoldTradeGoodsCost_ = unsoldTradeGoodsCost;
}

float TradeDistrictProcessingInfo::GetSoldTradeGoodsCost ()
{
    return soldTradeGoodsCost_;
}

void TradeDistrictProcessingInfo::SetSoldTradeGoodsCost (float soldTradeGoodsCost)
{
    soldTradeGoodsCost_ = soldTradeGoodsCost;
}

float TradeDistrictProcessingInfo::GetLogisticsBonus ()
{
    return logisticsBonus_;
}

void TradeDistrictProcessingInfo::SetLogisticsBonus (float logisticsBonus)
{
    logisticsBonus_ = logisticsBonus;
}

float TradeDistrictProcessingInfo::GetDefenseBonus ()
{
    return defenseBonus_;
}

void TradeDistrictProcessingInfo::SetDefenseBonus (float defenseBonus)
{
    defenseBonus_ = defenseBonus;
}
}
