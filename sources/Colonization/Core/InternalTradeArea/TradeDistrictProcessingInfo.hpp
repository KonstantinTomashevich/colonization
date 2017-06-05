#pragma once
#include <Urho3D/Core/Object.h>
#include <Urho3D/Container/HashMap.h>

namespace Colonization
{
//@ASBindGen Class ObjectType=Ref
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

    //@ASBindGen Function
    float GetUnusedProductionOf (Urho3D::StringHash type) const;
    //@ASBindGen Function
    void SetUnusedProductionOf (Urho3D::StringHash type, float points);

    //@ASBindGen Function
    Urho3D::VariantMap GetDistrictProduction (Urho3D::StringHash districtHash) const;
    //@ASBindGen Function
    void SetDistrictProduction (Urho3D::StringHash districtHash, Urho3D::VariantMap &districtProduction);

    //@ASBindGen Function
    Urho3D::VariantMap GetDistrictBalanceAdditions (Urho3D::StringHash districtHash) const;
    //@ASBindGen Function
    void SetDistrictBalanceAdditions (Urho3D::StringHash districtHash, Urho3D::VariantMap &districtBalanceAdditions);

    //@ASBindGen Function OverrideName=get_unsoldTradeGoodsCost
    float GetUnsoldTradeGoodsCost () const;
    //@ASBindGen Function OverrideName=set_unsoldTradeGoodsCost
    void SetUnsoldTradeGoodsCost (float unsoldTradeGoodsCost);

    //@ASBindGen Function OverrideName=get_soldTradeGoodsCost
    float GetSoldTradeGoodsCost () const;
    //@ASBindGen Function OverrideName=set_soldTradeGoodsCost
    void SetSoldTradeGoodsCost (float soldTradeGoodsCost);
};
}
