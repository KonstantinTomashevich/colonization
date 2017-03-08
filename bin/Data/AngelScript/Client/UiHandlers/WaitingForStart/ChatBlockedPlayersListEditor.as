#include "AngelScript/Utils/ClientUtils.as"
#include "AngelScript/Utils/PlayersNamesListEditor.as"

class ChatBlockedPlayersListEditor : PlayersNamesListEditor
{
    protected Window @GetWindow () override
    {
        return ui.root.GetChild ("waitingForStart").GetChild ("functionalWindows").GetChild ("chatBlockedPlayersWindow");
    }

    ChatBlockedPlayersListEditor ()
    {
        listVarName_ = "chatBlockedPlayersList";
    }

    ~ChatBlockedPlayersListEditor ()
    {

    }

    void HandleHideClick () override
    {
        VariantMap eventData;
        eventData ["windowName"] = Variant ("actionsWindow");
        SendEvent ("ShowFunctionalWindowRequest", eventData);
    }
}
