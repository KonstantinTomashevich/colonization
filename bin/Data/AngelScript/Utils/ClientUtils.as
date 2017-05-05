#include "AngelScript/Utils/Constants.as"

shared Node @GetScriptMain (Node @requester)
{
    Array <Node @> nodesWithTag = requester.scene.GetChildrenWithTag ("ScriptMain", true);
    if (not nodesWithTag.empty)
    {
        return nodesWithTag [0];
    }
    else
    {
        return null;
    }
}

shared bool CheckIsSceneLoaded (Scene @scene_)
{
    if (scene_.vars [VAR_REPLICATED_NODES_COUNT].GetInt () != 0)
    {
        Array <Node @> children = scene_.GetChildren (true);
        int replicated = 0;
        for (uint index = 0; index < children.length; index++)
        {
            if (children [index].id < FIRST_LOCAL_ID)
            {
                replicated++;
            }
        }
        return (replicated == scene_.vars [VAR_REPLICATED_NODES_COUNT].GetInt ());
    }
    else
        return false;
}

shared PlayerInfo @GetPlayerInfoByNameHash (Scene @scene_, StringHash playerNameHash)
{
    if (scene_.GetChild ("players") is null)
    {
        return null;
    }

    Array <Node @> playersNodes = scene_.GetChild ("players").GetChildrenWithComponent ("PlayerInfo");
    if (playersNodes.empty)
    {
        return null;
    }

    PlayerInfo @playerInfo = playersNodes [0].GetComponent ("PlayerInfo");
    uint index = 1;
    while (StringHash (playerInfo.name) != playerNameHash and index < playersNodes.length)
    {
        playerInfo = playersNodes [index].GetComponent ("PlayerInfo");
        index++;
    }

    if (StringHash (playerInfo.name) == playerNameHash)
    {
        return playerInfo;
    }
    else
    {
        return null;
    }
}

shared PlayerInfo @GetPlayerInfoByName (Scene @scene_, String playerName)
{
    return GetPlayerInfoByNameHash (scene_, StringHash (playerName));
}

shared PlayerInfo @GetPlayerInfoByIndex (Scene @scene_, uint index)
{
    if (scene_.GetChild ("players") is null)
    {
        return null;
    }

    Array <Node @> playersNodes = scene_.GetChild ("players").GetChildrenWithComponent ("PlayerInfo");
    if (playersNodes.empty)
    {
        return null;
    }

    if (index < playersNodes.length)
    {
        return playersNodes [index].GetComponent ("PlayerInfo");
    }
    else
    {
        return null;
    }
}

shared Array <String> GetPlayersNamesList (Scene @scene_)
{
    Array <String> playersNames;
    if (scene_.GetChild ("players") is null)
    {
        return playersNames;
    }

    Array <Node @> playersNodes = scene_.GetChild ("players").GetChildrenWithComponent ("PlayerInfo");
    if (playersNodes.empty)
    {
        return playersNames;
    }

    for (uint index = 0; index < playersNodes.length; index++)
    {
        PlayerInfo @playerInfo = playersNodes [index].GetComponent ("PlayerInfo");
        playersNames.Push (playerInfo.name);
    }
    return playersNames;
}

shared String ConstructWarShortname (Scene @scene_, DiplomacyWar @war)
{
    PlayerInfo @firstAttacker = GetPlayerInfoByNameHash (scene_, war.GetAttackerNameHashByIndex (0));
    PlayerInfo @firstDefender = GetPlayerInfoByNameHash (scene_, war.GetDefenderNameHashByIndex (0));
    String shortname = firstAttacker.name;
    if (war.attackersCount > 1)
    {
        shortname += " (" + war.attackersCount + ")";
    }

    shortname += " vs ";
    shortname += firstDefender.name;
    if (war.defendersCount > 1)
    {
        shortname += " (" + war.defendersCount + ")";
    }
    return shortname;
}

shared Array <String> GetWarsShortnamesList (Scene @scene_, Array <StringHash> &out warsHashesArray)
{
    Array <String> warsShortnames;
    if (scene_.GetChild ("diplomacy") is null)
    {
        return warsShortnames;
    }

    Array <Node @> warsNodes = scene_.GetChild ("diplomacy").GetChildrenWithComponent ("DiplomacyWar");
    if (warsNodes.empty)
    {
        return warsShortnames;
    }

    for (uint index = 0; index < warsNodes.length; index++)
    {
        DiplomacyWar @war = warsNodes [index].GetComponent ("DiplomacyWar");
        warsHashesArray.Push (war.hash);
        warsShortnames.Push (ConstructWarShortname (scene_, war));
    }
    return warsShortnames;
}

