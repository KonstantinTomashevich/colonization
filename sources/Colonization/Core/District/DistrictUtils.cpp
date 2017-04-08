#include <Colonization/BuildConfiguration.hpp>
#include "DistrictUtils.hpp"

namespace Colonization
{
namespace DistrictUtils
{
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
            sqrt (district->GetFarmsEvolutionPoints ()) * district->GetLandAverageFertility () * climateModifer;
}

float CalculateMinesProductionAmount (District *district, GameConfiguration *configuration)
{
    float districtPopulation = district->GetMenCount () + district->GetWomenCount ();
    return districtPopulation * configuration->GetMinesProductionPerColonist () *
            sqrt (district->GetMinesEvolutionPoints ());
}

float CalculateIndustryProductionAmount (District *district, GameConfiguration *configuration)
{
    float districtPopulation = district->GetMenCount () + district->GetWomenCount ();
    return districtPopulation * configuration->GetIndustryProductionPerColonist () *
            sqrt (district->GetIndustryEvolutionPoints ());
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

    farmsProductionRelativePrice /= sqrt (district->GetLandAverageFertility ());
    farmsProductionRelativePrice *= sqrt (sqrt (district->GetFarmsEvolutionPoints ()));
    return farmsProductionRelativePrice;
}

float CalculateMinesProductionRelativePrice (District *district, GameConfiguration *configuration)
{
    float minesProductionRelativePrice = 1.0f;
    if (district->HasCoalDeposits ())
    {
        minesProductionRelativePrice *= configuration->GetMinesProductionRelativePriceHasCoalModifer ();
    }
    if (district->HasIronDeposits ())
    {
        minesProductionRelativePrice *= configuration->GetMinesProductionRelativePriceHasIronModifer ();
    }
    if (district->HasSilverDeposits ())
    {
        minesProductionRelativePrice *= configuration->GetMinesProductionRelativePriceHasSilverModifer ();
    }
    if (district->HasGoldDeposits ())
    {
        minesProductionRelativePrice *= configuration->GetMinesProductionRelativePriceHasGoldModifer ();
    }

    minesProductionRelativePrice *= sqrt (sqrt (district->GetMinesEvolutionPoints ()));
    return minesProductionRelativePrice;
}

float CalculateIndustryProductionRelativePrice (District *district, GameConfiguration *configuration)
{
    float industryProductionRelativePrice = 1.0f;
    if (district->HasCoalDeposits ())
    {
        industryProductionRelativePrice *= configuration->GetIndustryProductionRelativePriceHasCoalModifer ();
    }
    if (district->HasIronDeposits ())
    {
        industryProductionRelativePrice *= configuration->GetIndustryProductionRelativePriceHasIronModifer ();
    }
    if (district->HasSilverDeposits ())
    {
        industryProductionRelativePrice *= configuration->GetIndustryProductionRelativePriceHasSilverModifer ();
    }
    if (district->HasGoldDeposits ())
    {
        industryProductionRelativePrice *= configuration->GetIndustryProductionRelativePriceHasGoldModifer ();
    }

    industryProductionRelativePrice *= sqrt (sqrt (district->GetIndustryEvolutionPoints ()));
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
    farmsProductionQuality *= sqrt (sqrt (district->GetFarmsEvolutionPoints ()));
    return farmsProductionQuality;
}

float CalculateMinesProductionQuality (District *district, GameConfiguration *configuration)
{
    float minesProductionQuality = 1.0f;
    if (district->HasCoalDeposits ())
    {
        minesProductionQuality *= configuration->GetMinesProductionQualityHasCoalModifer ();
    }
    if (district->HasIronDeposits ())
    {
        minesProductionQuality *= configuration->GetMinesProductionQualityHasIronModifer ();
    }
    if (district->HasSilverDeposits ())
    {
        minesProductionQuality *= configuration->GetMinesProductionQualityHasSilverModifer ();
    }
    if (district->HasGoldDeposits ())
    {
        minesProductionQuality *= configuration->GetMinesProductionQualityHasGoldModifer ();
    }

    minesProductionQuality *= sqrt (sqrt (district->GetMinesEvolutionPoints ()));
    return minesProductionQuality;
}

float CalculateIndustryProductionQuality (District *district, GameConfiguration *configuration)
{
    float industryProductionQuality = 1.0f;
    if (district->HasCoalDeposits ())
    {
        industryProductionQuality *= configuration->GetIndustryProductionQualityHasCoalModifer ();
    }
    if (district->HasIronDeposits ())
    {
        industryProductionQuality *= configuration->GetIndustryProductionQualityHasIronModifer ();
    }
    if (district->HasSilverDeposits ())
    {
        industryProductionQuality *= configuration->GetIndustryProductionQualityHasSilverModifer ();
    }
    if (district->HasGoldDeposits ())
    {
        industryProductionQuality *= configuration->GetIndustryProductionQualityHasGoldModifer ();
    }

    industryProductionQuality *= sqrt (sqrt (district->GetIndustryEvolutionPoints ()));
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
}
}
