#include <Colonization/BuildConfiguration.hpp>
#include "DistrictProductionInfoComparator.hpp"

namespace Colonization
{
namespace DistrictProductionInfoComparators
{
bool HigherSellability (DistrictProductionInfo &first, DistrictProductionInfo &second)
{
    return (first.sellability_ > second.sellability_);
}
}
}
