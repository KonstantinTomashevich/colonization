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
        UIElement @primaryMenu = ui.root.GetChild ("main_menu_root").GetChild ("primary_menu");
        Button @startGameButton = primaryMenu.GetChild ("start_game_button");
        Button @exitButton = primaryMenu.GetChild ("exit_from_game_button");

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
