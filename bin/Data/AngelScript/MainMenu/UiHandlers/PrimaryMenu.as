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
        for (int index = 0; index < ui.root.GetChild ("mainMenu").GetNumChildren (false); index++)
        {
            ui.root.GetChild ("mainMenu").GetChildren () [index].visible = false;
        }
        ui.root.GetChild ("mainMenu").GetChild ("startGameMenu").visible = true;
    }

    void HandleJoinGameClick ()
    {
        for (int index = 0; index < ui.root.GetChild ("mainMenu").GetNumChildren (false); index++)
        {
            ui.root.GetChild ("mainMenu").GetChildren () [index].visible = false;
        }
        ui.root.GetChild ("mainMenu").GetChild ("joinGameMenu").visible = true;
    }

    void HandleExitClick ()
    {
        SendEvent ("ExitRequest");
    }
}
