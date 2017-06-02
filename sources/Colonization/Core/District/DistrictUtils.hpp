#pragma once
#include <Colonization/Core/District/District.hpp>
#include <Colonization/Core/InternalTradeArea/DistrictProductionInfo.hpp>
#include <Colonization/Core/GameConfiguration.hpp>

namespace Colonization
{
namespace DistrictUtils
{
//@ASBindGen Function
float CalculateFarmsProductionAmount (District *district, GameConfiguration *configuration);
//@ASBindGen Function
float CalculateMinesProductionAmount (District *district, GameConfiguration *configuration);
//@ASBindGen Function
float CalculateIndustryProductionAmount (District *district, GameConfiguration *configuration);

DistrictProductionInfo CalculateProductionOfFarms (District *district, GameConfiguration *configuration);
DistrictProductionInfo CalculateProductionOfMines (District *district, GameConfiguration *configuration);
DistrictProductionInfo CalculateProductionOfIndustry (District *district, GameConfiguration *configuration);

//@ASBindGen Function
Urho3D::VariantMap CalculateProductionOfFarmsAsVariantMap (District *district, GameConfiguration *configuration);
//@ASBindGen Function
Urho3D::VariantMap CalculateProductionOfMinesAsVariantMap (District *district, GameConfiguration *configuration);
//@ASBindGen Function
Urho3D::VariantMap CalculateProductionOfIndustryAsVariantMap (District *district, GameConfiguration *configuration);

//@ASBindGen Function
float CalculateFarmsProductionRelativePrice (District *district, GameConfiguration *configuration);
//@ASBindGen Function
float CalculateMinesProductionRelativePrice (District *district, GameConfiguration *configuration);
//@ASBindGen Function
float CalculateIndustryProductionRelativePrice (District *district, GameConfiguration *configuration);

//@ASBindGen Function
float CalculateFarmsProductionQuality (District *district, GameConfiguration *configuration);
//@ASBindGen Function
float CalculateMinesProductionQuality (District *district, GameConfiguration *configuration);
//@ASBindGen Function
float CalculateIndustryProductionQuality (District *district, GameConfiguration *configuration);

//@ASBindGen Function
float CalculateProductionConsumptionOfFarms (GameConfiguration *configuration, District *district);
//@ASBindGen Function
float CalculateProductionConsumptionOfMines (GameConfiguration *configuration, District *district);
//@ASBindGen Function
float CalculateProductionConsumptionOfIndustry (GameConfiguration *configuration, District *district);
}
}
