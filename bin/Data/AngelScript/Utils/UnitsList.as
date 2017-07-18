#include "AngelScript/Utils/Constants.as"
#include "AngelScript/Utils/ClientUtils.as"
#include "AngelScript/Utils/StringListEditorUiHandler.as"

abstract class UnitsList : StringListEditorUiHandler
{
    UnitsList ()
    {

    }

    ~UnitsList ()
    {

    }

    void HandleAddElementClick () override
    {
        // Ignore, elements can't be added by user to this list.
    }

    // Select this unit on click
    void HandleRemoveElementClick (StringHash eventType, VariantMap &eventData) override
    {
        Node @scriptMain = GetScriptMain (node);
        UIElement @element = eventData ["Element"].GetPtr ();
        int elementOffset = element.vars ["ElementOffset"].GetInt ();
        int summaryOffset = elementsShowOffset_ + elementOffset;

        Array <String> elements = GetElements ();
        StringHash unitHash = StringHash (elements [summaryOffset].Split (';') [1].ToUInt ());
        scriptMain.vars ["selectionType"] = StringHash ("Unit");
        scriptMain.vars ["selectedHash"] = unitHash;
    }

    void HandleHideClick () override
    {
        // Ignore, this window can't be closed.
    }

    protected Array <StringHash> GetUnitsHashes ()
    {
        //! Will be implemented in inheritors!
        return Array <StringHash> ();
    }

    protected Array <String> GetElements () override
    {
        Array <StringHash> unitsHashes = GetUnitsHashes ();
        Array <String> elements;

        for (uint index = 0; index < unitsHashes.length; index++)
        {
            Unit @unit = GetUnitByHash (scene, unitsHashes [index]);
            String info = unit.ownerPlayerName + ";" + unit.hash.value + ";";

            if (unit.unitType == UNIT_FLEET)
            {
                info += "Fleet (" + (cast <FleetUnit> (unit)).warShipsCount + ")";
            }
            else if (unit.unitType == UNIT_TRADERS)
            {
                info += "Traders (" + Floor ((cast <TradersUnit> (unit)).tradeGoodsCost * 100.0f) / 100.0f + ")";
            }
            else if (unit.unitType == UNIT_COLONIZATORS)
            {
                info += "Colonizators (" + Floor ((cast <ColonizatorsUnit> (unit)).colonizatorsCount) + ")";
            }
            else if (unit.unitType == UNIT_ARMY)
            {
                info += "Army (" + Floor ((cast <ArmyUnit> (unit)).soldiersCount) + ")";
            }

            elements.Push (info);
        }
        return elements;
    }

    protected void SetElements (Array <String> elements) override
    {
        // Ignore, elements can't be added by user to this list.
    }

    protected String ProcessElementText (String text, int elementIndex) override
    {
        return text.Split (';') [2];
    }

    protected bool IsElementToAddCorrect (String element) override
    {
        // Ignore, elements can't be added by user to this list.
        return false;
    }

    protected void ProcessElementUi (UIElement @uiElement, int elementIndex, Array <String> &in elementsStrings) override
    {
        StringListEditorUiHandler::ProcessElementUi (uiElement, elementIndex, elementsStrings);
        PlayerInfo @playerInfo = GetPlayerInfoByNameHash (scene, StringHash (elementsStrings [elementIndex].Split (';') [0]));
        BorderImage @colorSample = uiElement.GetChild ("colorSample");

        if (playerInfo !is null)
        {
            colorSample.color = playerInfo.color;
        }
        else
        {
            colorSample.color = Color (0.5f, 0.5f, 0.5f, 1.0f);
        }
    }
}
