#include "AngelScript/Utils/ClientUtils.as"
#include "AngelScript/Utils/PlayersNamesListEditor.as"

class ChatPrivateReceiversListEditor : PlayersNamesListEditor
{
    protected Window @GetWindow () override
    {
        return ui.root.GetChild ("waitingForStart").GetChild ("functionalWindows").GetChild ("chatPrivateReceiversWindow");
    }

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
        eventData ["windowName"] = Variant ("actionsWindow");
        SendEvent ("ShowFunctionalWindowRequest", eventData);
    }
}
