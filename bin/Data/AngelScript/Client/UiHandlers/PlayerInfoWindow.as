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
        Text @playerStatsText = ui.root.GetChild ("ingame").GetChild ("playerStatsWindow").GetChild ("playerStatsInfo");
        String playerStatsInfo = "";
        playerStatsInfo += node.parent.parent.vars ["playerName"].GetString () + "\n";
        playerStatsInfo += "Gold: " + Floor (node.parent.parent.vars ["gold"].GetFloat ()) + "\n";
        playerStatsInfo += "Points: " + Floor (node.parent.parent.vars ["points"].GetFloat ());
        playerStatsText.text = playerStatsInfo;
    }

    void Stop ()
    {

    }
}
