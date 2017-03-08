#include "AngelScript/Utils/ClientUtils.as"
#include "AngelScript/Utils/PlayersNamesListEditor.as"

class ChatPrivateReceiversListEditor : PlayersNamesListEditor
{
    protected Window @GetWindow () override
    {
        return ui.root.GetChild ("ingame").GetChild ("chatPrivateReceiversWindow");
    }

    ChatPrivateReceiversListEditor ()
    {
        listVarName_ = "chatPrivateReceiversList";
    }

    ~ChatPrivateReceiversListEditor ()
    {

    }
}
