#include "AngelScript/Utils/ClientUtils.as"
#include "AngelScript/Utils/UnitsInBattleList.as"

class AttackersUnitsInBattleList : UnitsInBattleList
{
    AttackersUnitsInBattleList ()
    {
        isAttackers_ = true;
    }

    ~AttackersUnitsInBattleList ()
    {

    }

    protected Window @GetWindow () override
    {
        return ui.root.GetChild ("ingame").GetChild ("battleSelectedWindow").GetChild ("attackersUnitsList");
    }
}
