#include "AngelScript/Utils/ClientUtils.as"

class ActionsWindow : ScriptObject
{
    ActionsWindow ()
    {

    }

    ~ActionsWindow ()
    {

    }

    void Start ()
    {
        Window @actionsWindow = ui.root.GetChild ("waitingForStart").GetChild ("functionalWindows").GetChild ("actionsWindow");
        Button @selectPlayerColorButton = actionsWindow.GetChild ("selectPlayerColorButton");
        Button @acceptStartButton = actionsWindow.GetChild ("acceptStartButton");
        Button @declineStartButton = actionsWindow.GetChild ("declineStartButton");
        Button @requestStartButton = actionsWindow.GetChild ("requestStartButton");

        SubscribeToEvent (selectPlayerColorButton, "Released", "HandleSelectPlayerColorClick");
        SubscribeToEvent (acceptStartButton, "Released", "HandleAcceptStartClick");
        SubscribeToEvent (declineStartButton, "Released", "HandleDeclineStartClick");
        SubscribeToEvent (requestStartButton, "Released", "HandleRequestStartClick");
    }

    void Update (float timeStep)
    {
        Window @actionsWindow = ui.root.GetChild ("waitingForStart").GetChild ("functionalWindows").GetChild ("actionsWindow");
        Button @requestStartButton = actionsWindow.GetChild ("requestStartButton");

        Node @scriptMain = GetScriptMain (node);
        if (scriptMain !is null and scriptMain.vars ["isAdmin"].GetBool ())
        {
            requestStartButton.visible = true;
        }
        else
        {
            requestStartButton.visible = false;
        }
    }

    void Stop ()
    {

    }

    void HandleSelectPlayerColorClick ()
    {
        VariantMap eventData;
        eventData ["windowName"] = Variant ("selectPlayerColorWindow");
        SendEvent ("ShowFunctionalWindowRequest", eventData);
    }

    void HandleAcceptStartClick ()
    {
        VectorBuffer buffer;
        buffer.WriteBool (true);

        VariantMap eventData;
        eventData ["taskType"] = Variant (CTS_NETWORK_MESSAGE_SEND_IS_PLAYER_READY_FOR_START);
        eventData ["messageBuffer"] = Variant (buffer);
        SendEvent ("NewNetworkTask", eventData);
    }

    void HandleDeclineStartClick ()
    {
        VectorBuffer buffer;
        buffer.WriteBool (false);

        VariantMap eventData;
        eventData ["taskType"] = Variant (CTS_NETWORK_MESSAGE_SEND_IS_PLAYER_READY_FOR_START);
        eventData ["messageBuffer"] = Variant (buffer);
        SendEvent ("NewNetworkTask", eventData);
    }

    void HandleRequestStartClick ()
    {
        Node @scriptMain = GetScriptMain (node);
        if (scriptMain !is null and scriptMain.vars ["isAdmin"].GetBool ())
        {
            SendEvent (EVENT_HOST_REQUEST_GAME_START);
        }
    }
}
