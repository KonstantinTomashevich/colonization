class PrimaryMenu : ScriptObject
{
    PrimaryMenu ()
    {

    }

    ~PrimaryMenu ()
    {

    }

    void Start ()
    {
        Window @primaryMenu = ui.root.GetChild ("mainMenu").GetChild ("primaryMenu");
        Button @startGameButton = primaryMenu.GetChild ("startGameButton");
        Button @joinGameButton = primaryMenu.GetChild ("joinGameButton");
        Button @exitButton = primaryMenu.GetChild ("exitFromGameButton");

        SubscribeToEvent (startGameButton, "Released", "HandleStartGameClick");
        SubscribeToEvent (joinGameButton, "Released", "HandleJoinGameClick");
        SubscribeToEvent (exitButton, "Released", "HandleExitClick");
    }

    void Update (float timeStep)
    {

    }

    void Stop ()
    {

    }

    void HandleStartGameClick ()
    {
        VariantMap eventData;
        eventData ["windowName"] = Variant ("startGameMenu");
        SendEvent ("ShowWindowRequest", eventData);
    }

    void HandleJoinGameClick ()
    {
        VariantMap eventData;
        eventData ["windowName"] = Variant ("joinGameMenu");
        SendEvent ("ShowWindowRequest", eventData);
    }

    void HandleExitClick ()
    {
        SendEvent ("ExitRequest");
    }
}
