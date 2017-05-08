#include <Colonization/BuildConfiguration.hpp>
#include "BindColonizatorsUnit.hpp"
#include <Colonization/AngelScriptBinders/Core/Unit/UnitGettersAndSetters.hpp>
#include <Colonization/AngelScriptBinders/Templates/BindUnitInterface.hpp>
#include <Urho3D/ThirdParty/AngelScript/angelscript.h>
#include <Urho3D/AngelScript/APITemplates.h>
#include <Colonization/Core/Unit/ColonizatorsUnit.hpp>

namespace Colonization
{
void BindColonizatorsUnit (Urho3D::Script *script, bool bindInterface)
{
    asIScriptEngine *engine = script->GetScriptEngine ();
    if (!bindInterface)
    {
        Urho3D::RegisterComponent <ColonizatorsUnit> (engine, "ColonizatorsUnit");
    }
    else
    {
        BindUnitInterface <ColonizatorsUnit> (script, "ColonizatorsUnit");
        engine->RegisterObjectMethod ("ColonizatorsUnit", "int get_colonizatorsCount () const", asMETHOD (ColonizatorsUnit, GetColonizatorsCount), asCALL_THISCALL);
        engine->RegisterObjectMethod ("ColonizatorsUnit", "void set_colonizatorsCount (int colonizatorsCount)", asMETHOD (ColonizatorsUnit, SetColonizatorsCount), asCALL_THISCALL);
    }
}
}
