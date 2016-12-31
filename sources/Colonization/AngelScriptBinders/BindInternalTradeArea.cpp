#include <Colonization/BuildConfiguration.hpp>
#include "BindInternalTradeArea.hpp"
#include <Urho3D/ThirdParty/AngelScript/angelscript.h>
#include <Urho3D/AngelScript/APITemplates.h>
#include <Colonization/Core/InternalTradeArea.hpp>

namespace Colonization
{
TradeDistrictProcessingInfo *InternalTradeArea_ProcessTrade (InternalTradeArea *object, Map *map)
{
    Urho3D::SharedPtr <TradeDistrictProcessingInfo> result = object->ProcessTrade (map);
    TradeDistrictProcessingInfo *infoPtr = result.Get ();
    result.Detach ();
    infoPtr->AddRef ();
    return infoPtr;
}

void BindInternalTradeArea (Urho3D::Script *script)
{
    BindTradeDistrictsProcessingInfo (script);
    asIScriptEngine *engine = script->GetScriptEngine ();
    Urho3D::RegisterComponent <InternalTradeArea> (engine, "InternalTradeArea");
    BindInternalTradeAreaInterface (script, "InternalTradeArea");
}

void BindTradeDistrictsProcessingInfo (Urho3D::Script *script)
{
    asIScriptEngine *engine = script->GetScriptEngine ();
    Urho3D::RegisterObject <TradeDistrictProcessingInfo> (engine, "TradeDistrictProcessingInfo");

    engine->RegisterObjectMethod ("TradeDistrictProcessingInfo", "float GetUnusedEvolutionPointsOf (StringHash type)",
                                  asMETHOD (TradeDistrictProcessingInfo, GetUnusedEvolutionPointsOf), asCALL_THISCALL);

    engine->RegisterObjectMethod ("TradeDistrictProcessingInfo", "float SetUnusedEvolutionPointsOf (StringHash type, float points)",
                                  asMETHOD (TradeDistrictProcessingInfo, SetUnusedEvolutionPointsOf), asCALL_THISCALL);

    engine->RegisterObjectMethod ("TradeDistrictProcessingInfo", "float get_unsoldTradeGoodsCost ()", asMETHOD (TradeDistrictProcessingInfo, GetUnsoldTradeGoodsCost), asCALL_THISCALL);
    engine->RegisterObjectMethod ("TradeDistrictProcessingInfo", "float set_unsoldTradeGoodsCost (float cost)", asMETHOD (TradeDistrictProcessingInfo, SetUnsoldTradeGoodsCost), asCALL_THISCALL);

    engine->RegisterObjectMethod ("TradeDistrictProcessingInfo", "float get_soldTradeGoodsCost ()", asMETHOD (TradeDistrictProcessingInfo, GetSoldTradeGoodsCost), asCALL_THISCALL);
    engine->RegisterObjectMethod ("TradeDistrictProcessingInfo", "float set_soldTradeGoodsCost (float cost)", asMETHOD (TradeDistrictProcessingInfo, SetSoldTradeGoodsCost), asCALL_THISCALL);

    engine->RegisterObjectMethod ("TradeDistrictProcessingInfo", "float get_logisticsBonus ()", asMETHOD (TradeDistrictProcessingInfo, GetLogisticsBonus), asCALL_THISCALL);
    engine->RegisterObjectMethod ("TradeDistrictProcessingInfo", "float set_logisticsBonus (float logisticsBonus)", asMETHOD (TradeDistrictProcessingInfo, SetLogisticsBonus), asCALL_THISCALL);

    engine->RegisterObjectMethod ("TradeDistrictProcessingInfo", "float get_defenseBonus ()", asMETHOD (TradeDistrictProcessingInfo, GetDefenseBonus), asCALL_THISCALL);
    engine->RegisterObjectMethod ("TradeDistrictProcessingInfo", "float set_defenseBonus (float defenseBonus)", asMETHOD (TradeDistrictProcessingInfo, SetDefenseBonus), asCALL_THISCALL);
}

void BindInternalTradeAreaInterface (Urho3D::Script *script, Urho3D::String className)
{
    asIScriptEngine *engine = script->GetScriptEngine ();
    engine->RegisterObjectMethod (className.CString (), "TradeDistrictProcessingInfo @ProcessTrade (Map @+ map)", asFUNCTION (InternalTradeArea_ProcessTrade), asCALL_CDECL_OBJFIRST);
    engine->RegisterObjectMethod (className.CString (), "int get_districtsHashesCount ()", asMETHOD (InternalTradeArea, GetDistrictsHashesCount), asCALL_THISCALL);
    engine->RegisterObjectMethod (className.CString (), "StringHash GetDistrictHashByIndex (int index)", asMETHOD (InternalTradeArea, GetDistrictHashByIndex), asCALL_THISCALL);
    engine->RegisterObjectMethod (className.CString (), "void AddDistrictHash (StringHash districtHash)", asMETHOD (InternalTradeArea, AddDistrictHash), asCALL_THISCALL);
    engine->RegisterObjectMethod (className.CString (), "bool ContainsDistrictHash (StringHash districtHash)", asMETHOD (InternalTradeArea, ContainsDistrictHash), asCALL_THISCALL);
}
}
