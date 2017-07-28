#include <Colonization/BuildConfiguration.hpp>
#include "DistrictUtils.hpp"
#include <Colonization/Core/District/District.hpp>
#include <Colonization/Core/GameConfiguration.hpp>

namespace Colonization
{
namespace DistrictUtils
{
float CleanupColonyFromDistrict (District *district)
{
    district->SetHasColony (false);
    district->SetColonyOwnerName (Urho3D::String::EMPTY);
    district->RemoveAllColonyActions ();
    district->SetMenCount (0.0f);
    district->SetWomenCount (0.0f);

    district->SetFarmsEvolutionPoints (0.0f);
    district->SetMinesEvolutionPoints (0.0f);
    district->SetIndustryEvolutionPoints (0.0f);
    district->SetLogisticsEvolutionPoints (0.0f);
    district->SetDefenseEvolutionPoints (0.0f);

    district->SetFarmsBalance (0.0f);
    district->SetMinesBalance (0.0f);
    district->SetIndustryBalance (0.0f);
    district->SetLogisticsBalance (0.0f);
    district->SetDefenseBalance (0.0f);

    district->SetLastTradeFarmsProduction (Urho3D::Variant::emptyVariantMap);
    district->SetLastTradeMinesProduction (Urho3D::Variant::emptyVariantMap);
    district->SetLastTradeIndustryProduction (Urho3D::Variant::emptyVariantMap);

    district->SetLastTradeFarmsBalanceAddition (0.0f);
    district->SetLastTradeMinesBalanceAddition (0.0f);
    district->SetLastTradeIndustryBalanceAddition (0.0f);
    district->SetLastTradeLogisticsBalanceAddition (0.0f);
    district->SetLastTradeDefenseBalanceAddition (0.0f);
}

float CalculateFarmsProductionAmount (District *district, GameConfiguration *configuration)
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
            Urho3D::Sqrt (district->GetFarmsEvolutionPoints ()) * district->GetLandAverageFertility () * climateModifer;
}

// TODO: Should coal, iron, silver and gold affect mines and industry production amount?
float CalculateMinesProductionAmount (District *district, GameConfiguration *configuration)
{
    float districtPopulation = district->GetMenCount () + district->GetWomenCount ();
    return districtPopulation * configuration->GetMinesProductionPerColonist () *
            Urho3D::Sqrt (district->GetMinesEvolutionPoints ());
}

float CalculateIndustryProductionAmount (District *district, GameConfiguration *configuration)
{
    float districtPopulation = district->GetMenCount () + district->GetWomenCount ();
    return districtPopulation * configuration->GetIndustryProductionPerColonist () *
            Urho3D::Sqrt (district->GetIndustryEvolutionPoints ());
}

DistrictProductionInfo CalculateProductionOfFarms (District *district, GameConfiguration *configuration)
{
    DistrictProductionInfo production;
    production.districtHash_ = district->GetHash ();
    production.amount_ = CalculateFarmsProductionAmount (district, configuration);

    production.relativePrice_ = CalculateFarmsProductionRelativePrice (district, configuration);
    production.quality_ =  CalculateFarmsProductionQuality (district, configuration);
    production.selled_ = 0.0f;
    production.CalculateSellability ();
    return production;
}

DistrictProductionInfo CalculateProductionOfMines (District *district, GameConfiguration *configuration)
{
    DistrictProductionInfo production;
    production.districtHash_ = district->GetHash ();
    production.amount_ = CalculateMinesProductionAmount (district, configuration);

    production.relativePrice_ = CalculateMinesProductionRelativePrice (district, configuration);
    production.quality_ =  CalculateMinesProductionQuality (district, configuration);
    production.selled_ = 0.0f;
    production.CalculateSellability ();
    return production;
}

DistrictProductionInfo CalculateProductionOfIndustry (District *district, GameConfiguration *configuration)
{
    DistrictProductionInfo production;
    production.districtHash_ = district->GetHash ();
    production.amount_ = CalculateIndustryProductionAmount (district, configuration);

    production.relativePrice_ = CalculateIndustryProductionRelativePrice (district, configuration);
    production.quality_ =  CalculateIndustryProductionQuality (district, configuration);
    production.selled_ = 0.0f;
    production.CalculateSellability ();
    return production;
}

Urho3D::VariantMap CalculateProductionOfFarmsAsVariantMap (District *district, GameConfiguration *configuration)
{
    return CalculateProductionOfFarms (district, configuration).ToVariantMap ();
}

Urho3D::VariantMap CalculateProductionOfMinesAsVariantMap (District *district, GameConfiguration *configuration)
{
    return CalculateProductionOfMines (district, configuration).ToVariantMap ();
}

Urho3D::VariantMap CalculateProductionOfIndustryAsVariantMap (District *district, GameConfiguration *configuration)
{
    return CalculateProductionOfIndustry (district, configuration).ToVariantMap ();
}

float CalculateFarmsProductionRelativePrice (District *district, GameConfiguration *configuration)
{
    float farmsProductionRelativePrice = 1.0f;
    if (district->GetClimate () == CLIMATE_TROPICAL)
    {
        farmsProductionRelativePrice *= configuration->GetFarmsProductionRelativePriceTropicalClimateModifer ();
    }
    else if (district->GetClimate () == CLIMATE_HOT)
    {
        farmsProductionRelativePrice *= configuration->GetFarmsProductionRelativePriceHotClimateModifer ();
    }
    else if (district->GetClimate () == CLIMATE_TEMPERATE)
    {
        farmsProductionRelativePrice *= configuration->GetFarmsProductionRelativePriceTemperateClimateModifer ();
    }
    else if (district->GetClimate () == CLIMATE_TEMPERATE_CONTINENTAL)
    {
        farmsProductionRelativePrice *= configuration->GetFarmsProductionRelativePriceTemperateContinentalClimateModifer ();
    }
    else if (district->GetClimate () == CLIMATE_DESERT)
    {
        farmsProductionRelativePrice *= configuration->GetFarmsProductionRelativePriceDesertClimateModifer ();
    }
    else if (district->GetClimate () == CLIMATE_COLD)
    {
        farmsProductionRelativePrice *= configuration->GetFarmsProductionRelativePriceColdClimateModifer ();
    }

    farmsProductionRelativePrice /= Urho3D::Sqrt (district->GetLandAverageFertility ());
    farmsProductionRelativePrice *= Urho3D::Sqrt (Urho3D::Sqrt (district->GetFarmsEvolutionPoints ()));
    return farmsProductionRelativePrice;
}

float CalculateMinesProductionRelativePrice (District *district, GameConfiguration *configuration)
{
    float minesProductionRelativePrice = 1.0f;
    if (district->GetHasCoalDeposits ())
    {
        minesProductionRelativePrice *= configuration->GetMinesProductionRelativePriceHasCoalModifer ();
    }
    if (district->GetHasIronDeposits ())
    {
        minesProductionRelativePrice *= configuration->GetMinesProductionRelativePriceHasIronModifer ();
    }
    if (district->GetHasSilverDeposits ())
    {
        minesProductionRelativePrice *= configuration->GetMinesProductionRelativePriceHasSilverModifer ();
    }
    if (district->GetHasGoldDeposits ())
    {
        minesProductionRelativePrice *= configuration->GetMinesProductionRelativePriceHasGoldModifer ();
    }

    minesProductionRelativePrice *= Urho3D::Sqrt (Urho3D::Sqrt (district->GetMinesEvolutionPoints ()));
    return minesProductionRelativePrice;
}

float CalculateIndustryProductionRelativePrice (District *district, GameConfiguration *configuration)
{
    float industryProductionRelativePrice = 1.0f;
    if (district->GetHasCoalDeposits ())
    {
        industryProductionRelativePrice *= configuration->GetIndustryProductionRelativePriceHasCoalModifer ();
    }
    if (district->GetHasIronDeposits ())
    {
        industryProductionRelativePrice *= configuration->GetIndustryProductionRelativePriceHasIronModifer ();
    }
    if (district->GetHasSilverDeposits ())
    {
        industryProductionRelativePrice *= configuration->GetIndustryProductionRelativePriceHasSilverModifer ();
    }
    if (district->GetHasGoldDeposits ())
    {
        industryProductionRelativePrice *= configuration->GetIndustryProductionRelativePriceHasGoldModifer ();
    }

    industryProductionRelativePrice *= Urho3D::Sqrt (Urho3D::Sqrt (district->GetIndustryEvolutionPoints ()));
    return industryProductionRelativePrice;
}

float CalculateFarmsProductionQuality (District *district, GameConfiguration *configuration)
{
    float farmsProductionQuality = 1.0f;
    if (district->GetClimate () == CLIMATE_TROPICAL)
    {
        farmsProductionQuality *= configuration->GetFarmsProductionQualityTropicalClimateModifer ();
    }
    else if (district->GetClimate () == CLIMATE_HOT)
    {
        farmsProductionQuality *= configuration->GetFarmsProductionQualityHotClimateModifer ();
    }
    else if (district->GetClimate () == CLIMATE_TEMPERATE)
    {
        farmsProductionQuality *= configuration->GetFarmsProductionQualityTemperateClimateModifer ();
    }
    else if (district->GetClimate () == CLIMATE_TEMPERATE_CONTINENTAL)
    {
        farmsProductionQuality *= configuration->GetFarmsProductionQualityTemperateContinentalClimateModifer ();
    }
    else if (district->GetClimate () == CLIMATE_DESERT)
    {
        farmsProductionQuality *= configuration->GetFarmsProductionQualityDesertClimateModifer ();
    }
    else if (district->GetClimate () == CLIMATE_COLD)
    {
        farmsProductionQuality *= configuration->GetFarmsProductionQualityColdClimateModifer ();
    }

    farmsProductionQuality *= district->GetLandAverageFertility ();
    farmsProductionQuality *= Urho3D::Sqrt (Urho3D::Sqrt (district->GetFarmsEvolutionPoints ()));
    return farmsProductionQuality;
}

float CalculateMinesProductionQuality (District *district, GameConfiguration *configuration)
{
    float minesProductionQuality = 1.0f;
    if (district->GetHasCoalDeposits ())
    {
        minesProductionQuality *= configuration->GetMinesProductionQualityHasCoalModifer ();
    }
    if (district->GetHasIronDeposits ())
    {
        minesProductionQuality *= configuration->GetMinesProductionQualityHasIronModifer ();
    }
    if (district->GetHasSilverDeposits ())
    {
        minesProductionQuality *= configuration->GetMinesProductionQualityHasSilverModifer ();
    }
    if (district->GetHasGoldDeposits ())
    {
        minesProductionQuality *= configuration->GetMinesProductionQualityHasGoldModifer ();
    }

    minesProductionQuality *= Urho3D::Sqrt (Urho3D::Sqrt (district->GetMinesEvolutionPoints ()));
    return minesProductionQuality;
}

float CalculateIndustryProductionQuality (District *district, GameConfiguration *configuration)
{
    float industryProductionQuality = 1.0f;
    if (district->GetHasCoalDeposits ())
    {
        industryProductionQuality *= configuration->GetIndustryProductionQualityHasCoalModifer ();
    }
    if (district->GetHasIronDeposits ())
    {
        industryProductionQuality *= configuration->GetIndustryProductionQualityHasIronModifer ();
    }
    if (district->GetHasSilverDeposits ())
    {
        industryProductionQuality *= configuration->GetIndustryProductionQualityHasSilverModifer ();
    }
    if (district->GetHasGoldDeposits ())
    {
        industryProductionQuality *= configuration->GetIndustryProductionQualityHasGoldModifer ();
    }

    industryProductionQuality *= Urho3D::Sqrt (Urho3D::Sqrt (district->GetIndustryEvolutionPoints ()));
    return industryProductionQuality;
}

float CalculateProductionConsumptionOfFarms (GameConfiguration *configuration, District *district)
{
    float oneColonistConsumption = configuration->GetOneColonistFarmsProductionConsumption ();
    float minesConsumption = configuration->GetFarmsProductionMinesConsumption ();
    float industryConsumption = configuration->GetFarmsProductionIndustryConsumption ();

    float population = (district->GetMenCount () + district->GetWomenCount ());
    float totalConsumption = oneColonistConsumption * population;
    totalConsumption += minesConsumption * CalculateMinesProductionAmount (district, configuration);
    totalConsumption += industryConsumption * CalculateIndustryProductionAmount (district, configuration);
    return totalConsumption;
}

float CalculateProductionConsumptionOfMines (GameConfiguration *configuration, District *district)
{
    float oneColonistConsumption = configuration->GetOneColonistMinesProductionConsumption ();
    float farmsConsumption = configuration->GetMinesProductionFarmsConsumption ();
    float industryConsumption = configuration->GetMinesProductionIndustryConsumption ();

    float population = (district->GetMenCount () + district->GetWomenCount ());
    float totalConsumption = oneColonistConsumption * population;
    totalConsumption += farmsConsumption * CalculateFarmsProductionAmount (district, configuration);
    totalConsumption += industryConsumption * CalculateIndustryProductionAmount (district, configuration);
    return totalConsumption;
}

float CalculateProductionConsumptionOfIndustry (GameConfiguration *configuration, District *district)
{
    float oneColonistConsumption = configuration->GetOneColonistIndustryProductionConsumption ();
    float farmsConsumption = configuration->GetIndustryProductionFarmsConsumption ();
    float minesConsumption = configuration->GetIndustryProductionMinesConsumption ();

    float population = (district->GetMenCount () + district->GetWomenCount ());
    float totalConsumption = oneColonistConsumption * population;
    totalConsumption += farmsConsumption * CalculateFarmsProductionAmount (district, configuration);
    totalConsumption += minesConsumption * CalculateMinesProductionAmount (district, configuration);
    return totalConsumption;
}

float CalculateColonyPopulation (District *district)
{
    return district->GetMenCount () + district->GetWomenCount ();
}

float GetEvolutionPoints (District *district, DistrictEvolutionBranch branch)
{
    if (branch == DEB_FARMS)
    {
        return district->GetFarmsEvolutionPoints ();
    }
    else if (branch == DEB_MINES)
    {
        return district->GetMinesEvolutionPoints ();
    }
    else if (branch == DEB_INDUSTRY)
    {
        return district->GetIndustryEvolutionPoints ();
    }
    else if (branch == DEB_LOGISTICS)
    {
        return district->GetLogisticsEvolutionPoints ();
    }
    else if (branch == DEB_DEFENSE)
    {
        return district->GetDefenseEvolutionPoints ();
    }
}

void SetEvolutionPoints (District *district, DistrictEvolutionBranch branch, float evolutionPoints)
{
    if (branch == DEB_FARMS)
    {
        district->SetFarmsEvolutionPoints (evolutionPoints);
    }
    else if (branch == DEB_MINES)
    {
        district->SetMinesEvolutionPoints (evolutionPoints);
    }
    else if (branch == DEB_INDUSTRY)
    {
        district->SetIndustryEvolutionPoints (evolutionPoints);
    }
    else if (branch == DEB_LOGISTICS)
    {
        district->SetLogisticsEvolutionPoints (evolutionPoints);
    }
    else if (branch == DEB_DEFENSE)
    {
        district->SetDefenseEvolutionPoints (evolutionPoints);
    }
}

float GetBalance (District *district, DistrictEvolutionBranch branch)
{
    if (branch == DEB_FARMS)
    {
        return district->GetFarmsBalance ();
    }
    else if (branch == DEB_MINES)
    {
        return district->GetMinesBalance ();
    }
    else if (branch == DEB_INDUSTRY)
    {
        return district->GetIndustryBalance ();
    }
    else if (branch == DEB_LOGISTICS)
    {
        return district->GetLogisticsBalance ();
    }
    else if (branch == DEB_DEFENSE)
    {
        return district->GetDefenseBalance ();
    }
}

void SetBalance(District *district, DistrictEvolutionBranch branch, float balance)
{
    if (branch == DEB_FARMS)
    {
        district->SetFarmsBalance (balance);
    }
    else if (branch == DEB_MINES)
    {
        district->SetMinesBalance (balance);
    }
    else if (branch == DEB_INDUSTRY)
    {
        district->SetIndustryBalance (balance);
    }
    else if (branch == DEB_LOGISTICS)
    {
        district->SetLogisticsBalance (balance);
    }
    else if (branch == DEB_DEFENSE)
    {
        district->SetDefenseBalance (balance);
    }
}

float GetLastTradeBalanceAddition (District *district, DistrictEvolutionBranch branch)
{
    if (branch == DEB_FARMS)
    {
        return district->GetLastTradeFarmsBalanceAddition ();
    }
    else if (branch == DEB_MINES)
    {
        return district->GetLastTradeMinesBalanceAddition ();
    }
    else if (branch == DEB_INDUSTRY)
    {
        return district->GetLastTradeIndustryBalanceAddition ();
    }
    else if (branch == DEB_LOGISTICS)
    {
        return district->GetLastTradeLogisticsBalanceAddition ();
    }
    else if (branch == DEB_DEFENSE)
    {
        return district->GetLastTradeDefenseBalanceAddition ();
    }
}

void SetLastTradeBalanceAddition (District *district, DistrictEvolutionBranch branch, float lastTradeBalanceAddition)
{
    if (branch == DEB_FARMS)
    {
        district->SetLastTradeFarmsBalanceAddition (lastTradeBalanceAddition);
    }
    else if (branch == DEB_MINES)
    {
        district->SetLastTradeMinesBalanceAddition (lastTradeBalanceAddition);
    }
    else if (branch == DEB_INDUSTRY)
    {
        district->SetLastTradeIndustryBalanceAddition (lastTradeBalanceAddition);
    }
    else if (branch == DEB_LOGISTICS)
    {
        district->SetLastTradeLogisticsBalanceAddition (lastTradeBalanceAddition);
    }
    else if (branch == DEB_DEFENSE)
    {
        district->SetLastTradeDefenseBalanceAddition (lastTradeBalanceAddition);
    }
}

Urho3D::VariantMap GetLastTradeProduction (District *district, DistrictEvolutionBranch branch)
{
    if (branch == DEB_FARMS)
    {
        return district->GetLastTradeFarmsProduction ();
    }
    else if (branch == DEB_MINES)
    {
        return district->GetLastTradeMinesProduction ();
    }
    else if (branch == DEB_INDUSTRY)
    {
        return district->GetLastTradeIndustryProduction ();
    }
    else if (branch == DEB_LOGISTICS || branch == DEB_DEFENSE)
    {
        return Urho3D::Variant::emptyVariantMap;
    }
}

void SetLastTradeProduction(District *district, DistrictEvolutionBranch branch, const Urho3D::VariantMap &lastTradeProduction)
{
    if (branch == DEB_FARMS)
    {
        district->SetLastTradeFarmsProduction (lastTradeProduction);
    }
    else if (branch == DEB_MINES)
    {
        district->SetLastTradeMinesProduction (lastTradeProduction);
    }
    else if (branch == DEB_INDUSTRY)
    {
        district->SetLastTradeIndustryProduction (lastTradeProduction);
    }
}
}
}
