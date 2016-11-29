#include <Colonization/BuildConfiguration.hpp>
#include "BindDistrict.hpp"
#include <Urho3D/ThirdParty/AngelScript/angelscript.h>
#include <Urho3D/AngelScript/APITemplates.h>
#include <Colonization/Core/Map.hpp>
#include <Colonization/AngelScriptBinders/BindingMacroses.hpp>

namespace Colonization
{
void BindMap(Urho3D::Script *script)
{
    asIScriptEngine *engine = script->GetScriptEngine ();
    Urho3D::RegisterObject <Map> (engine, "Map");

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
}
}
