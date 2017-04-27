#include "AngelScript/Utils/ClientUtils.as"
#include "AngelScript/Utils/StringListEditorUiHandler.as"

class InfoTablePlayersList : StringListEditorUiHandler
{
    protected String currentPlayerName_;

    protected Window @GetWindow () override
    {
        return ui.root.GetChild ("ingame").GetChild ("infoTableWindow").GetChild ("playersList");
    }

    protected Array <String> GetElements () override
    {
        Node @scriptMain = GetScriptMain (scene);
        currentPlayerName_ = scriptMain.vars ["playerName"].GetString ();
        return GetPlayersNamesList (scene);
    }

    protected void SetElements (Array <String> elements) override
    {
        // Ignore, elements can't be added by user to this list.
    }

    protected String ProcessElementText (String text, int elementIndex) override
    {
        PlayerInfo @playerInfo = GetPlayerInfoByIndex (scene, elementIndex);
        if (playerInfo !is null)
        {
            String result = "" + (elementIndex + 1) + ". " + text;

            if (playerInfo.IsAtWarWith (StringHash (currentPlayerName_)))
            {
                result += "  [At war with you]";
            }
            else if (playerInfo.name == currentPlayerName_)
            {
                result += "  [You]";
            }
            else
            {
                result += "  [At peace with you]";
            }

            result += "  Points: " + playerInfo.points;
            return result;
        }
        else
        {
            return "---error---";
        }
    }

    protected bool IsElementToAddCorrect (String element) override
    {
        // Ignore, elements can't be added by user to this list.
        return false;
    }

    protected void ProcessElementUi (UIElement @uiElement, int elementIndex, Array <String> &in elementsStrings) override
    {
        StringListEditorUiHandler::ProcessElementUi (uiElement, elementIndex, elementsStrings);
        PlayerInfo @playerInfo = GetPlayerInfoByIndex (scene, elementIndex);
        if (playerInfo !is null)
        {
            BorderImage @colorSample = uiElement.GetChild ("colorSample");
            colorSample.color = playerInfo.color;
        }
    }

    InfoTablePlayersList ()
    {

    }

    ~InfoTablePlayersList ()
    {

    }

    void HandleAddElementClick () override
    {
        // Ignore, elements can't be added by user to this list.
    }

    void HandleRemoveElementClick (StringHash eventType, VariantMap &eventData) override
    {
        // TODO: Implement after diplomacy ui implementation.
    }

    void HandleHideClick () override
    {
        // Ignore, this window can't be closed.
    }
}
