#include <Colonization/BuildConfiguration.hpp>
#include "BindFogOfWarCalculator.hpp"
#include <Urho3D/ThirdParty/AngelScript/angelscript.h>
#include <Urho3D/AngelScript/APITemplates.h>
#include <Colonization/Frontend/FogOfWarCalculator.hpp>

namespace Colonization
{
Urho3D::VariantMap FogOfWarCalculator_GetFogOfWarMap (FogOfWarCalculator *object)
{
    Urho3D::HashMap <Urho3D::StringHash, bool> fogOfWarMap = object->GetFogOfWarMap ();
    Urho3D::VariantMap variantMap;
    for (int index = 0; index < fogOfWarMap.Size (); index++)
        variantMap [fogOfWarMap.Keys ().At (index)] = Urho3D::Variant (fogOfWarMap.Values ().At (index));
    return variantMap;
}

void BindFogOfWarCalculator (Urho3D::Script *script)
{
    asIScriptEngine *engine = script->GetScriptEngine ();
    Urho3D::RegisterComponent <FogOfWarCalculator> (engine, "FogOfWarCalculator");
    BindFogOfWarCalculatorInterface (script, "FogOfWarCalculator");
}

void BindFogOfWarCalculatorInterface (Urho3D::Script *script, Urho3D::String className)
{
    asIScriptEngine *engine = script->GetScriptEngine ();
    engine->RegisterObjectMethod (className.CString (), "String get_playerName () const", asMETHOD (FogOfWarCalculator, GetPlayerName), asCALL_THISCALL);
    engine->RegisterObjectMethod (className.CString (), "String set_playerName (String playerName) const", asMETHOD (FogOfWarCalculator, SetPlayerName), asCALL_THISCALL);

    engine->RegisterObjectMethod (className.CString (), "bool IsDistrictVisible (StringHash districtHash)", asMETHOD (FogOfWarCalculator, IsDistrictVisible), asCALL_THISCALL);
    engine->RegisterObjectMethod (className.CString (), "VariantMap get_fogOfWarMap ()", asFUNCTION (FogOfWarCalculator_GetFogOfWarMap), asCALL_CDECL_OBJFIRST);
}
}
