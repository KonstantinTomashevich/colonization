#include <Colonization/BuildConfiguration.hpp>
#include "BindMap.hpp"
#include <Urho3D/ThirdParty/AngelScript/angelscript.h>
#include <Urho3D/AngelScript/APITemplates.h>
#include <Colonization/Core/Map.hpp>
#include <Colonization/AngelScriptBinders/BindingMacroses.hpp>

namespace Colonization
{
Urho3D::CScriptArray *MapFindPath (Map *map, District *from, District *to,
                                   Urho3D::String playerName, bool canGoThroughColonies)
{
    Urho3D::PODVector <District *> result = map->FindPath (from, to, playerName, canGoThroughColonies);
    // Add refs for AngelScript.
    for (int index = 0; index < result.Size (); index++)
        result.At (index)->AddRef ();
    return Urho3D::VectorToArray <District *> (result, "Array<District@>");
}

void BindMap (Urho3D::Script *script)
{
    asIScriptEngine *engine = script->GetScriptEngine ();
    Urho3D::RegisterObject <Map> (engine, "Map");
    Urho3D::RegisterObjectConstructor <Map> (engine, "Map");

    CHECK_ANGELSCRIPT_RETURN (
                engine->RegisterObjectMethod (
                    "Map", "void UpdateDataNode (Node @dataNode, bool rewritePolygonPoints = false)",
                    asMETHOD (Map, UpdateDataNode), asCALL_THISCALL)
                );

    CHECK_ANGELSCRIPT_RETURN (
                engine->RegisterObjectMethod (
                    "Map", "void ReadDataFromNode (Node @dataNode)",
                    asMETHOD (Map, ReadDataFromNode), asCALL_THISCALL)
                );

    CHECK_ANGELSCRIPT_RETURN (
                engine->RegisterObjectMethod (
                    "Map", "District @GetDistrictByIndex (int index)",
                    asMETHOD (Map, GetDistrictByIndex), asCALL_THISCALL)
                );

    CHECK_ANGELSCRIPT_RETURN (
                engine->RegisterObjectMethod (
                    "Map", "District @GetDistrictByNameHash (StringHash nameHash)",
                    asMETHOD (Map, GetDistrictByNameHash), asCALL_THISCALL)
                );

    CHECK_ANGELSCRIPT_RETURN (
                engine->RegisterObjectMethod (
                    "Map", "District @GetDistrictByHash (StringHash hash)",
                    asMETHOD (Map, GetDistrictByHash), asCALL_THISCALL)
                );

    CHECK_ANGELSCRIPT_RETURN (
                engine->RegisterObjectMethod (
                    "Map", "int GetDistrictsCount ()",
                    asMETHOD (Map, GetDistrictsCount), asCALL_THISCALL)
                );

    CHECK_ANGELSCRIPT_RETURN (
                engine->RegisterObjectMethod (
                    "Map", "void AddDistrict (District @district)",
                    asMETHOD (Map, AddDistrict), asCALL_THISCALL)
                );

    CHECK_ANGELSCRIPT_RETURN (
                engine->RegisterObjectMethod (
                    "Map", "void UpdateNeighborsOfDistricts ()",
                    asMETHOD (Map, UpdateNeighborsOfDistricts), asCALL_THISCALL)
                );

    CHECK_ANGELSCRIPT_RETURN (
                engine->RegisterObjectMethod (
                    "Map", "void ClearDistricts ()",
                    asMETHOD (Map, ClearDistricts), asCALL_THISCALL)
                );

    CHECK_ANGELSCRIPT_RETURN (
                engine->RegisterObjectMethod (
                    "Map",
                    "Array<District@> @FindPath (District @from, District @to, String playerName, bool canGoThroughColonies)",
                    asFUNCTION (MapFindPath), asCALL_CDECL_OBJFIRST)
                );
}
}
