#pragma once
#include <Urho3D/AngelScript/Script.h>

namespace Colonization
{
void BindGameConfiguration (Urho3D::Script *script);
void BindGameConfigurationInterface (Urho3D::Script *script, Urho3D::String className);
}
