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
        return node.parent.vars ["chatBlockedPlayersList"].GetStringVector ();
    }

    protected void SetElements (Array <String> elements) override
    {
        node.parent.vars ["chatBlockedPlayersList"] = Variant (elements);
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
        String playerName = node.parent.parent.vars ["playerName"].GetString ();
        return (playerName != element);
    }
}
