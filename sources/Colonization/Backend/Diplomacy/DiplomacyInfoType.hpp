#pragma once
#include <Urho3D/Math/StringHash.h>

namespace Colonization
{
const Urho3D::StringHash DIPLOMACY_INFO_WAR_STARTED ("DiplomacyInfoWarStarted");
namespace DiplomacyInfoWarStarted
{
/// Player name hash (StringHash).
const Urho3D::StringHash ATTACKER ("Attacker");
/// Player name hash (StringHash).
const Urho3D::StringHash DEFENDER ("Defender");
}

const Urho3D::StringHash DIPLOMACY_INFO_PEACE_TREATY_ACCEPTED ("DiplomacyInfoPeaceTreatyAccepted");
namespace DiplomacyInfoPeaceTreatyAccepted
{
/// Player name hash (StringHash).
const Urho3D::StringHash SENDER ("Sender");
/// Player name hash (StringHash).
const Urho3D::StringHash ANOTHER_PLAYER ("AnotherPlayer");
}

const Urho3D::StringHash DIPLOMACY_INFO_PEACE_TREATY_DECLINED ("DiplomacyInfoPeaceTreatyDeclined");
namespace DiplomacyInfoPeaceTreatyDeclined
{
/// Player name hash (StringHash).
const Urho3D::StringHash SENDER ("Sender");
/// Player name hash (StringHash).
const Urho3D::StringHash ANOTHER_PLAYER ("AnotherPlayer");
}
}
