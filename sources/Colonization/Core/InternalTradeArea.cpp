#include <Colonization/BuildConfiguration.hpp>
#include "InternalTradeArea.hpp"
#include <Colonization/Core/District.hpp>
#include <Colonization/Core/Map.hpp>
#include <Colonization/Utils/Serialization/Categories.hpp>
#include <Colonization/Backend/UnitsManager.hpp>

#include <cmath>
#include <Urho3D/Core/Context.h>
#include <Urho3D/IO/Log.h>
#include <Urho3D/Scene/Scene.h>
#include <Colonization/Utils/Serialization/AttributeMacro.hpp>

namespace Colonization
{
const char *districtsHashesStructureElementsNames [] =
{
    "Districts In Area Count",
    "   District Hash",
    0
};

void InternalTradeArea::ConstructVectorOfRealDistricts (Map *map, Urho3D::PODVector <District *> &output)
{
    for (int index = 0; index < districtsHashes_.Size (); index++)
    {
        output.Push (map->GetDistrictByHash (districtsHashes_.At (index)));
    }
}

float InternalTradeArea::CalculateTotalEvolutionOf (Urho3D::StringHash evolutionBranch, Urho3D::PODVector <District *> &realDistricts)
{
    float totalEvolution = 0.0f;
    for (int index = 0; index < realDistricts.Size (); index++)
    {
        District *district = realDistricts.At (index);
        assert (district);
        if (evolutionBranch == Urho3D::StringHash ("farms"))
        {
            totalEvolution += district->GetFarmsEvolutionPoints () * district->GetLandAverageFertility ();
        }
        else if (evolutionBranch == Urho3D::StringHash ("mines"))
        {
            totalEvolution += district->GetMinesEvolutionPoints ();
        }
        else if (evolutionBranch == Urho3D::StringHash ("industry"))
        {
            totalEvolution += district->GetIndustryEvolutionPoints ();
        }
        else if (evolutionBranch == Urho3D::StringHash ("logistics"))
        {
            totalEvolution += district->GetLogisticsEvolutionPoints ();
        }
        else if (evolutionBranch == Urho3D::StringHash ("defense"))
        {
            totalEvolution += district->GetDefenseEvolutionPoints ();
        }
    }
    return totalEvolution;
}

int InternalTradeArea::CalculateTotalSoldiersCount ()
{
    int totalSoldiersCount = 0;
    for (int districtIndex = 0; districtIndex < districtsHashes_.Size (); districtIndex++)
    {
        Urho3D::PODVector <Unit *> unitsInDistrict = GetUnitsInDistrict (node_->GetScene (), districtsHashes_.At (districtIndex));
        if (!unitsInDistrict.Empty ())
        {
            for (int unitIndex = 0; unitIndex < unitsInDistrict.Size (); unitIndex++)
            {
                Unit *unit = unitsInDistrict.At (unitIndex);
                if (unit->GetUnitType () == UNIT_ARMY)
                {
                    totalSoldiersCount += unit->ArmyUnitGetSoldiersCount ();
                }
            }
        }
    }
    return totalSoldiersCount;
}

float InternalTradeArea::CalculateTotalProductionOfFarms (Urho3D::PODVector <District *> &realDistricts, GameConfiguration *configuration)
{
    float totalProduction = 0.0f;
    for (int index = 0; index < realDistricts.Size (); index++)
    {
        District *district = realDistricts.At (index);
        assert (district);

        float climateModifer = 1.0f;
        if (district->GetClimate () == CLIMATE_TROPICAL)
        {
            climateModifer = configuration->GetFarmsProductionTropicalClimateModifer ();
        }
        else if (district->GetClimate () == CLIMATE_HOT)
        {
            climateModifer = configuration->GetFarmsProductionHotClimateModifer ();
        }
        else if (district->GetClimate () == CLIMATE_TEMPERATE)
        {
            climateModifer = configuration->GetFarmsProductionTemperateClimateModifer ();
        }
        else if (district->GetClimate () == CLIMATE_TEMPERATE_CONTINENTAL)
        {
            climateModifer = configuration->GetFarmsProductionTemperateContinentalClimateModifer ();
        }
        else if (district->GetClimate () == CLIMATE_DESERT)
        {
            climateModifer = configuration->GetFarmsProductionDesertClimateModifer ();
        }
        else if (district->GetClimate () == CLIMATE_COLD)
        {
            climateModifer = configuration->GetFarmsProductionColdClimateModifer ();
        }

        float districtPopulation = district->GetMenCount () + district->GetWomenCount ();
        totalProduction += districtPopulation * configuration->GetFarmsProductionPerColonist () *
                sqrt (district->GetFarmsEvolutionPoints ()) * district->GetLandAverageFertility () * climateModifer;
    }
    return totalProduction;
}

float InternalTradeArea::CalculateTotalProductionOfMines (Urho3D::PODVector <District *> &realDistricts, GameConfiguration *configuration)
{
    float totalProduction = 0.0f;
    for (int index = 0; index < realDistricts.Size (); index++)
    {
        District *district = realDistricts.At (index);
        assert (district);

        float districtPopulation = district->GetMenCount () + district->GetWomenCount ();
        totalProduction += districtPopulation * configuration->GetMinesProductionPerColonist () *
                sqrt (district->GetMinesEvolutionPoints ());
    }
    return totalProduction;
}

float InternalTradeArea::CalculateTotalProductionOfIndustry (Urho3D::PODVector <District *> &realDistricts, GameConfiguration *configuration)
{
    float totalProduction = 0.0f;
    for (int index = 0; index < realDistricts.Size (); index++)
    {
        District *district = realDistricts.At (index);
        assert (district);

        float districtPopulation = district->GetMenCount () + district->GetWomenCount ();
        totalProduction += districtPopulation * configuration->GetIndustryProductionPerColonist () *
                sqrt (district->GetIndustryEvolutionPoints ());
    }
    return totalProduction;
}

float InternalTradeArea::CalculateTotalProductionConsumptionOf (GameConfiguration *configuration, Urho3D::StringHash evolutionBranch, Urho3D::PODVector <District *> &realDistricts, int soldiersCount)
{
    float totalConsumption = 0.0f;
    for (int index = 0; index < realDistricts.Size (); index++)
    {
        District *district = realDistricts.At (index);
        assert (district);
        if (evolutionBranch == Urho3D::StringHash ("farms"))
        {
            totalConsumption += CalculateDistrictProductionConsumptionOfFarms (configuration, district, soldiersCount);
        }
        else if (evolutionBranch == Urho3D::StringHash ("mines"))
        {
            totalConsumption += CalculateDistrictProductionConsumptionOfMines (configuration, district, soldiersCount);
        }
        else if (evolutionBranch == Urho3D::StringHash ("industry"))
        {
            totalConsumption += CalculateDistrictProductionConsumptionOfIndustry (configuration, district, soldiersCount);
        }
    }
    return totalConsumption;
}

float InternalTradeArea::CalculateDistrictProductionConsumptionOfFarms (GameConfiguration *configuration, District *district, int soldiersCount)
{
    float oneColonistConsumption = configuration->GetOneColonistFarmsProductionConsumption ();
    float oneSoldierConsumption = configuration->GetOneSoldierFarmsProductionConsumption ();
    float minesConsumption = configuration->GetFarmsProductionMinesConsumption ();
    float industryConsumption = configuration->GetFarmsProductionIndustryConsumption ();

    float totalConsumption = oneColonistConsumption * (district->GetMenCount () + district->GetWomenCount ());
    totalConsumption += oneSoldierConsumption * soldiersCount;
    totalConsumption += minesConsumption * district->GetMinesEvolutionPoints ();
    totalConsumption += industryConsumption * district->GetIndustryEvolutionPoints ();
    return totalConsumption;
}

float InternalTradeArea::CalculateDistrictProductionConsumptionOfMines (GameConfiguration *configuration, District *district, int soldiersCount)
{
    float oneColonistConsumption = configuration->GetOneColonistMinesProductionConsumption ();
    float oneSoldierConsumption = configuration->GetOneSoldierMinesProductionConsumption ();
    float farmsConsumption = configuration->GetMinesProductionFarmsConsumption ();
    float industryConsumption = configuration->GetMinesProductionIndustryConsumption ();

    float totalConsumption = oneColonistConsumption * (district->GetMenCount () + district->GetWomenCount ());
    totalConsumption += oneSoldierConsumption * soldiersCount;
    totalConsumption += farmsConsumption * district->GetFarmsEvolutionPoints ();
    totalConsumption += industryConsumption * district->GetIndustryEvolutionPoints ();
    return totalConsumption;
}

float InternalTradeArea::CalculateDistrictProductionConsumptionOfIndustry (GameConfiguration *configuration, District *district, int soldiersCount)
{
    float oneColonistConsumption = configuration->GetOneColonistIndustryProductionConsumption ();
    float oneSoldierConsumption = configuration->GetOneSoldierIndustryProductionConsumption ();
    float farmsConsumption = configuration->GetIndustryProductionFarmsConsumption ();
    float minesConsumption = configuration->GetIndustryProductionMinesConsumption ();

    float totalConsumption = oneColonistConsumption * (district->GetMenCount () + district->GetWomenCount ());
    totalConsumption += oneSoldierConsumption * soldiersCount;
    totalConsumption += farmsConsumption * district->GetFarmsEvolutionPoints ();
    totalConsumption += minesConsumption * district->GetMinesEvolutionPoints ();
    return totalConsumption;
}

InternalTradeArea::InternalTradeArea (Urho3D::Context *context) : Urho3D::Component (context),
    districtsHashes_ ()
{

}

InternalTradeArea::~InternalTradeArea ()
{

}

void InternalTradeArea::DrawDebugGeometry (Urho3D::DebugRenderer *debug, bool depthTest)
{
    Map *map = node_->GetScene ()->GetChild ("map")->GetComponent <Map> ();
    assert (map);

    if (!districtsHashes_.Empty ())
    {
        for (int index = 0; index < districtsHashes_.Size (); index++)
        {
            District *district = map->GetDistrictByHash (districtsHashes_.At (index));
            if (district)
            {
                district->DrawDebugGeometry (debug, depthTest);
            }
        }
    }
}

void InternalTradeArea::RegisterObject (Urho3D::Context *context)
{
    context->RegisterFactory <InternalTradeArea> (COLONIZATION_CORE_CATEGORY);
    URHO3D_ACCESSOR_ATTRIBUTE ("Is Enabled", IsEnabled, SetEnabled, bool, true, Urho3D::AM_DEFAULT);
    URHO3D_MIXED_ACCESSOR_VARIANT_VECTOR_STRUCTURE_ATTRIBUTE ("Districts hashes", GetDistrictsHashesArrayAttribute, SetDistrictsHashesArrayAttribute,
                                                              Urho3D::VariantVector, Urho3D::Variant::emptyVariantVector,
                                                              districtsHashesStructureElementsNames, Urho3D::AM_DEFAULT);
}

Urho3D::SharedPtr <TradeDistrictProcessingInfo> InternalTradeArea::ProcessTrade (Map *map)
{
    GameConfiguration *configuration = node_->GetScene ()->GetComponent <GameConfiguration> ();
    assert (configuration);
    Urho3D::PODVector <District *> realDistricts;
    ConstructVectorOfRealDistricts (map, realDistricts);
    int totalSoldiersCount = CalculateTotalSoldiersCount ();

    float totalFarmsEvolution = CalculateTotalProductionOfFarms (realDistricts, configuration);
    float totalMinesEvolution = CalculateTotalProductionOfMines (realDistricts, configuration);
    float totalIndustryEvolution = CalculateTotalProductionOfIndustry (realDistricts, configuration);

    float totalFarmsConsumption = CalculateTotalProductionConsumptionOf (configuration, "farms", realDistricts, totalSoldiersCount);
    float totalMinesConsumption = CalculateTotalProductionConsumptionOf (configuration, "mines", realDistricts, totalSoldiersCount);
    float totalIndustryConsumption = CalculateTotalProductionConsumptionOf (configuration, "industry", realDistricts, totalSoldiersCount);

    float farmsInternalProductionCost = configuration->GetFarmsProductionInternalCost ();
    float minesInternalProductionCost = configuration->GetMinesProductionInternalCost ();
    float industryInternalProductionCost = configuration->GetIndustryProductionInternalCost ();

    float farmsExternalProductionCost = configuration->GetFarmsProductionExternalCost ();
    float minesExternalProductionCost = configuration->GetMinesProductionExternalCost ();
    float industryExternalProductionCost = configuration->GetIndustryProductionExternalCost ();

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

    float totalLogisticsEvolution = CalculateTotalEvolutionOf ("logistics", realDistricts);
    float totalDefenseEvolution = CalculateTotalEvolutionOf ("defense", realDistricts);
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
    {
        variantVector.Push (Urho3D::Variant (districtsHashes_.At (index)));
    }
    return variantVector;
}

void InternalTradeArea::SetDistrictsHashesArrayAttribute (const Urho3D::VariantVector &attribute)
{
    districtsHashes_.Clear ();
    if (!attribute.Empty ())
    {
        int requestedSize = attribute.At (0).GetInt ();
        if (requestedSize > 0)
        {
            for (int index = 0; index < requestedSize; index++)
            {
                if (index + 1 < attribute.Size ())
                {
                    districtsHashes_.Push (attribute.At (index + 1).GetStringHash ());
                }
                else
                {
                    districtsHashes_.Push (Urho3D::StringHash ());
                }
            }
        }
    }
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

bool DistrictProductionInfoComparators::HigherSellability (DistrictProductionInfo &first, DistrictProductionInfo &second)
{
    return (first.sellability_ > second.sellability_);
}

void DistrictProductionInfo::CalculateSellability ()
{
    sellability_ = (quality_ / relativePrice_) * (quality_ / relativePrice_);
}

Urho3D::VariantMap DistrictProductionInfo::ToVariantMap ()
{
    Urho3D::VariantMap variantMap;
    variantMap [DISTRICT_PRODUCTION_AMOUNT_KEY] = amount_;
    variantMap [DISTRICT_PRODUCTION_RELATIVE_PRICE_KEY] = relativePrice_;
    variantMap [DISTRICT_PRODUCTION_QUALITY_KEY] = quality_;
    variantMap [DISTRICT_PRODUCTION_SELLABILITY_KEY] = sellability_;
    variantMap [DISTRICT_PRODUCTION_SELLED_KEY] = selled_;
    return variantMap;
}
}
