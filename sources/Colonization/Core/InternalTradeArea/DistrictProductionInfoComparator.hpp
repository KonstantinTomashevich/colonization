#pragma once
#include <Colonization/Core/InternalTradeArea/DistrictProductionInfo.hpp>

namespace Colonization
{
typedef bool (*DistrictProductionInfoComparator) (DistrictProductionInfo &first, DistrictProductionInfo &second);
namespace DistrictProductionInfoComparators
{
bool HigherSellability (DistrictProductionInfo &first, DistrictProductionInfo &second);
}
}
