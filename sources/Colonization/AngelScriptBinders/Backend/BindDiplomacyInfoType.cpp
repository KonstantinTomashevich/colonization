#include <Colonization/BuildConfiguration.hpp>
#include "BindDiplomacyInfoType.hpp"
#include <Urho3D/ThirdParty/AngelScript/angelscript.h>
#include <Urho3D/AngelScript/APITemplates.h>
#include <Colonization/Utils/Activities/Activity.hpp>
#include <Colonization/Backend/Diplomacy/DiplomacyInfoType.hpp>

namespace Colonization
{
Urho3D::StringHash get_DIPLOMACY_INFO_WAR_STARTED  ()
{
    return DIPLOMACY_INFO_WAR_STARTED;
}

Urho3D::StringHash get_DiplomacyInfoWarStarted_ATTACKER ()
{
    return DiplomacyInfoWarStarted::ATTACKER;
}

Urho3D::StringHash get_DiplomacyInfoWarStarted_DEFENDER ()
{
    return DiplomacyInfoWarStarted::DEFENDER;
}

Urho3D::StringHash get_DIPLOMACY_INFO_PEACE_TREATY_ACCEPTED  ()
{
    return DIPLOMACY_INFO_PEACE_TREATY_ACCEPTED;
}

Urho3D::StringHash get_DiplomacyInfoPeaceTreatyAccepted_SENDER ()
{
    return DiplomacyInfoPeaceTreatyAccepted::SENDER;
}

Urho3D::StringHash get_DiplomacyInfoPeaceTreatyAccepted_ANOTHER_PLAYER ()
{
    return DiplomacyInfoPeaceTreatyAccepted::ANOTHER_PLAYER;
}

Urho3D::StringHash get_DIPLOMACY_INFO_PEACE_TREATY_DECLINED  ()
{
    return DIPLOMACY_INFO_PEACE_TREATY_DECLINED;
}

Urho3D::StringHash get_DiplomacyInfoPeaceTreatyDeclined_SENDER ()
{
    return DiplomacyInfoPeaceTreatyDeclined::SENDER;
}

Urho3D::StringHash get_DiplomacyInfoPeaceTreatyDeclined_ANOTHER_PLAYER ()
{
    return DiplomacyInfoPeaceTreatyDeclined::ANOTHER_PLAYER;
}

void BindDiplomacyInfoType (Urho3D::Script *script)
{
    asIScriptEngine *engine = script->GetScriptEngine ();
    engine->RegisterGlobalFunction ("StringHash get_DIPLOMACY_INFO_WAR_STARTED ()", asFUNCTION (get_DIPLOMACY_INFO_WAR_STARTED), asCALL_CDECL);
    engine->RegisterGlobalFunction ("StringHash get_DiplomacyInfoWarStarted_ATTACKER ()", asFUNCTION (get_DiplomacyInfoWarStarted_ATTACKER), asCALL_CDECL);
    engine->RegisterGlobalFunction ("StringHash get_DiplomacyInfoWarStarted_DEFENDER ()", asFUNCTION (get_DiplomacyInfoWarStarted_DEFENDER), asCALL_CDECL);

    engine->RegisterGlobalFunction ("StringHash get_DIPLOMACY_INFO_PEACE_TREATY_ACCEPTED ()", asFUNCTION (get_DIPLOMACY_INFO_PEACE_TREATY_ACCEPTED), asCALL_CDECL);
    engine->RegisterGlobalFunction ("StringHash get_DiplomacyInfoPeaceTreatyAccepted_SENDER ()", asFUNCTION (get_DiplomacyInfoPeaceTreatyAccepted_SENDER), asCALL_CDECL);
    engine->RegisterGlobalFunction ("StringHash get_DiplomacyInfoPeaceTreatyAccepted_ANOTHER_PLAYER ()", asFUNCTION (get_DiplomacyInfoPeaceTreatyAccepted_ANOTHER_PLAYER), asCALL_CDECL);

    engine->RegisterGlobalFunction ("StringHash get_DIPLOMACY_INFO_PEACE_TREATY_DECLINED ()", asFUNCTION (get_DIPLOMACY_INFO_PEACE_TREATY_DECLINED), asCALL_CDECL);
    engine->RegisterGlobalFunction ("StringHash get_DiplomacyInfoPeaceTreatyDeclined_SENDER ()", asFUNCTION (get_DiplomacyInfoPeaceTreatyDeclined_SENDER), asCALL_CDECL);
    engine->RegisterGlobalFunction ("StringHash get_DiplomacyInfoPeaceTreatyDeclined_ANOTHER_PLAYER ()", asFUNCTION (get_DiplomacyInfoPeaceTreatyDeclined_ANOTHER_PLAYER), asCALL_CDECL);
}
}
