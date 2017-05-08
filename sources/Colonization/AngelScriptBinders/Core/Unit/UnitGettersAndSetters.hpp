#pragma once
#include <Urho3D/AngelScript/Script.h>
#include <Urho3D/AngelScript/APITemplates.h>
#include <Colonization/Core/Unit/Unit.hpp>

namespace Colonization
{
Urho3D::CScriptArray *Unit_GetWay (Unit *unit);
void Unit_SetWay (Unit *unit, Urho3D::CScriptArray *array);
}
