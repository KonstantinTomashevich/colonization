#include "AngelScript/Utils/Constants.as"

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
        eventData [ShowWindowRequest::WINDOW_NAME] = Variant ("startGameMenu");
        SendEvent (EVENT_SHOW_WINDOW_REQUEST, eventData);
    }

    void HandleJoinGameClick ()
    {
        VariantMap eventData;
        eventData [ShowWindowRequest::WINDOW_NAME] = Variant ("joinGameMenu");
        SendEvent (EVENT_SHOW_WINDOW_REQUEST, eventData);
    }

    void HandleExitClick ()
    {
        SendEvent (EVENT_EXIT_REQUEST);
    }
}
