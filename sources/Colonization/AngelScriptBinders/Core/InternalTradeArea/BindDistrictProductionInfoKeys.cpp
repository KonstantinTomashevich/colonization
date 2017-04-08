#include <Colonization/BuildConfiguration.hpp>
#include "BindInternalTradeArea.hpp"
#include <Urho3D/ThirdParty/AngelScript/angelscript.h>
#include <Urho3D/AngelScript/APITemplates.h>
#include <Colonization/Core/InternalTradeArea/DistrictProductionInfo.hpp>

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

void BindDistrictProductionInfoKeys (Urho3D::Script *script)
{
    asIScriptEngine *engine = script->GetScriptEngine ();
    engine->RegisterGlobalFunction ("StringHash get_DISTRICT_PRODUCTION_AMOUNT_KEY ()", asFUNCTION (get_DISTRICT_PRODUCTION_AMOUNT_KEY), asCALL_CDECL);
    engine->RegisterGlobalFunction ("StringHash get_DISTRICT_PRODUCTION_RELATIVE_PRICE_KEY ()", asFUNCTION (get_DISTRICT_PRODUCTION_RELATIVE_PRICE_KEY), asCALL_CDECL);
    engine->RegisterGlobalFunction ("StringHash get_DISTRICT_PRODUCTION_QUALITY_KEY ()", asFUNCTION (get_DISTRICT_PRODUCTION_QUALITY_KEY), asCALL_CDECL);
    engine->RegisterGlobalFunction ("StringHash get_DISTRICT_PRODUCTION_SELLABILITY_KEY ()", asFUNCTION (get_DISTRICT_PRODUCTION_SELLABILITY_KEY), asCALL_CDECL);
    engine->RegisterGlobalFunction ("StringHash get_DISTRICT_PRODUCTION_SELLED_KEY ()", asFUNCTION (get_DISTRICT_PRODUCTION_SELLED_KEY), asCALL_CDECL);
}
}
