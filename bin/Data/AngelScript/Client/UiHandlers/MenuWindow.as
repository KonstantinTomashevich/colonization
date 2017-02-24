class MenuWindow : ScriptObject
{
    MenuWindow ()
    {

    }

    ~MenuWindow ()
    {

    }

    void Start ()
    {
        Button @exitButton = ui.root.GetChild ("ingame").GetChild ("exitButton");
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
        SendEvent ("GoToMainMenuRequest");
    }
}
