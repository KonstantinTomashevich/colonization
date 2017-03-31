#pragma once
#include <Urho3D/AngelScript/Script.h>
namespace Colonization
{
void BindDistrict (Urho3D::Script *script);
void BindColonyActions (Urho3D::Script *script);
void BindDistrictEnums (Urho3D::Script *script);
void BindDistrictInterface (Urho3D::Script *script, Urho3D::String className);
}
