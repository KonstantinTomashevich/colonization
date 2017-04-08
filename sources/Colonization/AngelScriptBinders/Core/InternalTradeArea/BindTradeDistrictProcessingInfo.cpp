#include <Colonization/BuildConfiguration.hpp>
#include "BindInternalTradeArea.hpp"
#include <Urho3D/ThirdParty/AngelScript/angelscript.h>
#include <Urho3D/AngelScript/APITemplates.h>
#include <Colonization/Core/InternalTradeArea/TradeDistrictProcessingInfo.hpp>

namespace Colonization
{
void BindTradeDistrictProcessingInfo (Urho3D::Script *script)
{
    asIScriptEngine *engine = script->GetScriptEngine ();
    Urho3D::RegisterObject <TradeDistrictProcessingInfo> (engine, "TradeDistrictProcessingInfo");

    engine->RegisterObjectMethod ("TradeDistrictProcessingInfo", "float GetUnusedProductionOf (StringHash type) const",
                                  asMETHOD (TradeDistrictProcessingInfo, GetUnusedProductionOf), asCALL_THISCALL);
    engine->RegisterObjectMethod ("TradeDistrictProcessingInfo", "float SetUnusedProductionOf (StringHash type, float points)",
                                  asMETHOD (TradeDistrictProcessingInfo, SetUnusedProductionOf), asCALL_THISCALL);

    engine->RegisterObjectMethod ("TradeDistrictProcessingInfo", "VariantMap GetDistrictProduction (StringHash districtHash) const",
                                  asMETHOD (TradeDistrictProcessingInfo, GetDistrictProduction), asCALL_THISCALL);
    engine->RegisterObjectMethod ("TradeDistrictProcessingInfo", "void SetDistrictProduction (StringHash districtHash, VariantMap districtProduction)",
                                  asMETHOD (TradeDistrictProcessingInfo, SetDistrictProduction), asCALL_THISCALL);

    engine->RegisterObjectMethod ("TradeDistrictProcessingInfo", "VariantMap GetDistrictBalanceAdditions (StringHash districtHash) const",
                                  asMETHOD (TradeDistrictProcessingInfo, GetDistrictBalanceAdditions), asCALL_THISCALL);
    engine->RegisterObjectMethod ("TradeDistrictProcessingInfo", "void SetDistrictBalanceAdditions (StringHash districtHash, VariantMap districtBalanceAdditions)",
                                  asMETHOD (TradeDistrictProcessingInfo, SetDistrictBalanceAdditions), asCALL_THISCALL);

    engine->RegisterObjectMethod ("TradeDistrictProcessingInfo", "float get_unsoldTradeGoodsCost () const", asMETHOD (TradeDistrictProcessingInfo, GetUnsoldTradeGoodsCost), asCALL_THISCALL);
    engine->RegisterObjectMethod ("TradeDistrictProcessingInfo", "float set_unsoldTradeGoodsCost (float cost)", asMETHOD (TradeDistrictProcessingInfo, SetUnsoldTradeGoodsCost), asCALL_THISCALL);

    engine->RegisterObjectMethod ("TradeDistrictProcessingInfo", "float get_soldTradeGoodsCost () const", asMETHOD (TradeDistrictProcessingInfo, GetSoldTradeGoodsCost), asCALL_THISCALL);
    engine->RegisterObjectMethod ("TradeDistrictProcessingInfo", "float set_soldTradeGoodsCost (float cost)", asMETHOD (TradeDistrictProcessingInfo, SetSoldTradeGoodsCost), asCALL_THISCALL);
}
}
