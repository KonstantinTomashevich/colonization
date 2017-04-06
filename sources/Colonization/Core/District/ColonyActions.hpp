#pragma once
#include <Urho3D/Math/StringHash.h>

namespace Colonization
{
const Urho3D::StringHash COLONY_ACTION_ID ("ColonyActionId");
const Urho3D::StringHash COLONY_ACTION_PROGRESS ("ColonyActionProgress");
namespace ColonyActions
{
const Urho3D::StringHash BUILD_WAR_SHIP ("BuildWarShip");
namespace BuildWarShip
{
/// District hash (sea district, neighbor of colony).
const Urho3D::StringHash TARGET_DISTRICT ("TargetDistrict");
}
}
}
