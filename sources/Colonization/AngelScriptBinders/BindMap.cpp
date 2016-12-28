#include <Colonization/BuildConfiguration.hpp>
#include "BindMap.hpp"
#include <Urho3D/ThirdParty/AngelScript/angelscript.h>
#include <Urho3D/AngelScript/APITemplates.h>
#include <Colonization/Core/Map.hpp>
// TODO: Maybe delete BindingMacroses?
#include <Colonization/AngelScriptBinders/BindingMacroses.hpp>

namespace Colonization
{
// TODO: Not all functions are binded. Some server-side functions aren't binded.
Urho3D::CScriptArray *Map_FindPath (Map *map, Urho3D::StringHash startDistrictHash, Urho3D::StringHash targetDistrictHash,
                                   Urho3D::String playerName, bool canGoThroughColonies, bool isColonizator)
{
    Urho3D::Vector <Urho3D::StringHash> result = map->FindPath (
                startDistrictHash, targetDistrictHash, playerName, canGoThroughColonies, isColonizator);
    return Urho3D::VectorToArray <Urho3D::StringHash> (result, "Array<StringHash>");
}

void BindMap (Urho3D::Script *script)
{
    asIScriptEngine *engine = script->GetScriptEngine ();
    Urho3D::RegisterComponent <Map> (engine, "Map");
    BindMapInterface (script, "Map");
}

void BindMapInterface (Urho3D::Script *script, Urho3D::String className)
{
    asIScriptEngine *engine = script->GetScriptEngine ();
    engine->RegisterObjectMethod (className.CString (), "District @+ GetDistrictByIndex (int index)", asMETHOD (Map, GetDistrictByIndex), asCALL_THISCALL);
    engine->RegisterObjectMethod (className.CString (), "District @+ GetDistrictByNameHash (StringHash nameHash)", asMETHOD (Map, GetDistrictByNameHash), asCALL_THISCALL);
    engine->RegisterObjectMethod (className.CString (), "District @+ GetDistrictByHash (StringHash hash)", asMETHOD (Map, GetDistrictByHash), asCALL_THISCALL);
    engine->RegisterObjectMethod (className.CString (), "int GetDistrictsCount ()", asMETHOD (Map, GetDistrictsCount), asCALL_THISCALL);

    engine->RegisterObjectMethod (className.CString (), "District @+ CreateDistrict (Urho3D::String districtName)", asMETHOD (Map, CreateDistrict), asCALL_THISCALL);
    engine->RegisterObjectMethod (className.CString (), "void RecalculateDistrictsNeighbors ()", asMETHOD (Map, RecalculateDistrictsNeighbors), asCALL_THISCALL);
    engine->RegisterObjectMethod (className.CString (), "void ClearAndRemoveDistricts ()", asMETHOD (Map, ClearAndRemoveDistricts), asCALL_THISCALL);

    engine->RegisterObjectMethod (className.CString (),
                "Array<StringHash> @FindPath (StringHash startDistrictHash, StringHash targetDistrictHash, String playerName, bool canGoThroughColonies, bool isColonizator)",
                asFUNCTION (Map_FindPath), asCALL_CDECL_OBJFIRST);
}
}
