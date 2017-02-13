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
        return text;
    }

    protected bool IsElementToAddCorrect (String element) override
    {
        return true;
    }
}
