#include "AngelScript/Utils/ClientUtils.as"
#include "AngelScript/Utils/UnitsInBattleList.as"

class DefendersUnitsInBattleList : UnitsInBattleList
{
    DefendersUnitsInBattleList ()
    {
        isAttackers_ = false;
    }

    ~DefendersUnitsInBattleList ()
    {

    }

    protected Window @GetWindow () override
    {
        return ui.root.GetChild ("ingame").GetChild ("battleSelectedWindow").GetChild ("defendersUnitsList");
    }
}
