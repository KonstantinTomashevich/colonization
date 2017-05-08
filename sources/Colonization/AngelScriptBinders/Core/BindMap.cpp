#include <Colonization/BuildConfiguration.hpp>
#include "BindMap.hpp"
#include <Urho3D/ThirdParty/AngelScript/angelscript.h>
#include <Urho3D/AngelScript/APITemplates.h>
#include <Colonization/Core/Map.hpp>

namespace Colonization
{
Urho3D::CScriptArray *Map_FindPath (Map *map, Urho3D::StringHash targetDistrictHash,
                                   Unit *unit, bool setUnitWay)
{
    Urho3D::PODVector <Urho3D::StringHash> result = map->FindPath (
                targetDistrictHash, unit, setUnitWay);
    return Urho3D::VectorToArray <Urho3D::StringHash> (result, "Array<StringHash>");
}

Urho3D::CScriptArray *Map_GetColoniesOfPlayer (Map *map, Urho3D::StringHash playerNameHash)
{
    Urho3D::PODVector <Urho3D::StringHash> colonies = map->GetColoniesOfPlayer (playerNameHash);
    return Urho3D::VectorToArray <Urho3D::StringHash> (colonies, "Array<StringHash>");
}

Urho3D::String get_TAG_DISTRICT ()
{
    return TAG_DISTRICT;
}

void BindMap (Urho3D::Script *script)
{
    asIScriptEngine *engine = script->GetScriptEngine ();
    engine->RegisterGlobalFunction ("String get_TAG_DISTRICT ()", asFUNCTION (get_TAG_DISTRICT), asCALL_CDECL);
    Urho3D::RegisterComponent <Map> (engine, "Map");

    engine->RegisterObjectMethod ("Map", "District @+ GetDistrictByIndex (uint index) const", asMETHOD (Map, GetDistrictByIndex), asCALL_THISCALL);
    engine->RegisterObjectMethod ("Map", "District @+ GetDistrictByNameHash (StringHash nameHash) const", asMETHOD (Map, GetDistrictByNameHash), asCALL_THISCALL);
    engine->RegisterObjectMethod ("Map", "District @+ GetDistrictByHash (StringHash hash) const", asMETHOD (Map, GetDistrictByHash), asCALL_THISCALL);
    engine->RegisterObjectMethod ("Map", "uint GetDistrictsCount ()", asMETHOD (Map, GetDistrictsCount), asCALL_THISCALL);

    engine->RegisterObjectMethod ("Map", "District @+ CreateDistrict (String districtName)", asMETHOD (Map, CreateDistrict), asCALL_THISCALL);
    engine->RegisterObjectMethod ("Map", "void RecalculateDistrictsNeighbors ()", asMETHOD (Map, RecalculateDistrictsNeighbors), asCALL_THISCALL);
    engine->RegisterObjectMethod ("Map", "void RecalculateDistrictsHashes ()", asMETHOD (Map, RecalculateDistrictsHashes), asCALL_THISCALL);
    engine->RegisterObjectMethod ("Map", "void ClearAndRemoveDistricts ()", asMETHOD (Map, ClearAndRemoveDistricts), asCALL_THISCALL);

    engine->RegisterObjectMethod ("Map",
                "Array<StringHash> @FindPath (StringHash targetDistrictHash, Unit @unit, bool setUnitWay) const",
                asFUNCTION (Map_FindPath), asCALL_CDECL_OBJFIRST);
    engine->RegisterObjectMethod ("Map", "Array<StringHash> @GetColoniesOfPlayer (StringHash playerNameHash)", asFUNCTION (Map_GetColoniesOfPlayer), asCALL_CDECL_OBJFIRST);
}
}
