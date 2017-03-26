#include <Colonization/BuildConfiguration.hpp>
#include "BindInternalTradeArea.hpp"
#include <Urho3D/ThirdParty/AngelScript/angelscript.h>
#include <Urho3D/AngelScript/APITemplates.h>
#include <Colonization/Core/InternalTradeArea.hpp>

namespace Colonization
{
Urho3D::StringHash get_DISTRICT_PRODUCTION_AMOUNT_KEY ()
{
    return DISTRICT_PRODUCTION_AMOUNT_KEY;
}

Urho3D::StringHash get_DISTRICT_PRODUCTION_RELATIVE_PRICE_KEY ()
{
    return DISTRICT_PRODUCTION_RELATIVE_PRICE_KEY;
}

Urho3D::StringHash get_DISTRICT_PRODUCTION_QUALITY_KEY ()
{
    return DISTRICT_PRODUCTION_QUALITY_KEY;
}

Urho3D::StringHash get_DISTRICT_PRODUCTION_SELLABILITY_KEY ()
{
    return DISTRICT_PRODUCTION_SELLABILITY_KEY;
}

Urho3D::StringHash get_DISTRICT_PRODUCTION_SELLED_KEY ()
{
    return DISTRICT_PRODUCTION_SELLED_KEY;
}

TradeDistrictProcessingInfo *InternalTradeArea_ProcessTrade (InternalTradeArea *object, Map *map, float updateDelay, bool writeDistrictsBalance)
{
    Urho3D::SharedPtr <TradeDistrictProcessingInfo> result = object->ProcessTrade (map, updateDelay, writeDistrictsBalance);
    TradeDistrictProcessingInfo *infoPtr = result.Get ();
    result.Detach ();
    infoPtr->AddRef ();
    return infoPtr;
}

void BindInternalTradeArea (Urho3D::Script *script)
{
    BindDistrictProductionKeys (script);
    BindTradeDistrictsProcessingInfo (script);

    asIScriptEngine *engine = script->GetScriptEngine ();
    Urho3D::RegisterComponent <InternalTradeArea> (engine, "InternalTradeArea");
    BindInternalTradeAreaInterface (script, "InternalTradeArea");
}

void BindDistrictProductionKeys(Urho3D::Script *script)
{
    asIScriptEngine *engine = script->GetScriptEngine ();
    engine->RegisterGlobalFunction ("StringHash get_DISTRICT_PRODUCTION_AMOUNT_KEY ()", asFUNCTION (get_DISTRICT_PRODUCTION_AMOUNT_KEY), asCALL_CDECL);
    engine->RegisterGlobalFunction ("StringHash get_DISTRICT_PRODUCTION_RELATIVE_PRICE_KEY ()", asFUNCTION (get_DISTRICT_PRODUCTION_RELATIVE_PRICE_KEY), asCALL_CDECL);
    engine->RegisterGlobalFunction ("StringHash get_DISTRICT_PRODUCTION_QUALITY_KEY ()", asFUNCTION (get_DISTRICT_PRODUCTION_QUALITY_KEY), asCALL_CDECL);
    engine->RegisterGlobalFunction ("StringHash get_DISTRICT_PRODUCTION_SELLABILITY_KEY ()", asFUNCTION (get_DISTRICT_PRODUCTION_SELLABILITY_KEY), asCALL_CDECL);
    engine->RegisterGlobalFunction ("StringHash get_DISTRICT_PRODUCTION_SELLED_KEY ()", asFUNCTION (get_DISTRICT_PRODUCTION_SELLED_KEY), asCALL_CDECL);
}

void BindTradeDistrictsProcessingInfo (Urho3D::Script *script)
{
    asIScriptEngine *engine = script->GetScriptEngine ();
    Urho3D::RegisterObject <TradeDistrictProcessingInfo> (engine, "TradeDistrictProcessingInfo");

    engine->RegisterObjectMethod ("TradeDistrictProcessingInfo", "float GetUnusedProductionOf (StringHash type)",
                                  asMETHOD (TradeDistrictProcessingInfo, GetUnusedProductionOf), asCALL_THISCALL);

    engine->RegisterObjectMethod ("TradeDistrictProcessingInfo", "float SetUnusedProductionOf (StringHash type, float points)",
                                  asMETHOD (TradeDistrictProcessingInfo, SetUnusedProductionOf), asCALL_THISCALL);

    engine->RegisterObjectMethod ("TradeDistrictProcessingInfo", "float get_unsoldTradeGoodsCost ()", asMETHOD (TradeDistrictProcessingInfo, GetUnsoldTradeGoodsCost), asCALL_THISCALL);
    engine->RegisterObjectMethod ("TradeDistrictProcessingInfo", "float set_unsoldTradeGoodsCost (float cost)", asMETHOD (TradeDistrictProcessingInfo, SetUnsoldTradeGoodsCost), asCALL_THISCALL);

    engine->RegisterObjectMethod ("TradeDistrictProcessingInfo", "float get_soldTradeGoodsCost ()", asMETHOD (TradeDistrictProcessingInfo, GetSoldTradeGoodsCost), asCALL_THISCALL);
    engine->RegisterObjectMethod ("TradeDistrictProcessingInfo", "float set_soldTradeGoodsCost (float cost)", asMETHOD (TradeDistrictProcessingInfo, SetSoldTradeGoodsCost), asCALL_THISCALL);
}

void BindInternalTradeAreaInterface (Urho3D::Script *script, Urho3D::String className)
{
    asIScriptEngine *engine = script->GetScriptEngine ();
    engine->RegisterObjectMethod (className.CString (), "TradeDistrictProcessingInfo @ProcessTrade (Map @+ map, float updateDelay, bool writeDistrictsBalance)", asFUNCTION (InternalTradeArea_ProcessTrade), asCALL_CDECL_OBJFIRST);
    engine->RegisterObjectMethod (className.CString (), "int get_districtsHashesCount ()", asMETHOD (InternalTradeArea, GetDistrictsHashesCount), asCALL_THISCALL);
    engine->RegisterObjectMethod (className.CString (), "StringHash GetDistrictHashByIndex (int index)", asMETHOD (InternalTradeArea, GetDistrictHashByIndex), asCALL_THISCALL);
    engine->RegisterObjectMethod (className.CString (), "void AddDistrictHash (StringHash districtHash)", asMETHOD (InternalTradeArea, AddDistrictHash), asCALL_THISCALL);
    engine->RegisterObjectMethod (className.CString (), "bool ContainsDistrictHash (StringHash districtHash)", asMETHOD (InternalTradeArea, ContainsDistrictHash), asCALL_THISCALL);
}
}
