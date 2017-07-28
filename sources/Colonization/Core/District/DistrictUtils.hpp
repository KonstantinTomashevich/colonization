#pragma once
#include <Colonization/Core/InternalTradeArea/DistrictProductionInfo.hpp>
namespace Colonization
{
class GameConfiguration;
class District;

//@ASBindGen Enum
enum DistrictEvolutionBranch
{
    DEB_FARMS = 0,
    DEB_MINES = 1,
    DEB_INDUSTRY = 2,
    DEB_LOGISTICS = 3,
    DEB_DEFENSE = 4
};

namespace DistrictUtils
{
//@ASBindGen Function AddNamespaceToCallName=DistrictUtils
float CleanupColonyFromDistrict (District *district);
//@ASBindGen Function AddNamespaceToCallName=DistrictUtils
float CalculateFarmsProductionAmount (District *district, GameConfiguration *configuration);
//@ASBindGen Function AddNamespaceToCallName=DistrictUtils
float CalculateMinesProductionAmount (District *district, GameConfiguration *configuration);
//@ASBindGen Function AddNamespaceToCallName=DistrictUtils
float CalculateIndustryProductionAmount (District *district, GameConfiguration *configuration);

DistrictProductionInfo CalculateProductionOfFarms (District *district, GameConfiguration *configuration);
DistrictProductionInfo CalculateProductionOfMines (District *district, GameConfiguration *configuration);
DistrictProductionInfo CalculateProductionOfIndustry (District *district, GameConfiguration *configuration);

//@ASBindGen Function AddNamespaceToCallName=DistrictUtils
Urho3D::VariantMap CalculateProductionOfFarmsAsVariantMap (District *district, GameConfiguration *configuration);
//@ASBindGen Function AddNamespaceToCallName=DistrictUtils
Urho3D::VariantMap CalculateProductionOfMinesAsVariantMap (District *district, GameConfiguration *configuration);
//@ASBindGen Function AddNamespaceToCallName=DistrictUtils
Urho3D::VariantMap CalculateProductionOfIndustryAsVariantMap (District *district, GameConfiguration *configuration);

//@ASBindGen Function AddNamespaceToCallName=DistrictUtils
float CalculateFarmsProductionRelativePrice (District *district, GameConfiguration *configuration);
//@ASBindGen Function AddNamespaceToCallName=DistrictUtils
float CalculateMinesProductionRelativePrice (District *district, GameConfiguration *configuration);
//@ASBindGen Function AddNamespaceToCallName=DistrictUtils
float CalculateIndustryProductionRelativePrice (District *district, GameConfiguration *configuration);

//@ASBindGen Function AddNamespaceToCallName=DistrictUtils
float CalculateFarmsProductionQuality (District *district, GameConfiguration *configuration);
//@ASBindGen Function AddNamespaceToCallName=DistrictUtils
float CalculateMinesProductionQuality (District *district, GameConfiguration *configuration);
//@ASBindGen Function AddNamespaceToCallName=DistrictUtils
float CalculateIndustryProductionQuality (District *district, GameConfiguration *configuration);

//@ASBindGen Function AddNamespaceToCallName=DistrictUtils
float CalculateProductionConsumptionOfFarms (GameConfiguration *configuration, District *district);
//@ASBindGen Function AddNamespaceToCallName=DistrictUtils
float CalculateProductionConsumptionOfMines (GameConfiguration *configuration, District *district);
//@ASBindGen Function AddNamespaceToCallName=DistrictUtils
float CalculateProductionConsumptionOfIndustry (GameConfiguration *configuration, District *district);

//@ASBindGen Function AddNamespaceToCallName=DistrictUtils
float CalculateColonyPopulation (District *district);
//@ASBindGen Function AddNamespaceToCallName=DistrictUtils
float GetEvolutionPoints (District *district, DistrictEvolutionBranch branch);
//@ASBindGen Function AddNamespaceToCallName=DistrictUtils
void SetEvolutionPoints (District *district, DistrictEvolutionBranch branch, float evolutionPoints);

//@ASBindGen Function AddNamespaceToCallName=DistrictUtils
float GetBalance (District *district, DistrictEvolutionBranch branch);
//@ASBindGen Function AddNamespaceToCallName=DistrictUtils
void SetBalance (District *district, DistrictEvolutionBranch branch, float balance);

//@ASBindGen Function AddNamespaceToCallName=DistrictUtils
float GetLastTradeBalanceAddition (District *district, DistrictEvolutionBranch branch);
//@ASBindGen Function AddNamespaceToCallName=DistrictUtils
void SetLastTradeBalanceAddition (District *district, DistrictEvolutionBranch branch, float lastTradeBalanceAddition);

//@ASBindGen Function AddNamespaceToCallName=DistrictUtils
Urho3D::VariantMap GetLastTradeProduction (District *district, DistrictEvolutionBranch branch);
//@ASBindGen Function AddNamespaceToCallName=DistrictUtils
void SetLastTradeProduction (District *district, DistrictEvolutionBranch branch, const Urho3D::VariantMap &lastTradeProduction);
}
}
