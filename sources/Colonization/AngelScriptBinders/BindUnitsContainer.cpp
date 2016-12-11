#include <Colonization/BuildConfiguration.hpp>
#include "BindUnitsContainer.hpp"
#include <Urho3D/ThirdParty/AngelScript/angelscript.h>
#include <Urho3D/AngelScript/APITemplates.h>
#include <Colonization/Core/UnitsContainer.hpp>
#include <Colonization/AngelScriptBinders/BindingMacroses.hpp>

namespace Colonization
{
Urho3D::CScriptArray *UnitsContainerGetUnitsOfPlayer (UnitsContainer *unitsContainer, Urho3D::StringHash playerNameHash)
{
    Urho3D::PODVector <Unit *> result = unitsContainer->GetUnitsOfPlayer (playerNameHash);
    // Add refs for AngelScript.
    for (int index = 0; index < result.Size (); index++)
        result.At (index)->AddRef ();
    return Urho3D::VectorToArray <Unit *> (result, "Array<Unit@>");
}

void BindUnitsContainer (Urho3D::Script *script)
{
    asIScriptEngine *engine = script->GetScriptEngine ();
    Urho3D::RegisterObject <UnitsContainer> (engine, "UnitsContainer");
    Urho3D::RegisterObjectConstructor <UnitsContainer> (engine, "UnitsContainer");

    CHECK_ANGELSCRIPT_RETURN (
                engine->RegisterObjectMethod (
                    "UnitsContainer", "void UpdateDataNode (Node @dataNode)",
                    asMETHOD (UnitsContainer, UpdateDataNode), asCALL_THISCALL)
                );

    CHECK_ANGELSCRIPT_RETURN (
                engine->RegisterObjectMethod (
                    "UnitsContainer", "void ReadDataFromNode (Node @dataNode, Map @map)",
                    asMETHOD (UnitsContainer, ReadDataFromNode), asCALL_THISCALL)
                );

    CHECK_ANGELSCRIPT_RETURN (
                engine->RegisterObjectMethod (
                    "UnitsContainer", "int GetUnitsCount ()",
                    asMETHOD (UnitsContainer, GetUnitsCount), asCALL_THISCALL)
                );

    CHECK_ANGELSCRIPT_RETURN (
                engine->RegisterObjectMethod (
                    "UnitsContainer", "Unit @GetUnitByIndex (int index)",
                    asMETHOD (UnitsContainer, GetUnitByIndex), asCALL_THISCALL)
                );

    CHECK_ANGELSCRIPT_RETURN (
                engine->RegisterObjectMethod (
                    "UnitsContainer", "Array<Unit@> @GetUnitsOfPlayer (StringHash playerNameHash)",
                    asFUNCTION (UnitsContainerGetUnitsOfPlayer), asCALL_CDECL_OBJFIRST)
                );

    CHECK_ANGELSCRIPT_RETURN (
                engine->RegisterObjectMethod (
                    "UnitsContainer", "void AddUnit (Unit @unit)",
                    asMETHOD (UnitsContainer, AddUnit), asCALL_THISCALL)
                );

    CHECK_ANGELSCRIPT_RETURN (
                engine->RegisterObjectMethod (
                    "UnitsContainer", "bool RemoveAndDeleteUnit (Unit @unit)",
                    asMETHOD (UnitsContainer, RemoveAndDeleteUnit), asCALL_THISCALL)
                );

    CHECK_ANGELSCRIPT_RETURN (
                engine->RegisterObjectMethod (
                    "UnitsContainer", "void RemoveAndDeleteAllUnits ()",
                    asMETHOD (UnitsContainer, RemoveAndDeleteAllUnits), asCALL_THISCALL)
                );
}
}
