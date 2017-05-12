#pragma once
#include <Urho3D/AngelScript/Script.h>

namespace Colonization
{
void BindGameConfiguration (Urho3D::Script *script, bool bindInterface = true);
void BindGameConfigurationInterface (Urho3D::Script *script);
}
