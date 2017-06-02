#pragma once
#include <Urho3D/Math/StringHash.h>
#include <Urho3D/Core/Variant.h>

namespace Colonization
{
//@ASBindGen Constant
const Urho3D::StringHash DISTRICT_PRODUCTION_AMOUNT_KEY ("DistrictProductionAmount");
//@ASBindGen Constant
const Urho3D::StringHash DISTRICT_PRODUCTION_RELATIVE_PRICE_KEY ("DistrictProductionRelativePrice");
//@ASBindGen Constant
const Urho3D::StringHash DISTRICT_PRODUCTION_QUALITY_KEY ("DistrictProductionQuality");
//@ASBindGen Constant
const Urho3D::StringHash DISTRICT_PRODUCTION_SELLABILITY_KEY ("DistrictProductionSellability");
//@ASBindGen Constant
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
