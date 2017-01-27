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
        return unit;
    else
        return null;
}
