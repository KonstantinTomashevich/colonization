#include "AngelScript/Utils/ClientUtils.as"
#include "AngelScript/Utils/UnitsInBattleList.as"

class AttackersUnitsInBattleList : UnitsInBattleList
{
    protected Window @GetWindow () override
    {
        return ui.root.GetChild ("ingame").GetChild ("battleSelectedWindow").GetChild ("attackersUnitsList");
    }

    AttackersUnitsInBattleList ()
    {
        isAttackers_ = true;
    }

    ~AttackersUnitsInBattleList ()
    {

    }
}
