#include "AngelScript/Utils/Constants.as"
#include "AngelScript/Utils/ClientUtils.as"

class UnitSelectedWindow : ScriptObject
{
    protected bool isSceneLoaded_;
    protected float untilSelectionUpdate_;
    protected float SELECTION_UPDATE_DELAY = 0.02f;

    protected void UpdateUnitSelection ()
    {
        Node @scriptMain = GetScriptMain (node);
        Map @map = scene.GetChild ("map").GetComponent ("Map");
        FogOfWarCalculator @fogOfWarCalculator = scene.GetComponent ("FogOfWarCalculator");
        Window @unitSelectedWindow = ui.root.GetChild ("ingame").GetChild ("unitSelectedWindow");
        StringHash unitHash = scriptMain.vars ["selectedHash"].GetStringHash ();
        Unit @unit = GetUnitByHash (scene, unitHash);

        if (unit !is null and fogOfWarCalculator.IsDistrictVisible (unit.positionHash))
        {
            unitSelectedWindow.visible = true;
            UpdateBasicInfos (unitSelectedWindow, unit, map);
            UpdateButtonsVisibility (unitSelectedWindow, unit, map, scriptMain.vars [ScriptMainVars::PLAYER_NAME].GetString ());

            String additionalInfo;
            if (unit.isInBattle)
            {
                additionalInfo += "[At fight, can't move]\n";
            }

            if (unit.unitType == UNIT_FLEET)
            {
                additionalInfo += GenerateFleetInfo (cast <FleetUnit> (unit));
            }

            else if (unit.unitType == UNIT_TRADERS)
            {
                additionalInfo += GenerateTradersInfo (cast <TradersUnit> (unit));
            }

            else if (unit.unitType == UNIT_COLONIZATORS)
            {
                additionalInfo += GenerateColonizatorsInfo (cast <ColonizatorsUnit> (unit));
            }

            else if (unit.unitType == UNIT_ARMY)
            {
                additionalInfo += GenerateArmyInfo (cast <ArmyUnit> (unit));
            }

            additionalInfo += GenerateUnitWayInfo (unit, map);
            Text @anotherText = unitSelectedWindow.GetChild ("anotherText");
            anotherText.text = additionalInfo;
        }
        else
        {
            // Clear selection if unit isn't exists.
            unitSelectedWindow.visible = false;
            scriptMain.vars ["selectionType"] = StringHash ("None");
            scriptMain.vars ["selectedHash"] = StringHash ();
        }
    }

    protected void UpdateBasicInfos (Window @unitSelectedWindow, Unit @unit, Map @map)
    {
        Text @ownerText = unitSelectedWindow.GetChild ("ownerText");
        ownerText.text = unit.ownerPlayerName + "'s";

        BorderImage @colorSample = unitSelectedWindow.GetChild ("colorSample");
        PlayerInfo @unitOwner = GetPlayerInfoByName (scene, unit.ownerPlayerName);
        if (unitOwner !is null)
        {
            colorSample.color = unitOwner.color;
        }
        else
        {
            colorSample.color = NEUTRAL_COLOR;
        }

        Text @typeText = unitSelectedWindow.GetChild ("typeText");
        if (unit.unitType == UNIT_FLEET)
        {
            typeText.text = "Fleet";
        }
        else if (unit.unitType == UNIT_TRADERS)
        {
            typeText.text = "Traders";
        }
        else if (unit.unitType == UNIT_COLONIZATORS)
        {
            typeText.text = "Colonizators";
        }
        else if (unit.unitType == UNIT_ARMY)
        {
            typeText.text = "Army";
        }

        Text @positionText = unitSelectedWindow.GetChild ("positionText");
        positionText.text = "in " + map.GetDistrictByHash (unit.positionHash).name;
    }

    protected void UpdateButtonsVisibility (Window @unitSelectedWindow, Unit @unit, Map @map, String playerName)
    {
        Button @moveToButton = unitSelectedWindow.GetChild ("moveToButton");
        moveToButton.visible = unit.ownerPlayerName == playerName and not unit.isInBattle
                               and (unit.unitType == UNIT_FLEET or unit.unitType == UNIT_ARMY);

        Button @demobilizeArmyButton = unitSelectedWindow.GetChild ("demobilizeArmyButton");
        District @armyDistrict = map.GetDistrictByHash (unit.positionHash);
        demobilizeArmyButton.visible = unit.ownerPlayerName == playerName and unit.unitType == UNIT_ARMY and
                                       not unit.isInBattle and not armyDistrict.isSea and
                                       armyDistrict.hasColony and armyDistrict.colonyOwnerName == playerName;
    }

