#pragma once
#include <Urho3D/Core/Object.h>
#include <Urho3D/Container/HashMap.h>

namespace Colonization
{
class TradeDistrictProcessingInfo : public Urho3D::Object
{
URHO3D_OBJECT (TradeDistrictProcessingInfo, Object)
protected:
    Urho3D::HashMap <Urho3D::StringHash, float> unusedProduction_;
    Urho3D::HashMap <Urho3D::StringHash, Urho3D::VariantMap> districtsProduction_;
    Urho3D::HashMap <Urho3D::StringHash, Urho3D::VariantMap> districtsBalanceAdditions_;
    float unsoldTradeGoodsCost_;
    float soldTradeGoodsCost_;
public:
    TradeDistrictProcessingInfo (Urho3D::Context *context);
    virtual ~TradeDistrictProcessingInfo ();

    float GetUnusedProductionOf (Urho3D::StringHash type) const;
    void SetUnusedProductionOf (Urho3D::StringHash type, float points);

    Urho3D::VariantMap GetDistrictProduction (Urho3D::StringHash districtHash) const;
    void SetDistrictProduction (Urho3D::StringHash districtHash, Urho3D::VariantMap &districtProduction);

    Urho3D::VariantMap GetDistrictBalanceAdditions(Urho3D::StringHash districtHash) const;
    void SetDistrictBalanceAdditions (Urho3D::StringHash districtHash, Urho3D::VariantMap &districtBalanceAdditions);

    float GetUnsoldTradeGoodsCost () const;
    void SetUnsoldTradeGoodsCost (float unsoldTradeGoodsCost);

    float GetSoldTradeGoodsCost () const;
    void SetSoldTradeGoodsCost (float soldTradeGoodsCost);
};
}
