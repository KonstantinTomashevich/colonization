#include "AngelScript/Utils/ClientUtils.as"
#include "AngelScript/Utils/PlayersNamesListEditor.as"

class ChatBlockedPlayersListEditor : PlayersNamesListEditor
{
    protected Window @GetWindow () override
    {
        return ui.root.GetChild ("ingame").GetChild ("chatBlockedPlayersWindow");
    }

    ChatBlockedPlayersListEditor ()
    {
        listVarName_ = "chatBlockedPlayersList";
    }

    ~ChatBlockedPlayersListEditor ()
    {

    }
}
