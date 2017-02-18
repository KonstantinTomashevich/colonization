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
    Array <Node @> unitsNodes = scene_.GetChild ("units").GetChildrenWithComponent ("Unit");
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
    Array <Node @> unitsNodes = scene_.GetChild ("units").GetChildrenWithComponent ("Unit");
    Array <Unit @> unitsInDistrict;

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
    int foundIndex = 0;
    int index = 0;
    while (foundIndex < 0 and index < lineEditVector.length)
    {
        LineEdit @lineEditFromVector = lineEditVector [index].GetPtr ();
        if (lineEditFromVector is lineEdit)
        {
            foundIndex = index;
        }
        else
        {
            index++;
        }
    }
    if (foundIndex >= 0)
    {
        lineEditVector.Erase (foundIndex);
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
