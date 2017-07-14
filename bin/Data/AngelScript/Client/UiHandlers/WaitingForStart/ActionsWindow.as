#include "AngelScript/Utils/Constants.as"
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
        if (scriptMain !is null and scriptMain.vars [ScriptMainVars::IS_ADMIN].GetBool ())
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
        eventData [ShowFunctionalWindowRequest::WINDOW_NAME] = Variant ("selectPlayerColorWindow");
        SendEvent (EVENT_SHOW_FUNCTIONAL_WINDOW_REQUEST, eventData);
    }

    void HandleAcceptStartClick ()
    {
        VectorBuffer buffer;
        buffer.WriteBool (true);

        VariantMap eventData;
        eventData [NewNetworkTask::TASK_TYPE] = Variant (CTS_NETWORK_MESSAGE_SEND_IS_PLAYER_READY_FOR_START);
        eventData [NewNetworkTask::MESSAGE_BUFFER] = Variant (buffer);
        SendEvent (EVENT_NEW_NETWORK_TASK, eventData);
    }

    void HandleDeclineStartClick ()
    {
        VectorBuffer buffer;
        buffer.WriteBool (false);

        VariantMap eventData;
        eventData [NewNetworkTask::TASK_TYPE] = Variant (CTS_NETWORK_MESSAGE_SEND_IS_PLAYER_READY_FOR_START);
        eventData [NewNetworkTask::MESSAGE_BUFFER] = Variant (buffer);
        SendEvent (EVENT_NEW_NETWORK_TASK, eventData);
    }

    void HandleRequestStartClick ()
    {
        Node @scriptMain = GetScriptMain (node);
        if (scriptMain !is null and scriptMain.vars [ScriptMainVars::IS_ADMIN].GetBool ())
        {
            SendEvent (EVENT_HOST_REQUEST_GAME_START);
        }
    }
}
