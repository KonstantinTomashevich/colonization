#include <Colonization/BuildConfiguration.hpp>
#include "UnitGettersAndSetters.hpp"
#include <Urho3D/ThirdParty/AngelScript/angelscript.h>

namespace Colonization
{
Urho3D::CScriptArray *Unit_GetWay (Unit *unit)
{
    return Urho3D::VectorToArray <Urho3D::StringHash> (unit->GetWay (), "Array<StringHash>");
}

void Unit_SetWay (Unit *unit, Urho3D::CScriptArray *array)
{
    unit->SetWay (Urho3D::ArrayToPODVector <Urho3D::StringHash> (array));
}
}
