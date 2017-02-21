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
        Button @exitButton = primaryMenu.GetChild ("exitFromGameButton");

        SubscribeToEvent (startGameButton, "Released", "HandleStartGameClick");
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
        ui.root.GetChild ("mainMenu").GetChild ("primaryMenu").visible = false;
        ui.root.GetChild ("mainMenu").GetChild ("startGameMenu").visible = true;
    }

    void HandleExitClick ()
    {
        SendEvent ("ExitRequest");
    }
}
