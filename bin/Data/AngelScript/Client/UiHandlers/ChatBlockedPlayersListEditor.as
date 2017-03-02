#include "AngelScript/Utils/ClientUtils.as"
#include "AngelScript/Utils/StringListEditorUiHandler.as"

class ChatBlockedPlayersListEditor : StringListEditorUiHandler
{
    protected Window @GetWindow () override
    {
        return ui.root.GetChild ("ingame").GetChild ("chatBlockedPlayersWindow");
    }

    protected Array <String> GetElements () override
    {
        Node @scriptMain = GetScriptMain (node);
        return scriptMain.vars ["chatBlockedPlayersList"].GetStringVector ();
    }

    protected void SetElements (Array <String> elements) override
    {
        Node @scriptMain = GetScriptMain (node);
        scriptMain.vars ["chatBlockedPlayersList"] = Variant (elements);
    }

    protected String ProcessElementText (String text) override
    {
        PlayerInfo @playerInfo = GetPlayerInfoByName (scene, text);
        if (playerInfo !is null)
        {
            return (text + " [Found]");
        }
        else
        {
            return (text + " [Not found]");
        }
    }

    protected bool IsElementToAddCorrect (String element) override
    {
        Node @scriptMain = GetScriptMain (node);
        String playerName = scriptMain.vars ["playerName"].GetString ();
        return (playerName != element);
    }
}
