#include "AngelScript/Utils/ClientUtils.as"
#include "AngelScript/Utils/PlayersNamesListEditor.as"

class ChatPrivateReceiversListEditor : PlayersNamesListEditor
{
    ChatPrivateReceiversListEditor ()
    {
        listVarName_ = "chatPrivateReceiversList";
    }

    ~ChatPrivateReceiversListEditor ()
    {

    }

    protected Window @GetWindow () override
    {
        return ui.root.GetChild ("ingame").GetChild ("chatPrivateReceiversWindow");
    }
}
