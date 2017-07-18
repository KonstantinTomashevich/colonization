#include <Colonization/BuildConfiguration.hpp>
#include "BattleHelpers.hpp"

namespace Colonization
{
namespace BattleHelpers
{
bool AddUnitToBattle (Battle *battle, bool isAttacker, Urho3D::StringHash unitHash)
{
    if (isAttacker)
    {
        return battle->AddAttackerUnitHash (unitHash);
    }
    else
    {
        return battle->AddDefenderUnitHash (unitHash);
    }
}

bool RemoveUnitFromBattle (Battle *battle, bool isAttacker, Urho3D::StringHash unitHash)
{
    if (isAttacker)
    {
        return battle->RemoveAttackerUnitHash (unitHash);
    }
    else
    {
        return battle->RemoveDefenderUnitHash (unitHash);
    }
}

bool RemoveUnitWithUnkwnownAlignmentFromBattle (Battle *battle, Urho3D::StringHash unitHash)
{
    return battle->RemoveAttackerUnitHash (unitHash) || battle->RemoveDefenderUnitHash (unitHash);
}

Urho3D::StringHash GetUnitHashFromBattleByIndex (Battle *battle, bool isAttacker, int index)
{
    if (isAttacker)
    {
        return battle->GetAttackerUnitHashByIndex (index);
    }
    else
    {
        return battle->GetDefenderUnitHashByIndex (index);
    }
}

unsigned GetUnitsCountInBattle (Battle *battle, bool isAttackers)
{
    if (isAttackers)
    {
        return battle->GetAttackersUnitsCount ();
    }
    else
    {
        return battle->GetDefendersUnitsCount ();
    }
}

Urho3D::PODVector <Urho3D::StringHash> GetUnitsInBattleList (Battle *battle, bool isAttackers)
{
    if (isAttackers)
    {
        return battle->GetAttackersUnitsList ();
    }
    else
    {
        return battle->GetDefendersUnitsList ();
    }
}
}
}
