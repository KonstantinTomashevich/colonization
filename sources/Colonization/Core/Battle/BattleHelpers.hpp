#pragma once
#include <Colonization/Core/Battle/Battle.hpp>

namespace Colonization
{
namespace BattleHelpers
{
//@ASBindGen Function AddNamespaceToCallName=BattleHelpers
bool AddUnitToBattle (Battle *battle, bool isAttacker, Urho3D::StringHash unitHash);
//@ASBindGen Function AddNamespaceToCallName=BattleHelpers
bool RemoveUnitFromBattle (Battle *battle, bool isAttacker, Urho3D::StringHash unitHash);
//@ASBindGen Function AddNamespaceToCallName=BattleHelpers
bool RemoveUnitWithUnkwnownAlignmentFromBattle (Battle *battle, Urho3D::StringHash unitHash);
//@ASBindGen Function AddNamespaceToCallName=BattleHelpers
Urho3D::StringHash GetUnitHashFromBattleByIndex (Battle *battle, bool isAttacker, int index);
//@ASBindGen Function AddNamespaceToCallName=BattleHelpers
unsigned GetUnitsCountInBattle (Battle *battle, bool isAttackers);
//@ASBindGen Function AddNamespaceToCallName=BattleHelpers
Urho3D::PODVector <Urho3D::StringHash> GetUnitsInBattleList (Battle *battle, bool isAttackers);
}
}
