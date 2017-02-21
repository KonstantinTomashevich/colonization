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
        UIElement @uiRoot = ui.root.GetChild ("main_menu_root");
        Button @startGameButton = uiRoot.GetChild ("start_game_button");
        Button @exitButton = uiRoot.GetChild ("exit_from_game_button");

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
        SendEvent ("StartGameRequest");
    }

    void HandleExitClick ()
    {
        SendEvent ("ExitRequest");
    }
}
