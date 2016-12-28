#include <Colonization/BuildConfiguration.hpp>
#include "BindInternalTradeArea.hpp"
#include <Urho3D/ThirdParty/AngelScript/angelscript.h>
#include <Urho3D/AngelScript/APITemplates.h>
#include <Colonization/Core/InternalTradeArea.hpp>

namespace Colonization
{
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
}

void BindInternalTradeAreaInterface (Urho3D::Script *script, Urho3D::String className)
{
    asIScriptEngine *engine = script->GetScriptEngine ();
}
}
