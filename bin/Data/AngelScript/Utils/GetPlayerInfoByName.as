shared PlayerInfo @GetPlayerInfoByName (Scene @scene_, String playerName)
{
    Array <Node @> playersNodes = scene_.GetChild ("players").GetChildrenWithComponent ("PlayerInfo");
    PlayerInfo @playerInfo = playersNodes [0].GetComponent ("PlayerInfo");
    int index = 1;
    while (playerInfo.name != playerName and index < playersNodes.length)
    {
        playerInfo = playersNodes [index].GetComponent ("PlayerInfo");
        index++;
    }

    if (playerInfo.name == playerName)
        return playerInfo;
    else
        return null;
}
