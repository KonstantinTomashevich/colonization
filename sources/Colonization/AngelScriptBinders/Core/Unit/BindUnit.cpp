#include <Colonization/BuildConfiguration.hpp>
#include "BindUnit.hpp"
#include <Colonization/AngelScriptBinders/Core/Unit/UnitGettersAndSetters.hpp>
#include <Colonization/AngelScriptBinders/Core/Unit/BindUnitType.hpp>
#include <Colonization/AngelScriptBinders/Core/Unit/BindUnitTags.hpp>
#include <Colonization/AngelScriptBinders/Templates/BindUnitInterface.hpp>
#include <Urho3D/ThirdParty/AngelScript/angelscript.h>
#include <Urho3D/AngelScript/APITemplates.h>
#include <Colonization/Core/Unit/Unit.hpp>

namespace Colonization
{
void BindUnit (Urho3D::Script *script, bool bindInterface)
{
    asIScriptEngine *engine = script->GetScriptEngine ();
    if (!bindInterface)
    {
        BindUnitType (script);
        BindUnitTags (script);
        Urho3D::RegisterComponent <Unit> (engine, "Unit");
    }
    else
    {
        BindUnitInterface <Unit> (script, "Unit");
    }
}
}
