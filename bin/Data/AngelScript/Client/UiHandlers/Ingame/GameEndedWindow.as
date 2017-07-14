#include "AngelScript/Utils/Constants.as"

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
        SubscribeToEvent (EVENT_GAME_ENDED, "HandleGameEnded");
    }

    void Update (float timeStep)
    {

    }

    void Stop ()
    {

    }

    void HandleGameEnded (StringHash eventType, VariantMap &eventData)
    {
        String winnerName = eventData [GameEnded::WINNER_NAME].GetString ();
        String victoryType = eventData [GameEnded::VICTORY_TYPE].GetString ();
        String victoryInfo = eventData [GameEnded::VICTORY_INFO].GetString ();

        Window @gameEndedWindow = ui.root.GetChild ("ingame").GetChild ("gameEndedWindow");
        gameEndedWindow.visible = true;

        Text @infoText = gameEndedWindow.GetChild ("info");
        infoText.text = victoryInfo + "\n" + winnerName + " won the game " + victoryType + "!";
    }
}
