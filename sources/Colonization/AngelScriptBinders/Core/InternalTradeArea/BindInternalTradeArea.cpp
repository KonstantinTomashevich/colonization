#include <Colonization/BuildConfiguration.hpp>
#include "BindInternalTradeArea.hpp"
#include <Urho3D/ThirdParty/AngelScript/angelscript.h>
#include <Urho3D/AngelScript/APITemplates.h>
#include <Colonization/Core/InternalTradeArea/InternalTradeArea.hpp>

namespace Colonization
{
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
    asIScriptEngine *engine = script->GetScriptEngine ();
    Urho3D::RegisterComponent <InternalTradeArea> (engine, "InternalTradeArea");

    engine->RegisterObjectMethod ("InternalTradeArea", "TradeDistrictProcessingInfo @ProcessTrade (Map @+ map, float updateDelay, bool writeDistrictsBalance)", asFUNCTION (InternalTradeArea_ProcessTrade), asCALL_CDECL_OBJFIRST);
    engine->RegisterObjectMethod ("InternalTradeArea", "int get_districtsHashesCount ()", asMETHOD (InternalTradeArea, GetDistrictsHashesCount), asCALL_THISCALL);
    engine->RegisterObjectMethod ("InternalTradeArea", "StringHash GetDistrictHashByIndex (int index)", asMETHOD (InternalTradeArea, GetDistrictHashByIndex), asCALL_THISCALL);
    engine->RegisterObjectMethod ("InternalTradeArea", "void AddDistrictHash (StringHash districtHash)", asMETHOD (InternalTradeArea, AddDistrictHash), asCALL_THISCALL);
    engine->RegisterObjectMethod ("InternalTradeArea", "bool ContainsDistrictHash (StringHash districtHash)", asMETHOD (InternalTradeArea, ContainsDistrictHash), asCALL_THISCALL);
}
}
