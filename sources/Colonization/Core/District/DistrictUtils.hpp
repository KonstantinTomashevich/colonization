#pragma once
#include <Colonization/Core/District/District.hpp>
#include <Colonization/Core/InternalTradeArea/DistrictProductionInfo.hpp>
#include <Colonization/Core/GameConfiguration.hpp>

namespace Colonization
{
namespace DistrictUtils
{
float CalculateFarmsProductionAmount (District *district, GameConfiguration *configuration);
float CalculateMinesProductionAmount (District *district, GameConfiguration *configuration);
float CalculateIndustryProductionAmount (District *district, GameConfiguration *configuration);

DistrictProductionInfo CalculateProductionOfFarms (District *district, GameConfiguration *configuration);
DistrictProductionInfo CalculateProductionOfMines (District *district, GameConfiguration *configuration);
DistrictProductionInfo CalculateProductionOfIndustry (District *district, GameConfiguration *configuration);

float CalculateFarmsProductionRelativePrice (District *district, GameConfiguration *configuration);
float CalculateMinesProductionRelativePrice (District *district, GameConfiguration *configuration);
float CalculateIndustryProductionRelativePrice (District *district, GameConfiguration *configuration);

float CalculateFarmsProductionQuality (District *district, GameConfiguration *configuration);
float CalculateMinesProductionQuality (District *district, GameConfiguration *configuration);
float CalculateIndustryProductionQuality (District *district, GameConfiguration *configuration);

float CalculateProductionConsumptionOfFarms (GameConfiguration *configuration, District *district);
float CalculateProductionConsumptionOfMines (GameConfiguration *configuration, District *district);
float CalculateProductionConsumptionOfIndustry (GameConfiguration *configuration, District *district);
}
}
