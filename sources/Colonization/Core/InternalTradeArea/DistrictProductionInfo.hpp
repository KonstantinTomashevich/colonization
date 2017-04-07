#pragma once
#include <Urho3D/Math/StringHash.h>
#include <Urho3D/Core/Variant.h>

namespace Colonization
{
const Urho3D::StringHash DISTRICT_PRODUCTION_AMOUNT_KEY ("DistrictProductionAmount");
const Urho3D::StringHash DISTRICT_PRODUCTION_RELATIVE_PRICE_KEY ("DistrictProductionRelativePrice");
const Urho3D::StringHash DISTRICT_PRODUCTION_QUALITY_KEY ("DistrictProductionQuality");
const Urho3D::StringHash DISTRICT_PRODUCTION_SELLABILITY_KEY ("DistrictProductionSellability");
const Urho3D::StringHash DISTRICT_PRODUCTION_SELLED_KEY ("DistrictProductionSelled");

struct DistrictProductionInfo
{
    Urho3D::StringHash districtHash_;
    float amount_;
    float relativePrice_;
    float quality_;
    float sellability_;
    float selled_;

    void CalculateSellability ();
    Urho3D::VariantMap ToVariantMap ();
};
}
