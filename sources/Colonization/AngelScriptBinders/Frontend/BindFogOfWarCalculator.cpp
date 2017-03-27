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
    for (Urho3D::HashMap <Urho3D::StringHash, bool>::ConstIterator iterator = fogOfWarMap.Begin ();
         iterator != fogOfWarMap.End (); iterator++)
    {
        variantMap [iterator->first_] = Urho3D::Variant (iterator->second_);
    }
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
    engine->RegisterObjectMethod (className.CString (), "void set_playerName (String playerName)", asMETHOD (FogOfWarCalculator, SetPlayerName), asCALL_THISCALL);

    engine->RegisterObjectMethod (className.CString (), "bool IsDistrictVisible (StringHash districtHash)", asMETHOD (FogOfWarCalculator, IsDistrictVisible), asCALL_THISCALL);
    engine->RegisterObjectMethod (className.CString (), "VariantMap get_fogOfWarMap ()", asFUNCTION (FogOfWarCalculator_GetFogOfWarMap), asCALL_CDECL_OBJFIRST);
}
}
