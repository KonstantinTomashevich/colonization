#pragma once
#include <Urho3D/Math/StringHash.h>

namespace Colonization
{
//@ASBindGen Constant
const Urho3D::String EVENT_UNIT_CREATED ("UnitCreated");
namespace UnitCreated
{
//@ASBindGen Constant OverrideName=UnitCreated_UNIT_HASH AddNamespaceToName=UnitCreated
const Urho3D::StringHash UNIT_HASH ("UnitHash");
}

//@ASBindGen Constant
const Urho3D::String EVENT_UNIT_POSITION_CHANGED ("UnitPositionChanged");
namespace UnitPositionChanged
{
//@ASBindGen Constant OverrideName=UnitPositionChanged_UNIT_HASH AddNamespaceToName=UnitPositionChanged
const Urho3D::StringHash UNIT_HASH ("UnitHash");
}

//@ASBindGen Constant
const Urho3D::String EVENT_TRADERS_UNIT_LOSSES_GOLD ("TradersUnitLossesGold");
namespace TradersUnitLossesGold
{
//@ASBindGen Constant OverrideName=TradersUnitLossesGold_UNIT_HASH AddNamespaceToName=TradersUnitLossesGold
const Urho3D::StringHash UNIT_HASH ("UnitHash");
//@ASBindGen Constant OverrideName=TradersUnitLossesGold_BATTLE_HASH AddNamespaceToName=TradersUnitLossesGold
const Urho3D::StringHash BATTLE_HASH ("BattleHash");
//@ASBindGen Constant OverrideName=TradersUnitLossesGold_GOLD_AMOUNT AddNamespaceToName=TradersUnitLossesGold
const Urho3D::StringHash GOLD_AMOUNT ("GoldAmount");
}
}
