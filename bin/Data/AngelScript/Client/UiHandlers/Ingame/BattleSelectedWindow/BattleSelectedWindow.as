#include "AngelScript/Utils/Constants.as"
#include "AngelScript/Utils/ClientUtils.as"

class BattleSelectedWindow : ScriptObject
{
    protected bool isSceneLoaded_;

    BattleSelectedWindow ()
    {

    }

    ~BattleSelectedWindow ()
    {

    }

    void Start ()
    {
        ScriptInstance @attackersUnitsInBattleListInstance = node.CreateChild ("AttackersUnitsInBattleList", LOCAL).CreateComponent ("ScriptInstance");
        attackersUnitsInBattleListInstance.CreateObject (cache.GetResource ("ScriptFile",
                                                      "AngelScript/Client/UiHandlers/Ingame/BattleSelectedWindow/AttackersUnitsInBattleList.as"),
                                         "AttackersUnitsInBattleList");

        ScriptInstance @defendersUnitsInBattleListInstance = node.CreateChild ("DefendersUnitsInBattleList", LOCAL).CreateComponent ("ScriptInstance");
        defendersUnitsInBattleListInstance.CreateObject (cache.GetResource ("ScriptFile",
                                                      "AngelScript/Client/UiHandlers/Ingame/BattleSelectedWindow/DefendersUnitsInBattleList.as"),
                                         "DefendersUnitsInBattleList");
    }

    void Update (float timeStep)
    {
        Node @scriptMain = GetScriptMain (node);
        if (!isSceneLoaded_ and scriptMain.vars ["gameState"].GetInt () != GAME_STATE_WAITING_FOR_START)
        {
            isSceneLoaded_ = CheckIsSceneLoaded (scene);
        }
        else
        {
            Window @battleSelectedWindow = ui.root.GetChild ("ingame").GetChild ("battleSelectedWindow");
            StringHash selectionType = scriptMain.vars ["selectionType"].GetStringHash ();

            if (selectionType == StringHash ("Battle"))
            {
                Text @title = battleSelectedWindow.GetChild ("title");
                Battle @battle = GetBattleByHash (scene, scriptMain.vars ["selectedHash"].GetStringHash ());
                Map @map = scene.GetChild ("map").GetComponent ("Map");

                District @district = map.GetDistrictByHash (battle.districtHash);
                title.text = "Battle at " + district.name;
                battleSelectedWindow.visible = true;
            }
            else
            {
                battleSelectedWindow.visible = false;
            }
        }
    }

    void Stop ()
    {
        UnsubscribeFromAllEvents ();
    }
}
