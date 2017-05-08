#include <Colonization/BuildConfiguration.hpp>
#include "BindIsObjectTypeOf.hpp"
#include <Urho3D/ThirdParty/AngelScript/angelscript.h>
#include <Urho3D/AngelScript/APITemplates.h>
#include <Urho3D/Core/Object.h>

namespace Colonization
{
bool IsObjectTypeOf (Urho3D::Object *object, Urho3D::StringHash type)
{
    return object->GetTypeInfo ()->IsTypeOf (type);
}

void BindIsObjectTypeOf (Urho3D::Script *script)
{
    asIScriptEngine *engine = script->GetScriptEngine ();
    engine->RegisterGlobalFunction ("bool IsObjectTypeOf (Object @object, StringHash type)", asFUNCTION (IsObjectTypeOf), asCALL_CDECL);
}
}
