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

float InternalTradeArea::CalculateDistrictFarmsProductionAmount (District *district, GameConfiguration *configuration)
{
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
    return districtPopulation * configuration->GetFarmsProductionPerColonist () *
            sqrt (district->GetFarmsEvolutionPoints ()) * district->GetLandAverageFertility () * climateModifer;
}

float InternalTradeArea::CalculateDistrictMinesProductionAmount (District *district, GameConfiguration *configuration)
{
    float districtPopulation = district->GetMenCount () + district->GetWomenCount ();
    return districtPopulation * configuration->GetMinesProductionPerColonist () *
            sqrt (district->GetMinesEvolutionPoints ());
}

float InternalTradeArea::CalculateDistrictIndustryProductionAmount (District *district, GameConfiguration *configuration)
{
    float districtPopulation = district->GetMenCount () + district->GetWomenCount ();
    return districtPopulation * configuration->GetIndustryProductionPerColonist () *
            sqrt (district->GetIndustryEvolutionPoints ());
}

DistrictProductionInfo InternalTradeArea::CalculateDistrictProductionOfFarms (District *district, GameConfiguration *configuration)
{
    DistrictProductionInfo production;
    production.districtHash_ = district->GetHash ();
    production.amount_ = CalculateDistrictFarmsProductionAmount (district, configuration);

    // TODO: Implement farms production quality and relative price calculation.
    production.relativePrice_ = 1.0f;
    production.quality_ = 1.0f;
    production.selled_ = 0.0f;
    production.CalculateSellability ();
    return production;
}

DistrictProductionInfo InternalTradeArea::CalculateDistrictProductionOfMines (District *district, GameConfiguration *configuration)
{
    DistrictProductionInfo production;
    production.districtHash_ = district->GetHash ();
    production.amount_ = CalculateDistrictMinesProductionAmount (district, configuration);

    // TODO: Implement mines production quality and relative price calculation.
    production.relativePrice_ = 1.0f;
    production.quality_ = 1.0f;
    production.selled_ = 0.0f;
    production.CalculateSellability ();
    return production;
}

DistrictProductionInfo InternalTradeArea::CalculateDistrictProductionOfIndustry (District *district, GameConfiguration *configuration)
{
    DistrictProductionInfo production;
    production.districtHash_ = district->GetHash ();
    production.amount_ = CalculateDistrictIndustryProductionAmount (district, configuration);

    // TODO: Implement industry production quality and relative price calculation.
    production.relativePrice_ = 1.0f;
    production.quality_ = 1.0f;
    production.selled_ = 0.0f;
    production.CalculateSellability ();
    return production;
}

void InternalTradeArea::CalculateTotalProductionOfFarms (Urho3D::PODVector <District *> &realDistricts, GameConfiguration *configuration, Urho3D::Vector <DistrictProductionInfo> &output)
{
    for (int index = 0; index < realDistricts.Size (); index++)
    {
        District *district = realDistricts.At (index);
        assert (district);
        output.Push (CalculateDistrictProductionOfFarms (district, configuration));
    }
    Urho3D::Sort <DistrictProductionInfo, DistrictProductionInfoComparator> (
                output.Begin (), output.End (), DistrictProductionInfoComparators::HigherSellability);
}

void InternalTradeArea::CalculateTotalProductionOfMines (Urho3D::PODVector <District *> &realDistricts, GameConfiguration *configuration, Urho3D::Vector<DistrictProductionInfo> &output)
{
    for (int index = 0; index < realDistricts.Size (); index++)
    {
        District *district = realDistricts.At (index);
        assert (district);
        output.Push (CalculateDistrictProductionOfMines (district, configuration));
    }
    Urho3D::Sort <DistrictProductionInfo, DistrictProductionInfoComparator> (
                output.Begin (), output.End (), DistrictProductionInfoComparators::HigherSellability);
}

