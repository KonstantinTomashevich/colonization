#include <Colonization/BuildConfiguration.hpp>
#include "TradeDistrictProcessingInfo.hpp"

namespace Colonization
{
TradeDistrictProcessingInfo::TradeDistrictProcessingInfo (Urho3D::Context *context) : Urho3D::Object (context),
    unusedProduction_ (),
    unsoldTradeGoodsCost_ (0.0f),
    soldTradeGoodsCost_ (0.0f)
{

}

TradeDistrictProcessingInfo::~TradeDistrictProcessingInfo ()
{

}

float TradeDistrictProcessingInfo::GetUnusedProductionOf (Urho3D::StringHash type) const
{
    float *unusedProduction = unusedProduction_ [type];
    if (unusedProduction)
    {
        return *unusedProduction;
    }
    else
    {
        return 0.0f;
    }
}

void TradeDistrictProcessingInfo::SetUnusedProductionOf (Urho3D::StringHash type, float points)
{
    unusedProduction_ [type] = points;
}

Urho3D::VariantMap TradeDistrictProcessingInfo::GetDistrictProduction (Urho3D::StringHash districtHash) const
{
    Urho3D::VariantMap *districtProduction = districtsProduction_ [districtHash];
    if (districtProduction)
    {
        return *districtProduction;
    }
    else
    {
        return Urho3D::Variant::emptyVariantMap;
    }
}

void TradeDistrictProcessingInfo::SetDistrictProduction (Urho3D::StringHash districtHash, Urho3D::VariantMap &districtProduction)
{
    districtsProduction_ [districtHash] = districtProduction;
}

Urho3D::VariantMap TradeDistrictProcessingInfo::GetDistrictBalanceAdditions (Urho3D::StringHash districtHash) const
{
    Urho3D::VariantMap *districtBalanceAdditions = districtsBalanceAdditions_ [districtHash];
    if (districtBalanceAdditions)
    {
        return *districtBalanceAdditions;
    }
    else
    {
        return Urho3D::Variant::emptyVariantMap;
    }
}

void TradeDistrictProcessingInfo::SetDistrictBalanceAdditions (Urho3D::StringHash districtHash, Urho3D::VariantMap &districtBalanceAdditions)
{
    districtsBalanceAdditions_ [districtHash] = districtBalanceAdditions;
}

float TradeDistrictProcessingInfo::GetUnsoldTradeGoodsCost () const
{
    return unsoldTradeGoodsCost_;
}

void TradeDistrictProcessingInfo::SetUnsoldTradeGoodsCost (float unsoldTradeGoodsCost)
{
    unsoldTradeGoodsCost_ = unsoldTradeGoodsCost;
}

float TradeDistrictProcessingInfo::GetSoldTradeGoodsCost () const
{
    return soldTradeGoodsCost_;
}

void TradeDistrictProcessingInfo::SetSoldTradeGoodsCost (float soldTradeGoodsCost)
{
    soldTradeGoodsCost_ = soldTradeGoodsCost;
}
}
