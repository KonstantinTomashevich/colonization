#include "AngelScript/Utils/Constants.as"

class ExitWindow : ScriptObject
{
    ExitWindow ()
    {

    }

    ~ExitWindow ()
    {

    }

    void Start ()
    {
        Button @exitButton = ui.root.GetChild ("waitingForStart").GetChild ("exitButton");
        SubscribeToEvent (exitButton, "Released", "HandleExitClick");
    }

    void Update (float timeStep)
    {

    }

    void Stop ()
    {
        UnsubscribeFromAllEvents ();
    }

    void HandleExitClick ()
    {
        SendEvent (EVENT_GO_TO_MAIN_MENU_REQUEST);
    }
}
