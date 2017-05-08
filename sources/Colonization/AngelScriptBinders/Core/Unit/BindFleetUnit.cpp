#include <Colonization/BuildConfiguration.hpp>
#include "BindFleetUnit.hpp"
#include <Colonization/AngelScriptBinders/Core/Unit/UnitGettersAndSetters.hpp>
#include <Colonization/AngelScriptBinders/Templates/BindUnitInterface.hpp>
#include <Urho3D/ThirdParty/AngelScript/angelscript.h>
#include <Urho3D/AngelScript/APITemplates.h>
#include <Colonization/Core/Unit/FleetUnit.hpp>

namespace Colonization
{
Urho3D::CScriptArray *FleetUnit_GetWarShipsHealthPoints (FleetUnit *fleetUnit)
{
    return Urho3D::VectorToArray <float> (fleetUnit->GetWarShipsHealthPoints (), "Array<float>");
}

void FleetUnit_SetWarShipsHealthPoints (FleetUnit *fleetUnit, Urho3D::CScriptArray *array)
{
     fleetUnit->SetWarShipsHealthPoints (Urho3D::ArrayToPODVector <float> (array));
}

void BindFleetUnit (Urho3D::Script *script, bool bindInterface)
{
    asIScriptEngine *engine = script->GetScriptEngine ();
    if (!bindInterface)
    {
        Urho3D::RegisterComponent <FleetUnit> (engine, "FleetUnit");
    }
    else
    {
        BindUnitInterface <FleetUnit> (script, "FleetUnit");
        engine->RegisterObjectMethod ("FleetUnit", "int get_warShipsCount () const", asMETHOD (FleetUnit, GetWarShipsCount), asCALL_THISCALL);
        engine->RegisterObjectMethod ("FleetUnit", "void set_warShipsCount (int warShipsCount)", asMETHOD (FleetUnit, SetWarShipsCount), asCALL_THISCALL);

        engine->RegisterObjectMethod ("FleetUnit", "Array <float> @get_warShipsHealthPoints () const", asFUNCTION (FleetUnit_GetWarShipsHealthPoints), asCALL_CDECL_OBJFIRST);
        engine->RegisterObjectMethod ("FleetUnit", "void set_warShipsHealthPoints (Array <float> @warShipsHealthPoints)", asFUNCTION (FleetUnit_SetWarShipsHealthPoints), asCALL_CDECL_OBJFIRST);
    }
}
}
