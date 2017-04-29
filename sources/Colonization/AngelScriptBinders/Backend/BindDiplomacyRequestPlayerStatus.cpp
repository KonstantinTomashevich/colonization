#include <Colonization/BuildConfiguration.hpp>
#include "BindDiplomacyRequestPlayerStatus.hpp"
#include <Urho3D/ThirdParty/AngelScript/angelscript.h>
#include <Urho3D/AngelScript/APITemplates.h>
#include <Colonization/Backend/Diplomacy/DiplomacyRequest.hpp>

namespace Colonization
{
void BindDiplomacyRequestPlayerStatus(Urho3D::Script *script)
{
    asIScriptEngine *engine = script->GetScriptEngine ();
    engine->RegisterEnum ("DiplomacyRequestPlayerStatus");
    engine->RegisterEnumValue ("DiplomacyRequestPlayerStatus", "DRPSTATUS_WAITING", DRPSTATUS_WAITING);
    engine->RegisterEnumValue ("DiplomacyRequestPlayerStatus", "DRPSTATUS_DECLINED", DRPSTATUS_DECLINED);
    engine->RegisterEnumValue ("DiplomacyRequestPlayerStatus", "DRPSTATUS_ACCEPTED", DRPSTATUS_ACCEPTED);
}
}
