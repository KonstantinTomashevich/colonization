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
        UnitsContainer @unitsContainer = node.parent.vars ["unitsContainer"].GetPtr ();
        Unit @unit = unitsContainer.GetUnitByIndex (index);
        if (unit.unitType_ != UNIT_COLONIZATORS and unit.unitType_ != UNIT_TRADERS)
        {
            node.vars ["selectionType"] = StringHash ("Unit");
            node.vars ["selectedHash"] = unit.hash;
        }
    }
    
    protected void DistrictSelected (int index)
    {
        Map @map = node.parent.vars ["map"].GetPtr ();
        if (node.vars ["selectionType"].GetStringHash () == StringHash ("Unit"))
        {
            UnitsContainer @unitsContainer = node.parent.vars ["unitsContainer"].GetPtr ();
            Array <Variant> networkTasks = node.parent.GetChild ("networkScriptNode").vars ["tasksList"].GetVariantVector ();
            VariantMap taskData;
            taskData ["type"] = CTS_NETWORK_MESSAGE_SEND_PLAYER_ACTION;
            
            VectorBuffer buffer = VectorBuffer ();
            buffer.WriteInt (PLAYER_ACTION_SET_UNIT_MOVE_TARGET);
            buffer.WriteStringHash (node.vars ["selectedHash"].GetStringHash ());
            buffer.WriteStringHash (map.GetDistrictByIndex (index).hash);
            
            taskData ["buffer"] = buffer;
            networkTasks.Push (Variant (taskData));
            node.parent.GetChild ("networkScriptNode").vars ["tasksList"] = networkTasks;
        }
        else
        {
            node.vars ["selectionType"] = StringHash ("District");
            node.vars ["selectedHash"] = map.GetDistrictByIndex (index).hash;
        }
    }
    
    protected void ClearSelection ()
    {
        node.vars ["selectionType"] = StringHash ("None");
        node.vars ["selectedHash"] = StringHash ();
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
                else
                    ClearSelection ();
            }
            else
                ClearSelection ();
        }
    }
};
