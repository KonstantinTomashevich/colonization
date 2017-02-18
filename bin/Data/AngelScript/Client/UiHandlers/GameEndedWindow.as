class GameEndedWindow : ScriptObject
{
    GameEndedWindow ()
    {

    }

    ~GameEndedWindow ()
    {

    }

    void Start ()
    {
        ui.root.GetChild ("ingame").GetChild ("gameEndedWindow").visible = false;
        SubscribeToEvent ("GameEnded", "HandleGameEnded");
    }

    void Update (float timeStep)
    {

    }

    void Stop ()
    {

    }

    void HandleGameEnded (StringHash eventType, VariantMap &eventData)
    {
        String winnerName = eventData ["winnerName"].GetString ();
        String victoryType = eventData ["victoryType"].GetString ();
        String victoryInfo = eventData ["victoryInfo"].GetString ();

        Window @gameEndedWindow = ui.root.GetChild ("ingame").GetChild ("gameEndedWindow");
        gameEndedWindow.visible = true;

        Text @infoText = gameEndedWindow.GetChild ("info");
        infoText.text = victoryInfo + "\n" + winnerName + " won the game " + victoryType + "!";
    }
}
