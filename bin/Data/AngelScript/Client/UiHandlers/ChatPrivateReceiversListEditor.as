#include "AngelScript/Utils/ClientUtils.as"
#include "AngelScript/Utils/StringListEditorUiHandler.as"

class ChatPrivateReceiversListEditor : StringListEditorUiHandler
{
    protected Window @GetWindow () override
    {
        return ui.root.GetChild ("ingame").GetChild ("chatPrivateReceiversWindow");
    }

    protected Array <String> GetElements () override
    {
        return node.parent.vars ["chatPrivateReceiversList"].GetStringVector ();
    }

    protected void SetElements (Array <String> elements) override
    {
        node.parent.vars ["chatPrivateReceiversList"] = Variant (elements);
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
