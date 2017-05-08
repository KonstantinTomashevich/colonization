#include <Colonization/BuildConfiguration.hpp>
#include "BindUnitTags.hpp"
#include <Urho3D/ThirdParty/AngelScript/angelscript.h>
#include <Urho3D/AngelScript/APITemplates.h>
#include <Colonization/Core/Unit/UnitTags.hpp>

namespace Colonization
{
Urho3D::String get_TAG_UNIT ()
{
    return TAG_UNIT;
}

Urho3D::String get_TAG_FLEET ()
{
    return TAG_FLEET;
}

Urho3D::String get_TAG_TRADERS ()
{
    return TAG_TRADERS;
}

Urho3D::String get_TAG_COLONIZATORS ()
{
    return TAG_COLONIZATORS;
}

Urho3D::String get_TAG_ARMY ()
{
    return TAG_ARMY;
}

void BindUnitTags (Urho3D::Script *script)
{
    asIScriptEngine *engine = script->GetScriptEngine ();
    engine->RegisterGlobalFunction ("String get_TAG_UNIT ()", asFUNCTION (get_TAG_UNIT), asCALL_CDECL);
    engine->RegisterGlobalFunction ("String get_TAG_FLEET ()", asFUNCTION (get_TAG_FLEET), asCALL_CDECL);
    engine->RegisterGlobalFunction ("String get_TAG_TRADERS ()", asFUNCTION (get_TAG_TRADERS), asCALL_CDECL);
    engine->RegisterGlobalFunction ("String get_TAG_COLONIZATORS ()", asFUNCTION (get_TAG_COLONIZATORS), asCALL_CDECL);
    engine->RegisterGlobalFunction ("String get_TAG_ARMY ()", asFUNCTION (get_TAG_ARMY), asCALL_CDECL);
}
}