DiplomacyWar @GetWarByHash (Scene @scene_, StringHash warHash)
{
    if (scene_.GetChild ("diplomacy") is null)
    {
        return null;
    }

    Array <Node @> warsNodes = scene_.GetChild ("diplomacy").GetChildrenWithComponent ("DiplomacyWar");
    if (warsNodes.empty)
    {
        return null;
    }

    DiplomacyWar @war = warsNodes [0].GetComponent ("DiplomacyWar");
    uint index = 1;
    while (war.hash != warHash and index < warsNodes.length)
    {
        war = warsNodes [index].GetComponent ("DiplomacyWar");
        index++;
    }

    if (war.hash == warHash)
    {
        return war;
    }
    else
    {
        return null;
    }
}

shared Unit @GetUnitByHash (Scene @scene_, StringHash unitHash)
{
    if (scene_.GetChild ("units") is null)
    {
        return null;
    }

    Array <Node @> unitsNodes = scene_.GetChild ("units").GetChildrenWithComponent ("Unit");
    if (unitsNodes.empty)
    {
        return null;
    }

    Unit @unit = unitsNodes [0].GetComponent ("Unit");
    uint index = 1;
    while (unit.hash != unitHash and index < unitsNodes.length)
    {
        unit = unitsNodes [index].GetComponent ("Unit");
        index++;
    }

    if (unit.hash == unitHash)
    {
        return unit;
    }
    else
    {
        return null;
    }
}

Array <DiplomacyWar @> GetWarsWithPlayer (Scene @scene_, StringHash playerNameHash)
{
    Array <DiplomacyWar @> wars;
    if (scene_.GetChild ("diplomacy") is null)
    {
        return wars;
    }

    Array <Node @> warsNodes = scene_.GetChild ("diplomacy").GetChildrenWithComponent ("DiplomacyWar");
    if (warsNodes.empty)
    {
        return wars;
    }

    for (uint index = 0; index < warsNodes.length; index++)
    {
        DiplomacyWar @war = warsNodes [index].GetComponent ("DiplomacyWar");
        if (war.IsAttacker (playerNameHash) or war.IsDefender (playerNameHash))
        {
            wars.Push (war);
        }
    }
    return wars;
}

Array <DiplomacyWar @> FindWarsWhereThesePlayersFight (Scene @scene_, StringHash firstNameHash, StringHash secondNameHash)
{
    Array <DiplomacyWar @> wars;
    Array <DiplomacyWar @> warsOfFirstPlayer = GetWarsWithPlayer (scene_, firstNameHash);
    if (warsOfFirstPlayer.empty)
    {
        return wars;
    }

    for (uint index = 0; index < warsOfFirstPlayer.length; index++)
    {
        DiplomacyWar @war = warsOfFirstPlayer [index];
        if ((war.IsAttacker (firstNameHash) and war.IsDefender (secondNameHash)) or
            (war.IsDefender (firstNameHash) and war.IsAttacker (secondNameHash)))
        {
            wars.Push (war);
        }
    }
    return wars;
}

shared void RegisterLineEdit (Node @scriptMain, LineEdit @lineEdit)
{
    Array <Variant> lineEditVector = scriptMain.vars ["lineEditVector"].GetVariantVector ();
    lineEditVector.Push (Variant (lineEdit));
    scriptMain.vars ["lineEditVector"] = Variant (lineEditVector);
}

shared void UnregisterLineEdit (Node @scriptMain, LineEdit @lineEdit)
{
    Array <Variant> lineEditVector = scriptMain.vars ["lineEditVector"].GetVariantVector ();
    uint index = lineEditVector.Find (Variant (lineEdit));
    if (index >= 0)
    {
        lineEditVector.Erase (index);
    }
    scriptMain.vars ["lineEditVector"] = Variant (lineEditVector);
}

shared bool IsAnyLineEditFocused (Node @scriptMain)
{
    Array <Variant> lineEditVector = scriptMain.vars ["lineEditVector"].GetVariantVector ();
    for (uint index = 0; index < lineEditVector.length; index++)
    {
        LineEdit @lineEdit = lineEditVector [index].GetPtr ();
        if (lineEdit.focus)
        {
            return true;
        }
    }
    return false;
}

Array <String> GetMapsFoldersNames (String filesystemPrefix = FILESYSTEM_PREFIX,
     String mapsFolder = MAPS_FOLDER,
     String mapInfoFileName = MAP_INFO_FILE,
     String filter = EMPTY_FILTER)
{
    Array <String> foldersNames;
    foldersNames = fileSystem.ScanDir (filesystemPrefix + mapsFolder, mapInfoFileName, SCAN_DIRS, false);
    // Check maps list and delete items if they aren't maps.
    uint index = 0;
    while (index < foldersNames .length)
    {
        if (fileSystem.FileExists (filesystemPrefix + mapsFolder + foldersNames [index] + mapInfoFileName))
        {
            index++;
        }
        else
        {
            foldersNames.Erase (index);
        }
    }
    return foldersNames;
}
