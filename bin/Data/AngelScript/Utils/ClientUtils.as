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
    if (scene_.vars ["ReplicatedNodesCount"].GetInt () != 0)
    {
        Array <Node @> children = scene_.GetChildren (true);
        int replicated = 0;
        for (int index = 0; index < children.length; index++)
        {
            if (children [index].id < FIRST_LOCAL_ID)
            {
                replicated++;
            }
        }
        return (replicated == scene_.vars ["ReplicatedNodesCount"].GetInt ());
    }
    else
        return false;
}

shared PlayerInfo @GetPlayerInfoByName (Scene @scene_, String playerName)
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
    int index = 1;
    while (playerInfo.name != playerName and index < playersNodes.length)
    {
        playerInfo = playersNodes [index].GetComponent ("PlayerInfo");
        index++;
    }

    if (playerInfo.name == playerName)
    {
        return playerInfo;
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
    int index = 1;
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

shared Array <Unit @> GetUnitsInDistrict (Scene @scene_, StringHash districtHash)
{
    Array <Unit @> unitsInDistrict;
    if (scene_.GetChild ("units") is null)
    {
        return unitsInDistrict;
    }

    Array <Node @> unitsNodes = scene_.GetChild ("units").GetChildrenWithComponent ("Unit");
    for (int index = 0; index < unitsNodes.length; index++)
    {
        Unit @unit = unitsNodes [index].GetComponent ("Unit");
        if (unit.positionHash == districtHash)
        {
            unitsInDistrict.Push (unit);
        }
    }

    return unitsInDistrict;
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
    int index = lineEditVector.Find (Variant (lineEdit));
    if (index >= 0)
    {
        lineEditVector.Erase (index);
    }
    scriptMain.vars ["lineEditVector"] = Variant (lineEditVector);
}

shared bool IsAnyLineEditFocused (Node @scriptMain)
{
    Array <Variant> lineEditVector = scriptMain.vars ["lineEditVector"].GetVariantVector ();
    for (int index = 0; index < lineEditVector.length; index++)
    {
        LineEdit @lineEdit = lineEditVector [index].GetPtr ();
        if (lineEdit.focus)
        {
            return true;
        }
    }
    return false;
}
