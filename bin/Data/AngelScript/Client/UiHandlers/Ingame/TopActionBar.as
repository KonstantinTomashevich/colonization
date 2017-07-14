#include "AngelScript/Utils/Constants.as"
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
        Button @infoTableButton = topActionBar.GetChild ("infoTableButton");

        SubscribeToEvent (exitButton, "Released", "HandleExitClick");
        SubscribeToEvent (infoTableButton, "Released", "HandleOpenInfoTableClick");
    }

    void Update (float timeStep)
    {
        Node @scriptMain = GetScriptMain (node);
        Window @topActionBar = ui.root.GetChild ("ingame").GetChild ("topActionBar");
        Text @playerStatsText = topActionBar.GetChild ("playerStatsInfo");

        String playerStatsInfo = "";
        playerStatsInfo += scriptMain.vars [ScriptMainVars::PLAYER_NAME].GetString () + "  ";
        playerStatsInfo += "Gold: " + Floor (scriptMain.vars [ScriptMainVars::GOLD].GetFloat ()) + "  ";
        playerStatsInfo += "Points: " + Floor (scriptMain.vars [ScriptMainVars::POINTS].GetFloat ());
        playerStatsText.text = playerStatsInfo;

        BorderImage @colorSample = topActionBar.GetChild ("colorSample");
        PlayerInfo @playerInfo = GetPlayerInfoByName (scene, scriptMain.vars [ScriptMainVars::PLAYER_NAME].GetString ());
        colorSample.color = playerInfo.color;
    }

    void Stop ()
    {

    }

    void HandleExitClick ()
    {
        SendEvent (EVENT_GO_TO_MAIN_MENU_REQUEST);
    }

    void HandleOpenInfoTableClick ()
    {
        ui.root.GetChild ("ingame").GetChild ("infoTableWindow").visible = true;
    }
}
