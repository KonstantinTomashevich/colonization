#include <Colonization/BuildConfiguration.hpp>
#include "BindDiplomacyTags.hpp"
#include <Urho3D/ThirdParty/AngelScript/angelscript.h>
#include <Urho3D/AngelScript/APITemplates.h>
#include <Colonization/Core/Diplomacy/DiplomacyTags.hpp>

namespace Colonization
{
Urho3D::String get_TAG_DIPLOMACY_RELATION ()
{
    return TAG_DIPLOMACY_RELATION;
}

Urho3D::String get_TAG_WAR ()
{
    return TAG_WAR;
}

Urho3D::String get_TAG_PEACE ()
{
    return TAG_PEACE;
}

Urho3D::String get_TAG_DIPLOMACY_REQUEST ()
{
    return TAG_DIPLOMACY_REQUEST;
}

void BindDiplomacyTags (Urho3D::Script *script)
{
    asIScriptEngine *engine = script->GetScriptEngine ();
    engine->RegisterGlobalFunction ("String get_TAG_DIPLOMACY_RELATION ()", asFUNCTION (get_TAG_DIPLOMACY_RELATION), asCALL_CDECL);
    engine->RegisterGlobalFunction ("String get_TAG_DIPLOMACY_REQUEST ()", asFUNCTION (get_TAG_DIPLOMACY_REQUEST), asCALL_CDECL);
    engine->RegisterGlobalFunction ("String get_TAG_WAR ()", asFUNCTION (get_TAG_WAR), asCALL_CDECL);
    engine->RegisterGlobalFunction ("String get_TAG_PEACE ()", asFUNCTION (get_TAG_PEACE), asCALL_CDECL);
}
}
