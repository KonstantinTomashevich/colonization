#pragma once
#include <Urho3D/AngelScript/Script.h>
namespace Colonization
{
void BindUnit (Urho3D::Script *script);
void BindUnitType (Urho3D::Script *script);
void BindUnitInterface (Urho3D::Script *script, Urho3D::String className);
}
