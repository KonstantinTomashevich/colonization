#include "AngelScript/Utils/ClientUtils.as"
#include "AngelScript/Utils/StringListEditorUiHandler.as"

class PlayersList : StringListEditorUiHandler
{
    protected Window @GetWindow () override
    {
        return ui.root.GetChild ("waitingForStart").GetChild ("playersListWindow");
    }

    protected Array <String> GetElements () override
    {
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
            if (playerInfo.isReadyForStart)
            {
                result += " [Ready]";
            }
            else
            {
                result += " [Not Ready]";
            }
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

        Button @kickButton = uiElement.GetChild ("removeButton");
        Node @scriptMain = GetScriptMain (node);
        if (scriptMain !is null)
        {
            if (elementsStrings [elementIndex] == scriptMain.vars [ScriptMainVars::PLAYER_NAME].GetString () or
                not scriptMain.vars [ScriptMainVars::IS_ADMIN].GetBool ())
            {
                kickButton.visible = false;
            }
            else
            {
                kickButton.visible = true;
            }
        }
        else
        {
            kickButton.visible = false;
        }
    }

    PlayersList ()
    {

    }

    ~PlayersList ()
    {

    }

    void HandleAddElementClick () override
    {
        // Ignore, elements can't be added by user to this list.
    }

    void HandleRemoveElementClick (StringHash eventType, VariantMap &eventData) override
    {
        Node @scriptMain = GetScriptMain (node);
        if (scriptMain !is null)
        {
            UIElement @element = eventData ["Element"].GetPtr ();
            int elementOffset = element.vars ["ElementOffset"].GetInt ();
            int summaryOffset = elementsShowOffset_ + elementOffset;

            PlayerInfo @playerInfo = GetPlayerInfoByIndex (scene, summaryOffset);
            if (playerInfo !is null and playerInfo.name != scriptMain.vars [ScriptMainVars::PLAYER_NAME].GetString () and
                scriptMain.vars [ScriptMainVars::IS_ADMIN].GetBool ())
            {
                VariantMap kickEventData;
                kickEventData [HostRequestKickPlayer_PLAYER_NAME] = Variant (playerInfo.name);
                SendEvent (EVENT_HOST_REQUEST_KICK_PLAYER, kickEventData);
            }
        }
    }

    void HandleHideClick () override
    {
        // Ignore, this window can't be closed.
    }
}
