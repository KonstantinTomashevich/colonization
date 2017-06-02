#pragma once
#include <Urho3D/Math/StringHash.h>

namespace Colonization
{
//@ASBindGen Constant
const Urho3D::StringHash DIPLOMACY_INFO_WAR_STARTED ("DiplomacyInfoWarStarted");
namespace DiplomacyInfoWarStarted
{
//@ASBindGen Constant OverrideName=DiplomacyInfoWarStarted_ATTACKER
/// Player name hash (StringHash).
const Urho3D::StringHash ATTACKER ("Attacker");
//@ASBindGen Constant OverrideName=DiplomacyInfoWarStarted_DEFENDER
/// Player name hash (StringHash).
const Urho3D::StringHash DEFENDER ("Defender");
}

//@ASBindGen Constant
const Urho3D::StringHash DIPLOMACY_INFO_PEACE_TREATY_ACCEPTED ("DiplomacyInfoPeaceTreatyAccepted");
namespace DiplomacyInfoPeaceTreatyAccepted
{
//@ASBindGen Constant OverrideName=DiplomacyInfoPeaceTreatyAccepted_SENDER
/// Player name hash (StringHash).
const Urho3D::StringHash SENDER ("Sender");
//@ASBindGen Constant OverrideName=DiplomacyInfoPeaceTreatyAccepted_ANOTHER_PLAYER
/// Player name hash (StringHash).
const Urho3D::StringHash ANOTHER_PLAYER ("AnotherPlayer");
}

//@ASBindGen Constant
const Urho3D::StringHash DIPLOMACY_INFO_PEACE_TREATY_DECLINED ("DiplomacyInfoPeaceTreatyDeclined");
namespace DiplomacyInfoPeaceTreatyDeclined
{
//@ASBindGen Constant OverrideName=DiplomacyInfoPeaceTreatyDeclined_SENDER
/// Player name hash (StringHash).
const Urho3D::StringHash SENDER ("Sender");
//@ASBindGen Constant OverrideName=DiplomacyInfoPeaceTreatyDeclined_ANOTHER_PLAYER
/// Player name hash (StringHash).
const Urho3D::StringHash ANOTHER_PLAYER ("AnotherPlayer");
}
}
