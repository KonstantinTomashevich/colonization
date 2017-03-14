#pragma once
#include <Urho3D/AngelScript/Script.h>
namespace Colonization
{
void BindGameStateType(Urho3D::Script *script);
void BindReplicatedSceneVarsNames (Urho3D::Script *script);
void BindHostActivityEvents (Urho3D::Script *script);
void BindHostActivity (Urho3D::Script *script);
void BindHostActivityInterface (Urho3D::Script *script, Urho3D::String className);
}