void InternalTradeArea::CalculateTotalProductionOfIndustry (Urho3D::PODVector <District *> &realDistricts, GameConfiguration *configuration, Urho3D::Vector<DistrictProductionInfo> &output)
{
    for (int index = 0; index < realDistricts.Size (); index++)
    {
        District *district = realDistricts.At (index);
        assert (district);
        output.Push (CalculateDistrictProductionOfIndustry (district, configuration));
    }
    Urho3D::Sort <DistrictProductionInfo, DistrictProductionInfoComparator> (
                output.Begin (), output.End (), DistrictProductionInfoComparators::HigherSellability);
}

float InternalTradeArea::CalculateTotalProductionConsumptionOfFarms (GameConfiguration *configuration, Urho3D::PODVector<District *> &realDistricts, int soldiersCount)
{
    float totalConsumption = 0.0f;
    for (int index = 0; index < realDistricts.Size (); index++)
    {
        District *district = realDistricts.At (index);
        assert (district);
        totalConsumption += CalculateDistrictProductionConsumptionOfFarms (configuration, district);
    }

    float oneSoldierConsumption = configuration->GetOneSoldierFarmsProductionConsumption ();
    totalConsumption += oneSoldierConsumption * soldiersCount;
    return totalConsumption;
}

float InternalTradeArea::CalculateTotalProductionConsumptionOfMines (GameConfiguration *configuration, Urho3D::PODVector<District *> &realDistricts, int soldiersCount)
{
    float totalConsumption = 0.0f;
    for (int index = 0; index < realDistricts.Size (); index++)
    {
        District *district = realDistricts.At (index);
        assert (district);
        totalConsumption += CalculateDistrictProductionConsumptionOfMines (configuration, district);
    }

    float oneSoldierConsumption = configuration->GetOneSoldierMinesProductionConsumption ();
    totalConsumption += oneSoldierConsumption * soldiersCount;
    return totalConsumption;
}

float InternalTradeArea::CalculateTotalProductionConsumptionOfIndustry (GameConfiguration *configuration, Urho3D::PODVector<District *> &realDistricts, int soldiersCount)
{
    float totalConsumption = 0.0f;
    for (int index = 0; index < realDistricts.Size (); index++)
    {
        District *district = realDistricts.At (index);
        assert (district);
        totalConsumption += CalculateDistrictProductionConsumptionOfIndustry (configuration, district);
    }

    float oneSoldierConsumption = configuration->GetOneSoldierIndustryProductionConsumption ();
    totalConsumption += oneSoldierConsumption * soldiersCount;
    return totalConsumption;
}

float InternalTradeArea::CalculateDistrictProductionConsumptionOfFarms (GameConfiguration *configuration, District *district)
{
    float oneColonistConsumption = configuration->GetOneColonistFarmsProductionConsumption ();
    float minesConsumption = configuration->GetFarmsProductionMinesConsumption ();
    float industryConsumption = configuration->GetFarmsProductionIndustryConsumption ();

    float population = (district->GetMenCount () + district->GetWomenCount ());
    float totalConsumption = oneColonistConsumption * population;
    totalConsumption += minesConsumption * CalculateDistrictMinesProductionAmount (district, configuration);
    totalConsumption += industryConsumption * CalculateDistrictIndustryProductionAmount (district, configuration);
    return totalConsumption;
}

float InternalTradeArea::CalculateDistrictProductionConsumptionOfMines (GameConfiguration *configuration, District *district)
{
    float oneColonistConsumption = configuration->GetOneColonistMinesProductionConsumption ();
    float farmsConsumption = configuration->GetMinesProductionFarmsConsumption ();
    float industryConsumption = configuration->GetMinesProductionIndustryConsumption ();

    float population = (district->GetMenCount () + district->GetWomenCount ());
    float totalConsumption = oneColonistConsumption * population;
    totalConsumption += farmsConsumption * CalculateDistrictFarmsProductionAmount (district, configuration);
    totalConsumption += industryConsumption * CalculateDistrictIndustryProductionAmount (district, configuration);
    return totalConsumption;
}

