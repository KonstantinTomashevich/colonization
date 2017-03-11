#include "AngelScript/Utils/ClientUtils.as"

class PlayerInfoWindow : ScriptObject
{
    PlayerInfoWindow ()
    {

    }

    ~PlayerInfoWindow ()
    {

    }

    void Start ()
    {

    }

    void Update (float timeStep)
    {
        Node @scriptMain = GetScriptMain (node);
        Text @playerStatsText = ui.root.GetChild ("ingame").GetChild ("playerStatsWindow").GetChild ("playerStatsInfo");
        String playerStatsInfo = "";
        playerStatsInfo += scriptMain.vars ["playerName"].GetString () + "\n";
        playerStatsInfo += "Gold: " + Floor (scriptMain.vars ["gold"].GetFloat ()) + "\n";
        playerStatsInfo += "Points: " + Floor (scriptMain.vars ["points"].GetFloat ());
        playerStatsText.text = playerStatsInfo;
    }

    void Stop ()
    {

    }
}
