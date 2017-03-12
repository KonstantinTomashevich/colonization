#include <Colonization/BuildConfiguration.hpp>
#include "BindVictoryTypesProcessorScriptDataAccessor.hpp"
#include <Urho3D/ThirdParty/AngelScript/angelscript.h>
#include <Urho3D/AngelScript/APITemplates.h>
#include <Colonization/Backend/VictoryProgressUpdater.hpp>

namespace Colonization
{
void BindVictoryTypesProcessorScriptDataAccessor (Urho3D::Script *script)
{
    asIScriptEngine *engine = script->GetScriptEngine ();
    Urho3D::RegisterObject <VictoryTypesProcessorScriptDataAccessor> (engine, "VictoryTypesProcessorScriptDataAccessor");
    BindVictoryTypesProcessorScriptDataAccessorInterface (script, "VictoryTypesProcessorScriptDataAccessor");
}

void BindVictoryTypesProcessorScriptDataAccessorInterface (Urho3D::Script *script, Urho3D::String className)
{
    asIScriptEngine *engine = script->GetScriptEngine ();
    engine->RegisterObjectMethod (className.CString (), "const PlayerInfo @+ get_playerInfo () const", asMETHOD (VictoryTypesProcessorScriptDataAccessor, GetPlayerInfo), asCALL_THISCALL);
    engine->RegisterObjectMethod (className.CString (), "uint get_districtsCount () const", asMETHOD (VictoryTypesProcessorScriptDataAccessor, GetDistrictsCount), asCALL_THISCALL);
    engine->RegisterObjectMethod (className.CString (), "const District @+ GetDistrictByIndex (int index) const", asMETHOD (VictoryTypesProcessorScriptDataAccessor, GetDistrictByIndex), asCALL_THISCALL);

    engine->RegisterObjectMethod (className.CString (), "uint get_unitsCount () const", asMETHOD (VictoryTypesProcessorScriptDataAccessor, GetUnitsCount), asCALL_THISCALL);
    engine->RegisterObjectMethod (className.CString (), "const Unit @+ GetUnitByIndex (int index) const", asMETHOD (VictoryTypesProcessorScriptDataAccessor, GetUnitByIndex), asCALL_THISCALL);

    engine->RegisterObjectMethod (className.CString (), "uint get_internalTradeAreasCount () const", asMETHOD (VictoryTypesProcessorScriptDataAccessor, GetInternalTradeAreasCount), asCALL_THISCALL);
    engine->RegisterObjectMethod (className.CString (), "const InternalTradeArea @+ GetInternalTradeAreaByIndex (int index) const", asMETHOD (VictoryTypesProcessorScriptDataAccessor, GetInternalTradeAreaByIndex), asCALL_THISCALL);
}
}
