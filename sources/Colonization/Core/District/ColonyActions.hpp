#pragma once
#include <Urho3D/Math/StringHash.h>

namespace Colonization
{
//@ASBindGen Constant
const Urho3D::StringHash COLONY_ACTION_ID ("ColonyActionId");
//@ASBindGen Constant
const Urho3D::StringHash COLONY_ACTION_PROGRESS ("ColonyActionProgress");
namespace ColonyActions
{
//@ASBindGen Constant OverrideName=ColonyActions_BUILD_WAR_SHIP AddNamespaceToName=ColonyActions
const Urho3D::StringHash BUILD_WAR_SHIP ("BuildWarShip");
namespace BuildWarShip
{
//@ASBindGen Constant OverrideName=ColonyActions_BuildWarShip_TARGET_DISTRICT AddNamespaceToName=ColonyActions::BuildWarShip
/// District hash (sea district, neighbor of colony).
const Urho3D::StringHash TARGET_DISTRICT ("TargetDistrict");
}

//@ASBindGen Constant OverrideName=ColonyActions_FORM_ARMY AddNamespaceToName=ColonyActions
const Urho3D::StringHash FORM_ARMY ("FormArmy");
namespace FormArmy
{
//@ASBindGen Constant OverrideName=ColonyActions_FormArmy_SOLDIERS_COUNT AddNamespaceToName=ColonyActions::FormArmy
/// Int.
const Urho3D::StringHash SOLDIERS_COUNT ("SoldiersCount");
}
}
}
