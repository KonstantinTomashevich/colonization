#include "AngelScript/Utils/ClientUtils.as"
#include "AngelScript/Utils/PlayersNamesListEditor.as"

class ChatBlockedPlayersListEditor : PlayersNamesListEditor
{
    ChatBlockedPlayersListEditor ()
    {
        listVarName_ = "chatBlockedPlayersList";
    }

    ~ChatBlockedPlayersListEditor ()
    {

    }

    protected Window @GetWindow () override
    {
        return ui.root.GetChild ("ingame").GetChild ("chatBlockedPlayersWindow");
    }
}