float InternalTradeArea::CalculateDistrictProductionConsumptionOfIndustry (GameConfiguration *configuration, District *district)
{
    float oneColonistConsumption = configuration->GetOneColonistIndustryProductionConsumption ();
    float farmsConsumption = configuration->GetIndustryProductionFarmsConsumption ();
    float minesConsumption = configuration->GetIndustryProductionMinesConsumption ();

    float population = (district->GetMenCount () + district->GetWomenCount ());
    float totalConsumption = oneColonistConsumption * population;
    totalConsumption += farmsConsumption * CalculateDistrictFarmsProductionAmount (district, configuration);
    totalConsumption += minesConsumption * CalculateDistrictMinesProductionAmount (district, configuration);
    return totalConsumption;
}

float InternalTradeArea::ConsumeProduction (float consumption, Urho3D::Vector <DistrictProductionInfo> &production)
{
    int index = 0;
    while (index < production.Size () && consumption > 0.0f)
    {
        DistrictProductionInfo &productionInfo = production.At (index);
        if (consumption > productionInfo.amount_)
        {
            productionInfo.selled_ = productionInfo.amount_;
            consumption -= productionInfo.selled_;
        }
        else
        {
            productionInfo.selled_ = consumption;
            consumption = 0.0f;
        }
        index++;
    }
    return consumption;
}

float InternalTradeArea::CalculateTotalProduction (Urho3D::Vector <DistrictProductionInfo> &production)
{
    float totalProduction = 0.0f;
    for (int index = 0; index < production.Size (); index++)
    {
        totalProduction += production.At (index).amount_;
    }
    return totalProduction;
}

void InternalTradeArea::ProcessFarmsBalance (Map *map, GameConfiguration *configuration, Urho3D::Vector <DistrictProductionInfo> &farmsProduction, float updateDelay)
{
    for (int index = 0; index < farmsProduction.Size (); index++)
    {
        DistrictProductionInfo &productionInfo = farmsProduction.At (index);
        District *district = map->GetDistrictByHash (productionInfo.districtHash_);

        float farmsBalanceAdditionInternal = productionInfo.selled_ *
                configuration->GetFarmsProductionInternalCost () * updateDelay;

        float fromFarmsToLogistics = farmsBalanceAdditionInternal * configuration->GetTradeAreaFarmsLogisticsExpenses ();
        district->SetLogisticsBalance (district->GetLogisticsBalance () + fromFarmsToLogistics);

        float fromFarmsToDefense = farmsBalanceAdditionInternal * configuration->GetTradeAreaFarmsDefenseExpenses ();
        district->SetDefenseBalance (district->GetDefenseBalance () + fromFarmsToDefense);

        farmsBalanceAdditionInternal -= fromFarmsToLogistics;
        farmsBalanceAdditionInternal -= fromFarmsToDefense;

        float farmsBalanceAdditionExternal = (productionInfo.amount_ - productionInfo.selled_) *
                configuration->GetFarmsProductionExternalCost () * updateDelay;
        district->SetFarmsBalance (district->GetFarmsBalance () +
                                   farmsBalanceAdditionInternal * configuration->GetTradeAreaInternalProfitToBalance () +
                                   farmsBalanceAdditionExternal * configuration->GetTradeAreaExternalProfitToBalance ());
    }
}

