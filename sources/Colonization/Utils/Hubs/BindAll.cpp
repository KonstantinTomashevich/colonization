#include <Colonization/BuildConfiguration.hpp>
#include "BindAll.hpp"
#include <Colonization/AngelScriptBindings/Bindings.cpp>

namespace Colonization
{
void BindAll (Urho3D::Script *script)
{
    Bindings::RegisterAnything (script->GetScriptEngine ());
}
}