    protected String GenerateFleetInfo (FleetUnit @unit)
    {
        String info;
        info += "War ships count: " + unit.warShipsCount + ".\n";
        info += "War ships HP:\n   ";

        Array <float> warShipsHealth = unit.warShipsHealthPoints;
        uint indexInRow = 0;
        for (uint index = 0; index < warShipsHealth.length; index++)
        {
            String health = Floor (warShipsHealth [index] * 10.0f) / 10.0f + "%";
            while (health.length < 4)
            {
                health = " " + health;
            }

            info += " " + health;
            indexInRow++;
            if (indexInRow == 4)
            {
                indexInRow = 0;
                info += "\n   ";
            }
        }
        info += "\n";
        return info;
    }

    protected String GenerateTradersInfo (TradersUnit @unit)
    {
        return "Trade goods cost: " + Floor (unit.tradeGoodsCost * 100.0f) / 100.0f + ".\n";
    }

    protected String GenerateColonizatorsInfo (ColonizatorsUnit @unit)
    {
        return "Colonizators count: " + Floor (unit.colonizatorsCount) + ".\n";
    }

    protected String GenerateArmyInfo (ArmyUnit @unit)
    {
        return "Soldiers count: " + Floor (unit.soldiersCount) + ".\n";
    }

    protected String GenerateUnitWayInfo (Unit @unit, Map @map)
    {
        String info;
        if (unit.way.length > 0)
        {
            Array <StringHash> unitWay = unit.way;
            info += "Going to: " + map.GetDistrictByHash (unitWay [unitWay.length - 1]).name + ".\n";
            info += "Next waypoint: " + map.GetDistrictByHash (unitWay [0]).name + "\n";
            info += "Traveled to next waypoit: " +
                    Floor (unit.wayToNextDistrictProgressInPercents) + "%.\n";
        }
        return info;
    }

    UnitSelectedWindow ()
    {
        isSceneLoaded_ = false;
        untilSelectionUpdate_ = 0.0f;
    }

    ~UnitSelectedWindow ()
    {

    }

    void Start ()
    {
        Window @unitSelectedWindow = ui.root.GetChild ("ingame").GetChild ("unitSelectedWindow");
        Button @moveToButton = unitSelectedWindow.GetChild ("moveToButton");
        Button @demobilizeArmyButton = unitSelectedWindow.GetChild ("demobilizeArmyButton");

        SubscribeToEvent (moveToButton, "Released", "HandleMoveUnitToClick");
        SubscribeToEvent (demobilizeArmyButton, "Released", "HandleDemobilizeArmyClick");
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
            untilSelectionUpdate_ -= timeStep;
            if (untilSelectionUpdate_ <= 0.0f)
            {
                Window @unitSelectedWindow = ui.root.GetChild ("ingame").GetChild ("unitSelectedWindow");
                StringHash selectionType = scriptMain.vars ["selectionType"].GetStringHash ();

                if (selectionType == StringHash ("Unit"))
                {
                    UpdateUnitSelection ();
                }
                else
                {
                    unitSelectedWindow.visible = false;
                }
                untilSelectionUpdate_ = SELECTION_UPDATE_DELAY;
            }
        }
    }

    void Stop ()
    {
        UnsubscribeFromAllEvents ();
    }

    void HandleMoveUnitToClick ()
    {
        Node @scriptMain = GetScriptMain (node);
        scriptMain.vars ["currentClickCommand"] = StringHash ("MoveUnit");
    }

    void HandleDemobilizeArmyClick ()
    {
        Node @scriptMain = GetScriptMain (node);
        Map @map = scene.GetChild ("map").GetComponent ("Map");
        FogOfWarCalculator @fogOfWarCalculator = scene.GetComponent ("FogOfWarCalculator");
        Window @unitSelectedWindow = ui.root.GetChild ("ingame").GetChild ("unitSelectedWindow");
        StringHash unitHash = scriptMain.vars ["selectedHash"].GetStringHash ();
        Unit @unit = GetUnitByHash (scene, unitHash);

        if (unit is null)
        {
            return;
        }
        District @armyDistrict = map.GetDistrictByHash (unit.positionHash);
        String playerName = scriptMain.vars [ScriptMainVars::PLAYER_NAME].GetString ();

        if (unit.ownerPlayerName == playerName and unit.unitType == UNIT_ARMY and
            not unit.isInBattle and not armyDistrict.isSea and
            armyDistrict.hasColony and armyDistrict.colonyOwnerName == playerName)
        {
            VectorBuffer buffer = VectorBuffer ();
            buffer.WriteInt (PLAYER_ACTION_DEMOBILIZE_ARMY);
            buffer.WriteStringHash (unitHash);

            VariantMap eventData;
            eventData [NewNetworkTask::TASK_TYPE] = Variant (CTS_NETWORK_MESSAGE_SEND_PLAYER_ACTION);
            eventData [NewNetworkTask::MESSAGE_BUFFER] = Variant (buffer);
            SendEvent (EVENT_NEW_NETWORK_TASK, eventData);
        }
    }
}