void InternalTradeArea::ProcessMinesBalance (Map *map, GameConfiguration *configuration, Urho3D::Vector <DistrictProductionInfo> &minesProduction, float updateDelay)
{
    for (int index = 0; index < minesProduction.Size (); index++)
    {
        DistrictProductionInfo &productionInfo = minesProduction.At (index);
        District *district = map->GetDistrictByHash (productionInfo.districtHash_);

        float minesBalanceAdditionInternal = productionInfo.selled_ *
                configuration->GetMinesProductionInternalCost () * updateDelay;

        float fromMinesToLogistics = minesBalanceAdditionInternal * configuration->GetTradeAreaMinesLogisticsExpenses ();
        district->SetLogisticsBalance (district->GetLogisticsBalance () + fromMinesToLogistics);

        float fromMinesToDefense = minesBalanceAdditionInternal * configuration->GetTradeAreaMinesDefenseExpenses ();
        district->SetDefenseBalance (district->GetDefenseBalance () + fromMinesToDefense);

        minesBalanceAdditionInternal -= fromMinesToLogistics;
        minesBalanceAdditionInternal -= fromMinesToDefense;

        float minesBalanceAdditionExternal = (productionInfo.amount_ - productionInfo.selled_) *
                configuration->GetMinesProductionExternalCost () * updateDelay;
        district->SetMinesBalance (district->GetMinesBalance () +
                                   minesBalanceAdditionInternal * configuration->GetTradeAreaInternalProfitToBalance () +
                                   minesBalanceAdditionExternal * configuration->GetTradeAreaExternalProfitToBalance ());
    }
}

