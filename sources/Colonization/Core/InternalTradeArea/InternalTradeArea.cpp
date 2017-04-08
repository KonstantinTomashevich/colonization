#include <Colonization/BuildConfiguration.hpp>
#include "InternalTradeArea.hpp"
#include <Colonization/Core/District/District.hpp>
#include <Colonization/Core/Map.hpp>

#include <Colonization/Utils/Serialization/Categories.hpp>
#include <Colonization/Utils/Network/NetworkUpdateCounter.hpp>
#include <Colonization/Utils/Network/NetworkUpdateSmoother.hpp>
#include <Colonization/Backend/UnitsManager.hpp>
#include <Colonization/Core/District/DistrictUtils.hpp>

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

void InternalTradeArea::CalculateTotalProductionOfFarms (Urho3D::PODVector <District *> &realDistricts, GameConfiguration *configuration, Urho3D::Vector <DistrictProductionInfo> &output)
{
    for (int index = 0; index < realDistricts.Size (); index++)
    {
        District *district = realDistricts.At (index);
        assert (district);
        output.Push (DistrictUtils::CalculateProductionOfFarms (district, configuration));
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
        output.Push (DistrictUtils::CalculateProductionOfMines (district, configuration));
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
        output.Push (DistrictUtils::CalculateProductionOfIndustry (district, configuration));
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
        totalConsumption += DistrictUtils::CalculateProductionConsumptionOfFarms (configuration, district);
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
        totalConsumption += DistrictUtils::CalculateProductionConsumptionOfMines (configuration, district);
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
        totalConsumption += DistrictUtils::CalculateProductionConsumptionOfIndustry (configuration, district);
    }

    float oneSoldierConsumption = configuration->GetOneSoldierIndustryProductionConsumption ();
    totalConsumption += oneSoldierConsumption * soldiersCount;
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

void InternalTradeArea::ProcessFarmsBalance (Map *map, GameConfiguration *configuration, Urho3D::Vector <DistrictProductionInfo> &farmsProduction, float updateDelay,
                                             bool changeDistrictsVars, Urho3D::HashMap<Urho3D::StringHash, Urho3D::VariantMap> &districtsBalanceAdditions)
{
    for (int index = 0; index < farmsProduction.Size (); index++)
    {
        DistrictProductionInfo &productionInfo = farmsProduction.At (index);
        District *district = map->GetDistrictByHash (productionInfo.districtHash_);

        float farmsBalanceAdditionInternal = productionInfo.selled_ *
                configuration->GetFarmsProductionInternalCost () * productionInfo.relativePrice_ *
                updateDelay * (1.0f - configuration->GetInternalTaxes ());

        float fromFarmsToLogistics = farmsBalanceAdditionInternal * configuration->GetTradeAreaFarmsLogisticsExpenses ();
        if (changeDistrictsVars)
        {
            district->SetLogisticsBalance (district->GetLogisticsBalance () + fromFarmsToLogistics);
        }

        float fromFarmsToDefense = farmsBalanceAdditionInternal * configuration->GetTradeAreaFarmsDefenseExpenses ();
        if (changeDistrictsVars)
        {
            district->SetDefenseBalance (district->GetDefenseBalance () + fromFarmsToDefense);
        }

        farmsBalanceAdditionInternal -= fromFarmsToLogistics;
        farmsBalanceAdditionInternal -= fromFarmsToDefense;
        float farmsBalanceAdditionExternal = (productionInfo.amount_ - productionInfo.selled_) *
                configuration->GetFarmsProductionExternalCost () * productionInfo.relativePrice_ *
                updateDelay * (1.0f - configuration->GetExternalTaxes ());

        float farmsBalanceAddition =
                farmsBalanceAdditionInternal * configuration->GetTradeAreaInternalProfitToBalance () +
                farmsBalanceAdditionExternal * configuration->GetTradeAreaExternalProfitToBalance ();
        if (changeDistrictsVars)
        {
            district->SetFarmsBalance (district->GetFarmsBalance () + farmsBalanceAddition);
        }

        Urho3D::VariantMap &districtBalanceAdditions = districtsBalanceAdditions [district->GetHash ()];
        districtBalanceAdditions ["farms"] = farmsBalanceAddition;
        districtBalanceAdditions ["logistics"] = districtBalanceAdditions ["logistics"].GetFloat () + fromFarmsToLogistics;
        districtBalanceAdditions ["defense"] = districtBalanceAdditions ["defense"].GetFloat () + fromFarmsToDefense;
        districtsBalanceAdditions [district->GetHash ()] = districtBalanceAdditions;
    }
}

void InternalTradeArea::ProcessMinesBalance (Map *map, GameConfiguration *configuration, Urho3D::Vector <DistrictProductionInfo> &minesProduction, float updateDelay,
                                             bool changeDistrictsVars, Urho3D::HashMap <Urho3D::StringHash, Urho3D::VariantMap> &districtsBalanceAdditions)
{
    for (int index = 0; index < minesProduction.Size (); index++)
    {
        DistrictProductionInfo &productionInfo = minesProduction.At (index);
        District *district = map->GetDistrictByHash (productionInfo.districtHash_);

        float minesBalanceAdditionInternal = productionInfo.selled_ *
                configuration->GetMinesProductionInternalCost () * productionInfo.relativePrice_ *
                updateDelay * (1.0f - configuration->GetInternalTaxes ());

        float fromMinesToLogistics = minesBalanceAdditionInternal * configuration->GetTradeAreaMinesLogisticsExpenses ();
        if (changeDistrictsVars)
        {
            district->SetLogisticsBalance (district->GetLogisticsBalance () + fromMinesToLogistics);
        }

        float fromMinesToDefense = minesBalanceAdditionInternal * configuration->GetTradeAreaMinesDefenseExpenses ();
        if (changeDistrictsVars)
        {
            district->SetDefenseBalance (district->GetDefenseBalance () + fromMinesToDefense);
        }

        minesBalanceAdditionInternal -= fromMinesToLogistics;
        minesBalanceAdditionInternal -= fromMinesToDefense;
        float minesBalanceAdditionExternal = (productionInfo.amount_ - productionInfo.selled_) *
                configuration->GetMinesProductionExternalCost () * productionInfo.relativePrice_ *
                updateDelay * (1.0f - configuration->GetExternalTaxes ());

        float minesBalanceAddition =
                minesBalanceAdditionInternal * configuration->GetTradeAreaInternalProfitToBalance () +
                minesBalanceAdditionExternal * configuration->GetTradeAreaExternalProfitToBalance ();
        if (changeDistrictsVars)
        {
            district->SetMinesBalance (district->GetMinesBalance () + minesBalanceAddition);
        }

        Urho3D::VariantMap &districtBalanceAdditions = districtsBalanceAdditions [district->GetHash ()];
        districtBalanceAdditions ["mines"] = minesBalanceAddition;
        districtBalanceAdditions ["logistics"] = districtBalanceAdditions ["logistics"].GetFloat () + fromMinesToLogistics;
        districtBalanceAdditions ["defense"] = districtBalanceAdditions ["defense"].GetFloat () + fromMinesToDefense;
        districtsBalanceAdditions [district->GetHash ()] = districtBalanceAdditions;
    }
}

void InternalTradeArea::ProcessIndustryBalance (Map *map, GameConfiguration *configuration, Urho3D::Vector <DistrictProductionInfo> &industryProduction, float updateDelay,
                                                bool changeDistrictsVars, Urho3D::HashMap <Urho3D::StringHash, Urho3D::VariantMap> &districtsBalanceAdditions)
{
    for (int index = 0; index < industryProduction.Size (); index++)
    {
        DistrictProductionInfo &productionInfo = industryProduction.At (index);
        District *district = map->GetDistrictByHash (productionInfo.districtHash_);

        float industryBalanceAdditionInternal = productionInfo.selled_ *
                configuration->GetIndustryProductionInternalCost () * productionInfo.relativePrice_ *
                updateDelay * (1.0f - configuration->GetInternalTaxes ());

        float fromIndustryToLogistics = industryBalanceAdditionInternal * configuration->GetTradeAreaIndustryLogisticsExpenses ();
        if (changeDistrictsVars)
        {
            district->SetLogisticsBalance (district->GetLogisticsBalance () + fromIndustryToLogistics);
        }

        float fromIndustryToDefense = industryBalanceAdditionInternal * configuration->GetTradeAreaIndustryDefenseExpenses ();
        if (changeDistrictsVars)
        {
            district->SetDefenseBalance (district->GetDefenseBalance () + fromIndustryToDefense);
        }

        industryBalanceAdditionInternal -= fromIndustryToLogistics;
        industryBalanceAdditionInternal -= fromIndustryToDefense;
        float industryBalanceAdditionExternal = (productionInfo.amount_ - productionInfo.selled_) *
                configuration->GetIndustryProductionExternalCost () * productionInfo.relativePrice_ *
                updateDelay * (1.0f - configuration->GetExternalTaxes ());

        float industryBalanceAddition =
                industryBalanceAdditionInternal * configuration->GetTradeAreaInternalProfitToBalance () +
                industryBalanceAdditionExternal * configuration->GetTradeAreaExternalProfitToBalance ();
        if (changeDistrictsVars)
        {
            district->SetIndustryBalance (district->GetIndustryBalance () + industryBalanceAddition);
        }

        Urho3D::VariantMap &districtBalanceAdditions = districtsBalanceAdditions [district->GetHash ()];
        districtBalanceAdditions ["industry"] = industryBalanceAddition;
        districtBalanceAdditions ["logistics"] = districtBalanceAdditions ["logistics"].GetFloat () + fromIndustryToLogistics;
        districtBalanceAdditions ["defense"] = districtBalanceAdditions ["defense"].GetFloat () + fromIndustryToDefense;
        districtsBalanceAdditions [district->GetHash ()] = districtBalanceAdditions;
    }
}

void InternalTradeArea::ProcessDistrictsProductionInfo (Urho3D::Vector <DistrictProductionInfo> &production, Urho3D::StringHash productionType,
                                                        Urho3D::HashMap <Urho3D::StringHash, Urho3D::VariantMap> &districtsProductionInfo, float updateDelay)
{
    for (int index = 0; index < production.Size (); index++)
    {
        DistrictProductionInfo &productionInfo = production.At (index);
        productionInfo.amount_ *= updateDelay;
        productionInfo.selled_ *= updateDelay;
        Urho3D::VariantMap &districtProduction = districtsProductionInfo [productionInfo.districtHash_];
        districtProduction [productionType] = productionInfo.ToVariantMap ();
        districtsProductionInfo [productionInfo.districtHash_] = districtProduction;
    }
}

void InternalTradeArea::WriteDistrictsBalanceAdditions (Map *map, Urho3D::SharedPtr<TradeDistrictProcessingInfo> result,
                                                        Urho3D::HashMap <Urho3D::StringHash, Urho3D::VariantMap> &districtsBalanceAdditions,
                                                        bool changeDistrictsVars)
{
    for (Urho3D::HashMap <Urho3D::StringHash, Urho3D::VariantMap>::Iterator iterator = districtsBalanceAdditions.Begin ();
         iterator != districtsBalanceAdditions.End (); iterator++)
    {
        Urho3D::StringHash districtHash = iterator->first_;
        Urho3D::VariantMap data = iterator->second_;
        result->SetDistrictBalanceAdditions (districtHash, data);

        if (changeDistrictsVars)
        {
            District *district = map->GetDistrictByHash (districtHash);
            district->SetLastTradeFarmsBalanceAddition (data ["farms"].GetFloat ());
            district->SetLastTradeMinesBalanceAddition (data ["mines"].GetFloat ());
            district->SetLastTradeIndustryBalanceAddition (data ["industry"].GetFloat ());
            district->SetLastTradeLogisticsBalanceAddition (data ["logistics"].GetFloat ());
            district->SetLastTradeDefenseBalanceAddition (data ["defense"].GetFloat ());
        }
    }
}

void InternalTradeArea::WriteDistrictsProduction (Map *map, Urho3D::SharedPtr<TradeDistrictProcessingInfo> result,
                                                  Urho3D::HashMap <Urho3D::StringHash, Urho3D::VariantMap> &districtsProduction,
                                                  bool changeDistrictsVars)
{
    for (Urho3D::HashMap <Urho3D::StringHash, Urho3D::VariantMap>::Iterator iterator = districtsProduction.Begin ();
         iterator != districtsProduction.End (); iterator++)
    {
        Urho3D::StringHash districtHash = iterator->first_;
        Urho3D::VariantMap data = iterator->second_;
        result->SetDistrictProduction (districtHash, data);

        if (changeDistrictsVars)
        {
            District *district = map->GetDistrictByHash (districtHash);
            district->SetLastTradeFarmsProduction (data ["farms"].GetVariantMap ());
            district->SetLastTradeMinesProduction (data ["mines"].GetVariantMap ());
            district->SetLastTradeIndustryProduction (data ["industry"].GetVariantMap ());
        }
    }
}

void InternalTradeArea::CallDistrictsUpdate (Urho3D::PODVector<District *> &districts)
{
    for (int index = 0; index < districts.Size (); index++)
    {
        District *district = districts.At (index);
        if (node_->GetScene ()->GetComponent <NetworkUpdateSmoother> ())
        {
            NetworkUpdateCounter *counter = district->GetNode ()->GetComponent <NetworkUpdateCounter> ();
            if (!counter)
            {
                counter = CreateNetworkUpdateCounterForComponent (district);
            }
            counter->AddUpdatePoints (100.0f);
        }
    }
}

float InternalTradeArea::CalculateSoldFarmsProductionCost (Urho3D::Vector <DistrictProductionInfo> &production, GameConfiguration *configuration)
{
    float soldFarmsProductionCost = 0.0f;
    for (int index = 0; index < production.Size (); index++)
    {
        DistrictProductionInfo productionInfo = production.At (index);
        soldFarmsProductionCost += productionInfo.selled_ * productionInfo.relativePrice_ *
                configuration->GetFarmsProductionInternalCost ();
    }
    return soldFarmsProductionCost;
}

float InternalTradeArea::CalculateSoldMinesProductionCost (Urho3D::Vector <DistrictProductionInfo> &production, GameConfiguration *configuration)
{
    float soldMinesProductionCost = 0.0f;
    for (int index = 0; index < production.Size (); index++)
    {
        DistrictProductionInfo productionInfo = production.At (index);
        soldMinesProductionCost += productionInfo.selled_ * productionInfo.relativePrice_ *
                configuration->GetMinesProductionInternalCost ();
    }
    return soldMinesProductionCost;
}

float InternalTradeArea::CalculateSoldIndustryProductionCost (Urho3D::Vector <DistrictProductionInfo> &production, GameConfiguration *configuration)
{
    float soldIndustryProductionCost = 0.0f;
    for (int index = 0; index < production.Size (); index++)
    {
        DistrictProductionInfo productionInfo = production.At (index);
        soldIndustryProductionCost += productionInfo.selled_ * productionInfo.relativePrice_ *
                configuration->GetIndustryProductionInternalCost ();
    }
    return soldIndustryProductionCost;
}

float InternalTradeArea::CalculateUnsoldFarmsProductionCost (Urho3D::Vector <DistrictProductionInfo> &production, GameConfiguration *configuration)
{
    float unsoldFarmsProductionCost = 0.0f;
    for (int index = 0; index < production.Size (); index++)
    {
        DistrictProductionInfo productionInfo = production.At (index);
        unsoldFarmsProductionCost += (productionInfo.amount_ - productionInfo.selled_) *
                productionInfo.relativePrice_ * configuration->GetFarmsProductionExternalCost ();
    }
    return unsoldFarmsProductionCost;
}

float InternalTradeArea::CalculateUnsoldMinesProductionCost (Urho3D::Vector <DistrictProductionInfo> &production, GameConfiguration *configuration)
{
    float unsoldMinesProductionCost = 0.0f;
    for (int index = 0; index < production.Size (); index++)
    {
        DistrictProductionInfo productionInfo = production.At (index);
        unsoldMinesProductionCost += (productionInfo.amount_ - productionInfo.selled_) *
                productionInfo.relativePrice_ * configuration->GetMinesProductionExternalCost ();
    }
    return unsoldMinesProductionCost;
}

float InternalTradeArea::CalculateUnsoldIndustryProductionCost (Urho3D::Vector <DistrictProductionInfo> &production, GameConfiguration *configuration)
{
    float unsoldIndustryProductionCost = 0.0f;
    for (int index = 0; index < production.Size (); index++)
    {
        DistrictProductionInfo productionInfo = production.At (index);
        unsoldIndustryProductionCost += (productionInfo.amount_ - productionInfo.selled_) *
                productionInfo.relativePrice_ * configuration->GetIndustryProductionExternalCost ();
    }
    return unsoldIndustryProductionCost;
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

Urho3D::SharedPtr <TradeDistrictProcessingInfo> InternalTradeArea::ProcessTrade (Map *map, float updateDelay, bool changeDistrictsVars)
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

    ConsumeProduction (totalFarmsConsumption, farmsTotalProduction);
    ConsumeProduction (totalMinesConsumption, minesTotalProduction);
    ConsumeProduction (totalIndustryConsumption, industryTotalProduction);

    Urho3D::SharedPtr <TradeDistrictProcessingInfo> result (new TradeDistrictProcessingInfo (context_));
    result->SetUnusedProductionOf ("farms", (totalFarmsProduction - totalFarmsConsumption) * updateDelay);
    result->SetUnusedProductionOf ("mines", (totalMinesProduction - totalMinesConsumption) * updateDelay);
    result->SetUnusedProductionOf ("industry", (totalIndustryProduction - totalIndustryConsumption) * updateDelay);

    result->SetSoldTradeGoodsCost (updateDelay * (CalculateSoldFarmsProductionCost (farmsTotalProduction, configuration) +
                                                  CalculateSoldMinesProductionCost (minesTotalProduction, configuration) +
                                                  CalculateSoldIndustryProductionCost (industryTotalProduction, configuration)));

    result->SetUnsoldTradeGoodsCost (updateDelay * (CalculateUnsoldFarmsProductionCost (farmsTotalProduction, configuration) +
                                                    CalculateUnsoldMinesProductionCost (minesTotalProduction, configuration) +
                                                    CalculateUnsoldIndustryProductionCost (industryTotalProduction, configuration)));

    Urho3D::HashMap <Urho3D::StringHash, Urho3D::VariantMap> districtsBalanceAdditions;
    ProcessFarmsBalance (map, configuration, farmsTotalProduction, updateDelay, changeDistrictsVars, districtsBalanceAdditions);
    ProcessMinesBalance (map, configuration, minesTotalProduction, updateDelay, changeDistrictsVars, districtsBalanceAdditions);
    ProcessIndustryBalance (map, configuration, industryTotalProduction, updateDelay, changeDistrictsVars, districtsBalanceAdditions);
    WriteDistrictsBalanceAdditions (map, result, districtsBalanceAdditions, changeDistrictsVars);

    Urho3D::HashMap <Urho3D::StringHash, Urho3D::VariantMap> districtsProduction;
    ProcessDistrictsProductionInfo (farmsTotalProduction, Urho3D::StringHash ("farms"), districtsProduction, updateDelay);
    ProcessDistrictsProductionInfo (minesTotalProduction, Urho3D::StringHash ("mines"), districtsProduction, updateDelay);
    ProcessDistrictsProductionInfo (industryTotalProduction, Urho3D::StringHash ("industry"), districtsProduction, updateDelay);
    WriteDistrictsProduction (map, result, districtsProduction, changeDistrictsVars);



    if (changeDistrictsVars)
    {
        CallDistrictsUpdate (realDistricts);
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
}
