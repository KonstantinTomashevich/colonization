#include <Colonization/BuildConfiguration.hpp>
#include "BindGetUnitsInDistrict.hpp"
#include <Urho3D/AngelScript/APITemplates.h>
#include <Urho3D/ThirdParty/AngelScript/angelscript.h>
#include <Colonization/Backend/UnitsManager.hpp>

namespace Colonization
{
Urho3D::CScriptArray *script_GetUnitsInDistrict (Urho3D::Scene *scene, Urho3D::StringHash districtHash)
{
    Urho3D::PODVector <Unit *> units = GetUnitsInDistrict (scene, districtHash);
    return Urho3D::VectorToHandleArray <Unit> (units, "Array <Unit @>");
}

void BindGetUnitsInDistrict (Urho3D::Script *script)
{
    asIScriptEngine *engine = script->GetScriptEngine ();
    engine->RegisterGlobalFunction ("Array <Unit @> @GetUnitsInDistrict (Scene @scene, StringHash districtHash)", asFUNCTION (script_GetUnitsInDistrict), asCALL_CDECL);
}
}
