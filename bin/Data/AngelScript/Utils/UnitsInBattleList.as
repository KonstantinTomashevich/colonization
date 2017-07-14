#include "AngelScript/Utils/ClientUtils.as"
#include "AngelScript/Utils/UnitsList.as"

abstract class UnitsInBattleList : UnitsList
{
    bool isAttackers_ = true;

    protected Array <StringHash> GetUnitsHashes () override
    {
        Node @scriptMain = GetScriptMain (node);
        if (scriptMain.vars ["selectionType"].GetStringHash () == StringHash ("Battle"))
        {
            Battle @battle = GetBattleByHash (scene, scriptMain.vars ["selectedHash"].GetStringHash ());
            return GetUnitsInBattleList (battle, isAttackers_);
        }
        else
        {
            return Array <StringHash> ();
        }
    }

    UnitsInBattleList ()
    {

    }

    ~UnitsInBattleList ()
    {

    }
}
