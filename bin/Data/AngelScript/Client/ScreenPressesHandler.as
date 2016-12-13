class ScreenPressesHandler : ScriptObject
{
    protected Node @GetFirstReplicatedParentOf (Node @localNode)
    {
        Node @scanningNode = localNode;
        while (scanningNode !is null and scanningNode.id >= FIRST_LOCAL_ID)
            scanningNode = scanningNode.parent;
        return scanningNode;
    }
    
    protected int GetUnitIndexByNodeId (int id)
    {
        Array <Node @> units = scene.GetChild ("units").GetChildren ();
        for (int index = 0; index < units.length; index++)
            if (units [index].id == id)
                return index;
        return -1;
    }
    
    protected int GetDistrictIndexByNodeId (int id)
    {
        Array <Node @> units = scene.GetChild ("map").GetChildren ();
        for (int index = 0; index < units.length; index++)
            if (units [index].id == id)
                return index;
        return -1;
    }
    
    protected void UnitSelected (int index)
    {
        
    }
    
    protected void DistrictSelected (int index)
    {
        
    }
    
    protected void ClearSelection ()
    {
        
    }
    
    ScreenPressesHandler ()
    {
        
    }
    
    ~ScreenPressesHandler ()
    {
        
    }
    
    void Start ()
    {
        SubscribeToEvent ("UIMouseClick", "HandleScreenPress");
    }
    
    void Update (float timeStep)
    {
        
    }
    
    void Stop ()
    {
        
    }
    
    void HandleScreenPress (StringHash eventType, VariantMap &eventData)
    {
        if (eventData ["Element"].GetPtr () is null)
        {
            Camera @camera = scene.GetChild ("camera").GetComponent ("Camera");
            Ray ray = camera.GetScreenRay (eventData ["X"].GetInt () * 1.0f / graphics.width,
                                           eventData ["Y"].GetInt () * 1.0f / graphics.height);
            RayQueryResult result = octree.RaycastSingle (ray, RAY_TRIANGLE, 300, DRAWABLE_GEOMETRY);
            
            if (result.node !is null)
            {
                Node @firstReplicated = GetFirstReplicatedParentOf (result.node);
                if (firstReplicated !is scene)
                {
                    int objectIndex = GetUnitIndexByNodeId (firstReplicated.id);
                    if (objectIndex >= 0)
                        UnitSelected (objectIndex);
                    else
                    {
                        objectIndex = GetDistrictIndexByNodeId (firstReplicated.id);
                        if (objectIndex >= 0)
                            DistrictSelected (objectIndex);
                        else
                            ClearSelection ();
                    }
                }
            }
            else
                ClearSelection ();
        }
    }
};
