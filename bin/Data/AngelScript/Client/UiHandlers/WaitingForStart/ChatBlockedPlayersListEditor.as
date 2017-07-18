#include "AngelScript/Utils/Constants.as"
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

    void HandleHideClick () override
    {
        VariantMap eventData;
        eventData [ShowFunctionalWindowRequest::WINDOW_NAME] = Variant ("actionsWindow");
        SendEvent (EVENT_SHOW_FUNCTIONAL_WINDOW_REQUEST, eventData);
    }

    protected Window @GetWindow () override
    {
        return ui.root.GetChild ("waitingForStart").GetChild ("functionalWindows").GetChild ("chatBlockedPlayersWindow");
    }
}
