#include <Colonization/BuildConfiguration.hpp>
#include "BindDiplomacyOfferType.hpp"
#include <Urho3D/ThirdParty/AngelScript/angelscript.h>
#include <Urho3D/AngelScript/APITemplates.h>
#include <Colonization/Utils/Activities/Activity.hpp>
#include <Colonization/Backend/Diplomacy/DiplomacyOfferType.hpp>

namespace Colonization
{
Urho3D::StringHash get_DIPLOMACY_OFFER_PEACE ()
{
    return DIPLOMACY_OFFER_PEACE;
}

Urho3D::StringHash get_DiplomacyOfferPeace_ENEMY_NAME_HASH ()
{
    return DiplomacyOfferPeace::ENEMY_NAME_HASH;
}

void BindDiplomacyOfferType (Urho3D::Script *script)
{
    asIScriptEngine *engine = script->GetScriptEngine ();
    engine->RegisterGlobalFunction ("StringHash get_DIPLOMACY_OFFER_PEACE ()", asFUNCTION (get_DIPLOMACY_OFFER_PEACE), asCALL_CDECL);
    engine->RegisterGlobalFunction ("StringHash get_DiplomacyOfferPeace_ENEMY_NAME_HASH ()", asFUNCTION (get_DiplomacyOfferPeace_ENEMY_NAME_HASH), asCALL_CDECL);
}
}