void InternalTradeArea::ProcessIndustryBalance (Map *map, GameConfiguration *configuration, Urho3D::Vector <DistrictProductionInfo> &industryProduction, float updateDelay)
{
    for (int index = 0; index < industryProduction.Size (); index++)
    {
        DistrictProductionInfo &productionInfo = industryProduction.At (index);
        District *district = map->GetDistrictByHash (productionInfo.districtHash_);

        float industryBalanceAdditionInternal = productionInfo.selled_ *
                configuration->GetIndustryProductionInternalCost () * updateDelay;

        float fromIndustryToLogistics = industryBalanceAdditionInternal * configuration->GetTradeAreaIndustryLogisticsExpenses ();
        district->SetLogisticsBalance (district->GetLogisticsBalance () + fromIndustryToLogistics);

        float fromIndustryToDefense = industryBalanceAdditionInternal * configuration->GetTradeAreaIndustryDefenseExpenses ();
        district->SetDefenseBalance (district->GetDefenseBalance () + fromIndustryToDefense);

        industryBalanceAdditionInternal -= fromIndustryToLogistics;
        industryBalanceAdditionInternal -= fromIndustryToDefense;

        float industryBalanceAdditionExternal = (productionInfo.amount_ - productionInfo.selled_) *
                configuration->GetIndustryProductionExternalCost () * updateDelay;
        district->SetIndustryBalance (district->GetIndustryBalance () +
                                   industryBalanceAdditionInternal * configuration->GetTradeAreaInternalProfitToBalance () +
                                   industryBalanceAdditionExternal * configuration->GetTradeAreaExternalProfitToBalance ());
    }
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

Urho3D::SharedPtr <TradeDistrictProcessingInfo> InternalTradeArea::ProcessTrade (Map *map, float updateDelay, bool writeDistrictsBalance)
{
    GameConfiguration *configuration = node_->GetScene ()->GetComponent <GameConfiguration> ();
    assert (configuration);
    Urho3D::PODVector <District *> realDistricts;
    ConstructVectorOfRealDistricts (map, realDistricts);
    int totalSoldiersCount = CalculateTotalSoldiersCount ();

    Urho3D::Vector <DistrictProductionInfo> farmsTotalProduction;
    CalculateTotalProductionOfFarms (realDistricts, configuration, farmsTotalProduction);
    float totalFarmsProduction = CalculateTotalProduction (farmsTotalProduction);

    Urho3D::Vector <DistrictProductionInfo> minesTotalProduction;
    CalculateTotalProductionOfMines (realDistricts, configuration, minesTotalProduction);
    float totalMinesProduction = CalculateTotalProduction (minesTotalProduction);

    Urho3D::Vector <DistrictProductionInfo> industryTotalProduction;
    CalculateTotalProductionOfIndustry (realDistricts, configuration, industryTotalProduction);
    float totalIndustryProduction = CalculateTotalProduction (industryTotalProduction);

    float totalFarmsConsumption = CalculateTotalProductionConsumptionOfFarms (configuration, realDistricts, totalSoldiersCount);
    float totalMinesConsumption = CalculateTotalProductionConsumptionOfMines (configuration, realDistricts, totalSoldiersCount);
    float totalIndustryConsumption = CalculateTotalProductionConsumptionOfIndustry (configuration, realDistricts, totalSoldiersCount);

    float farmsUnsatisfiedConsumption = ConsumeProduction (totalFarmsConsumption, farmsTotalProduction);
    float minesUnsatisfiedConsumption = ConsumeProduction (totalMinesConsumption, minesTotalProduction);
    float industryUnsatisfiedConsumption = ConsumeProduction (totalIndustryConsumption, industryTotalProduction);

    float farmsInternalProductionCost = configuration->GetFarmsProductionInternalCost ();
    float minesInternalProductionCost = configuration->GetMinesProductionInternalCost ();
    float industryInternalProductionCost = configuration->GetIndustryProductionInternalCost ();

    float farmsExternalProductionCost = configuration->GetFarmsProductionExternalCost ();
    float minesExternalProductionCost = configuration->GetMinesProductionExternalCost ();
    float industryExternalProductionCost = configuration->GetIndustryProductionExternalCost ();

    float soldFarmsProduction = (farmsUnsatisfiedConsumption <= 0.0f) ? totalFarmsConsumption : totalFarmsProduction;
    float soldMinesProduction = (minesUnsatisfiedConsumption <= 0.0f) ? totalMinesConsumption : totalMinesProduction;
    float soldIndustryProduction = (industryUnsatisfiedConsumption <= 0.0f) ? totalIndustryConsumption : totalIndustryProduction;

    Urho3D::SharedPtr <TradeDistrictProcessingInfo> result (new TradeDistrictProcessingInfo (context_));
    result->SetUnusedProductionOf ("farms", totalFarmsProduction - totalFarmsConsumption);
    result->SetUnusedProductionOf ("mines", totalMinesProduction - totalMinesConsumption);
    result->SetUnusedProductionOf ("industry", totalIndustryProduction - totalIndustryConsumption);

    result->SetSoldTradeGoodsCost ( updateDelay * (soldFarmsProduction * farmsInternalProductionCost +
                                                   soldMinesProduction * minesInternalProductionCost +
                                                   soldIndustryProduction * industryInternalProductionCost));

    result->SetUnsoldTradeGoodsCost ( updateDelay * ( (totalFarmsProduction - soldFarmsProduction) * farmsExternalProductionCost +
                                                      (totalMinesProduction - soldMinesProduction) * minesExternalProductionCost +
                                                      (totalIndustryProduction - soldIndustryProduction) * industryExternalProductionCost));

    if (writeDistrictsBalance)
    {
        ProcessFarmsBalance (map, configuration, farmsTotalProduction, updateDelay);
        ProcessMinesBalance (map, configuration, minesTotalProduction, updateDelay);
        ProcessIndustryBalance (map, configuration, industryTotalProduction, updateDelay);
    }
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
    unusedProduction_ (),
    unsoldTradeGoodsCost_ (0.0f),
    soldTradeGoodsCost_ (0.0f)
{

}

TradeDistrictProcessingInfo::~TradeDistrictProcessingInfo ()
{

}

float TradeDistrictProcessingInfo::GetUnusedProductionOf (Urho3D::StringHash type)
{
    return unusedProduction_ [type];
}

void TradeDistrictProcessingInfo::SetUnusedProductionOf (Urho3D::StringHash type, float points)
{
    unusedProduction_ [type] = points;
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
