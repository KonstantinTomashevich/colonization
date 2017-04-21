#include "AngelScript/Utils/ClientUtils.as"

class TopActionBar : ScriptObject
{
    TopActionBar ()
    {

    }

    ~TopActionBar ()
    {

    }

    void Start ()
    {
        Window @topActionBar = ui.root.GetChild ("ingame").GetChild ("topActionBar");
        Button @exitButton = topActionBar.GetChild ("exitButton");
        SubscribeToEvent (exitButton, "Released", "HandleExitClick");
    }

    void Update (float timeStep)
    {
        Node @scriptMain = GetScriptMain (node);
        Window @topActionBar = ui.root.GetChild ("ingame").GetChild ("topActionBar");
        Text @playerStatsText = topActionBar.GetChild ("playerStatsInfo");

        String playerStatsInfo = "";
        playerStatsInfo += scriptMain.vars ["playerName"].GetString () + "  ";
        playerStatsInfo += "Gold: " + Floor (scriptMain.vars ["gold"].GetFloat ()) + "  ";
        playerStatsInfo += "Points: " + Floor (scriptMain.vars ["points"].GetFloat ());
        playerStatsText.text = playerStatsInfo;
    }

    void Stop ()
    {

    }

    void HandleExitClick ()
    {
        SendEvent ("GoToMainMenuRequest");
    }
}
