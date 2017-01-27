shared Array <Unit @> GetUnitsInDistrict (Scene @scene_, StringHash districtHash)
{
    Array <Node @> unitsNodes = scene_.GetChild ("units").GetChildrenWithComponent ("Unit");
    Array <Unit @> unitsInDistrict;

    for (int index = 0; index < unitsNodes.length; index++)
    {
        Unit @unit = unitsNodes [index].GetComponent ("Unit");
        if (unit.positionHash == districtHash)
            unitsInDistrict.Push (unit);
    }

    return unitsInDistrict;
}
