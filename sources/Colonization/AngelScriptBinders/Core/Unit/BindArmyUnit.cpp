#include <Colonization/BuildConfiguration.hpp>
#include "BindArmyUnit.hpp"
#include <Colonization/AngelScriptBinders/Core/Unit/UnitGettersAndSetters.hpp>
#include <Colonization/AngelScriptBinders/Templates/BindUnitInterface.hpp>
#include <Urho3D/ThirdParty/AngelScript/angelscript.h>
#include <Urho3D/AngelScript/APITemplates.h>
#include <Colonization/Core/Unit/ArmyUnit.hpp>

namespace Colonization
{
void BindArmyUnit(Urho3D::Script *script, bool bindInterface)
{
    asIScriptEngine *engine = script->GetScriptEngine ();
    if (!bindInterface)
    {
        Urho3D::RegisterComponent <ArmyUnit> (engine, "ArmyUnit");
    }
    else
    {
        BindUnitInterface <ArmyUnit> (script, "ArmyUnit");
        engine->RegisterObjectMethod ("ArmyUnit", "int get_soldiersCount () const", asMETHOD (ArmyUnit, GetSoldiersCount), asCALL_THISCALL);
        engine->RegisterObjectMethod ("ArmyUnit", "void set_soldiersCount (int soldiersCount)", asMETHOD (ArmyUnit, SetSoldiersCount), asCALL_THISCALL);
    }
}
}
