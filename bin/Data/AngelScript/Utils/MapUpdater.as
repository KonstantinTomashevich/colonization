class MapUpdater : ScriptObject
{
    uint mapNodeId_;
    bool updateDistrictsHashesNextFrame_;
    bool updateDistrictsNeighborsNextFrame_;
    bool calculateUnitsPositionsAsCentersNextFrame_;
    bool setCorrectYForUnitsAndColoniesPositionNextFrame_;
    bool recalculateMapMaskNextFrame_;

    MapUpdater ()
    {
        updateDistrictsHashesNextFrame_ = false;
        updateDistrictsNeighborsNextFrame_ = false;
        calculateUnitsPositionsAsCentersNextFrame_ = false;
        setCorrectYForUnitsAndColoniesPositionNextFrame_ = false;
        recalculateMapMaskNextFrame_ = false;
    }

    ~MapUpdater ()
    {

    }

    void Start ()
    {

    }

    void Update (float timeStep)
    {
        if (updateDistrictsHashesNextFrame_)
        {
            Map @map = scene.GetNode (mapNodeId_).GetComponent ("Map");
            map.RecalculateDistrictsHashes ();
            updateDistrictsHashesNextFrame_ = false;
        }

        if (updateDistrictsNeighborsNextFrame_)
        {
            Map @map = scene.GetNode (mapNodeId_).GetComponent ("Map");
            map.RecalculateDistrictsNeighbors ();
            updateDistrictsNeighborsNextFrame_ = false;
        }

        if (calculateUnitsPositionsAsCentersNextFrame_)
        {
            Map @map = scene.GetNode (mapNodeId_).GetComponent ("Map");
            for (uint index = 0; index < map.districtsCount; index++)
            {
                CalculateUnitPositionAsCenter (map.GetDistrictByIndex (index));
            }
            calculateUnitsPositionsAsCentersNextFrame_ = false;
        }

        if (setCorrectYForUnitsAndColoniesPositionNextFrame_)
        {
            Map @map = scene.GetNode (mapNodeId_).GetComponent ("Map");
            Terrain @terrain = scene.GetNode (mapNodeId_).GetChild ("local").GetComponent ("Terrain");
            for (uint index = 0; index < map.districtsCount; index++)
            {
                CorrectYForUnitAndColonyPosition (map.GetDistrictByIndex (index), terrain);
            }
            setCorrectYForUnitsAndColoniesPositionNextFrame_ = false;
        }

        if (recalculateMapMaskNextFrame_)
        {
            MapMaskUpdater @mapMaskUpdater = scene.GetComponent ("MapMaskUpdater");
            mapMaskUpdater.RecalculateMaskImage ();
            recalculateMapMaskNextFrame_ = false;
        }
    }

    void Stop ()
    {

    }

    protected void CalculateUnitPositionAsCenter (District @district)
    {
        Array <Vector3> polygonPoints = district.polygonPoints;
        Vector3 sum;
        for (uint index = 0; index < polygonPoints.length; index++)
        {
            sum.x += polygonPoints [index].x;
            sum.y += polygonPoints [index].y;
            sum.z += polygonPoints [index].z;
        }
        district.unitPosition = sum / polygonPoints.length;
    }

    protected void CorrectYForUnitAndColonyPosition (District @district, Terrain @terrain)
    {
        float unitHeight = terrain.GetHeight (district.unitPosition);
        float colonyHeight = terrain.GetHeight (district.colonyPosition);

        Vector3 newUnitPosition = district.unitPosition;
        newUnitPosition.y = unitHeight;
        district.unitPosition = newUnitPosition;

        Vector3 newColonyPosition = district.colonyPosition;
        newColonyPosition.y = colonyHeight;
        district.colonyPosition = newColonyPosition;
    }
};
