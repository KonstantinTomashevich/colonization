#include "AngelScript/Utils/Constants.as"
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

    void HandleHideClick () override
    {
        VariantMap eventData;
        eventData [ShowFunctionalWindowRequest::WINDOW_NAME] = Variant ("actionsWindow");
        SendEvent (EVENT_SHOW_FUNCTIONAL_WINDOW_REQUEST, eventData);
    }

    protected Window @GetWindow () override
    {
        return ui.root.GetChild ("waitingForStart").GetChild ("functionalWindows").GetChild ("chatPrivateReceiversWindow");
    }
}
