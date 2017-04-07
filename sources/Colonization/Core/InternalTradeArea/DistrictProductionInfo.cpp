#include <Colonization/BuildConfiguration.hpp>
#include "DistrictProductionInfo.hpp"

namespace Colonization
{
void DistrictProductionInfo::CalculateSellability ()
{
    sellability_ = quality_ / relativePrice_;
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
