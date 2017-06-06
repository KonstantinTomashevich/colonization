#include "AngelScript/Utils/ClientUtils.as"

class UnitSelectedWindow : ScriptObject
{
    protected bool isSceneLoaded_;
    protected float untilSelectionUpdate_;
    protected float SELECTION_UPDATE_DELAY = 0.02f;

    protected void UpdateUnitSelection ()
    {
        Node @scriptMain = GetScriptMain (node);
        FogOfWarCalculator @fogOfWarCalculator = scene.GetComponent ("FogOfWarCalculator");
        Window @unitInfoWindow = ui.root.GetChild ("ingame").GetChild ("unitInfoWindow");
        StringHash unitHash = scriptMain.vars ["selectedHash"].GetStringHash ();
        Unit @unit = GetUnitByHash (scene, unitHash);

        if (unit !is null and fogOfWarCalculator.IsDistrictVisible (unit.positionHash))
        {
            unitInfoWindow.visible = true;
            Text @ownerText = unitInfoWindow.GetChild ("ownerText");
            ownerText.text = unit.ownerPlayerName + "'s";

            BorderImage @colorSample = unitInfoWindow.GetChild ("colorSample");
            PlayerInfo @unitOwner = GetPlayerInfoByName (scene, unit.ownerPlayerName);
            if (unitOwner !is null)
            {
                colorSample.color = unitOwner.color;
            }
            else
            {
                colorSample.color = NEUTRAL_COLOR;
            }

            Text @typeText = unitInfoWindow.GetChild ("typeText");
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

            Button @moveToButton = unitInfoWindow.GetChild ("moveToButton");
            if (unit.unitType == UNIT_FLEET or unit.unitType == UNIT_ARMY)
            {
                moveToButton.visible = true;
            }
            else
            {
                moveToButton.visible = false;
            }

            Map @map = scene.GetChild ("map").GetComponent ("Map");
            Text @positionText = unitInfoWindow.GetChild ("positionText");
            positionText.text = "in " + map.GetDistrictByHash (unit.positionHash).name;

            String additionalInfo;
            if (unit.unitType == UNIT_FLEET)
            {
                additionalInfo += "War ships count: " +( cast <FleetUnit> (unit)).warShipsCount + ".\n";
            }

            else if (unit.unitType == UNIT_TRADERS)
            {
                additionalInfo += "Trade goods cost: " + Floor ((cast <TradersUnit> (unit)).tradeGoodsCost * 100.0f) / 100.0f + ".\n";
            }

            else if (unit.unitType == UNIT_COLONIZATORS)
            {
                additionalInfo += "Colonizators count: " + (cast <ColonizatorsUnit> (unit)).colonizatorsCount + ".\n";
            }

            else if (unit.unitType == UNIT_ARMY)
            {
                additionalInfo += "Soldiers count: " + (cast <ArmyUnit> (unit)).soldiersCount + ".\n";
            }

            if (unit.way.length > 0)
            {
                Array <StringHash> unitWay = unit.way;
                additionalInfo += "Going to: " + map.GetDistrictByHash (unitWay [unitWay.length - 1]).name + ".\n";
                additionalInfo += "Next waypoint: " + map.GetDistrictByHash (unitWay [0]).name + "\n";
                additionalInfo += "Traveled to next waypoit: " +
                                    Floor (unit.wayToNextDistrictProgressInPercents) + "%.\n";
            }

            Text @anotherText = unitInfoWindow.GetChild ("anotherText");
            anotherText.text = additionalInfo;
        }
        else
        {
            // Clear selection if unit isn't exists.
            unitInfoWindow.visible = false;
            scriptMain.vars ["selectionType"] = StringHash ("None");
            scriptMain.vars ["selectedHash"] = StringHash ();
        }
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
        Window @unitInfoWindow = ui.root.GetChild ("ingame").GetChild ("unitInfoWindow");
        Button @moveToButton = unitInfoWindow.GetChild ("moveToButton");
        SubscribeToEvent (moveToButton, "Released", "HandleMoveUnitToClick");
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
                Window @unitInfoWindow = ui.root.GetChild ("ingame").GetChild ("unitInfoWindow");
                StringHash selectionType = scriptMain.vars ["selectionType"].GetStringHash ();

                if (selectionType == StringHash ("Unit"))
                {
                    UpdateUnitSelection ();
                }
                else
                {
                    unitInfoWindow.visible = false;
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
}
