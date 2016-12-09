#include <Colonization/BuildConfiguration.hpp>
#include "BindAngelScriptDirectMemoryAccess.hpp"
#include <Colonization/AngelScriptBinders/BindingMacroses.hpp>

namespace Colonization
{
void AddRef (Urho3D::Object *object)
{
    object->AddRef ();
}

void ReleaseRef (Urho3D::Object *object)
{
    object->ReleaseRef ();
}

void SetRefs (Urho3D::Object *object, int refs)
{
    while (object->Refs () > refs)
        object->ReleaseRef ();

    while (object->Refs () < refs)
        object->AddRef ();
}

void BindAngelScriptDirectMemoryAccess (Urho3D::Script *script)
{
    asIScriptEngine *engine = script->GetScriptEngine ();
    CHECK_ANGELSCRIPT_RETURN (
                engine->RegisterGlobalFunction ("void AddRef (Object @objecty)",
                                                asFUNCTION (AddRef), asCALL_CDECL)
                );

    CHECK_ANGELSCRIPT_RETURN (
                engine->RegisterGlobalFunction ("void ReleaseRef (Object @objecty)",
                                                asFUNCTION (ReleaseRef), asCALL_CDECL)
                );

    CHECK_ANGELSCRIPT_RETURN (
                engine->RegisterGlobalFunction ("void SetRefs (Object @objecty, int refs)",
                                                asFUNCTION (SetRefs), asCALL_CDECL)
                );
}
}
