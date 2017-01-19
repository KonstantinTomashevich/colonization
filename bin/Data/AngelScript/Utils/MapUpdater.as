class MapUpdater : ScriptObject
{
    uint mapNodeId_;
    bool updateDistrictsHashesNextFrame_;
    bool updateDistrictsNeighborsNextFrame_;
    bool calculateUnitsPositionsAsCentersNextFrame_;
    bool setCorrectYForUnitsAndColoniesPositionNextFrame_;
    
    protected void CalculateUnitPositionAsCenter (District @district)
    {
        Array <Vector3> polygonPoints = district.polygonPoints;
        Vector3 sum;
        for (int index = 0; index < polygonPoints.length; index++)
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
    
    MapUpdater ()
    {
        updateDistrictsHashesNextFrame_ = false;
        updateDistrictsNeighborsNextFrame_ = false;
        calculateUnitsPositionsAsCentersNextFrame_ = false;
        setCorrectYForUnitsAndColoniesPositionNextFrame_ = false;
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
            for (int index = 0; index < map.GetDistrictsCount (); index++)
                CalculateUnitPositionAsCenter (map.GetDistrictByIndex (index));
            calculateUnitsPositionsAsCentersNextFrame_ = false;
        }
        
        if (setCorrectYForUnitsAndColoniesPositionNextFrame_)
        {
            Map @map = scene.GetNode (mapNodeId_).GetComponent ("Map");
            Terrain @terrain = scene.GetNode (mapNodeId_).GetChild ("local").GetComponent ("Terrain");
            for (int index = 0; index < map.GetDistrictsCount (); index++)
                CorrectYForUnitAndColonyPosition (map.GetDistrictByIndex (index), terrain);
            setCorrectYForUnitsAndColoniesPositionNextFrame_ = false;
        }
    }
    
    void Stop ()
    {
        
    }
};