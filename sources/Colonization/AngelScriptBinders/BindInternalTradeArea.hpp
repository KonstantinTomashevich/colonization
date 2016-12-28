#pragma once
#include <Urho3D/AngelScript/Script.h>
namespace Colonization
{
void BindInternalTradeArea (Urho3D::Script *script);
void BindTradeDistrictsProcessingInfo (Urho3D::Script *script);
void BindInternalTradeAreaInterface (Urho3D::Script *script, Urho3D::String className);
}
