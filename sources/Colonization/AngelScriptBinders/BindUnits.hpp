#pragma once
#include <Urho3D/AngelScript/Script.h>
namespace Colonization
{
void BindUnits (Urho3D::Script *script);
void BindUnitType (Urho3D::Script *script);
void BindUnit (Urho3D::Script *script);
void BindUnitInterface (Urho3D::Script *script, Urho3D::String className);
void BindFleetUnit (Urho3D::Script *script);
void BindTradersUnit (Urho3D::Script *script);
void BindColonizatorsUnit (Urho3D::Script *script);
void BindArmyUnit (Urho3D::Script *script);
}
