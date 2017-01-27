class ScreenPressesHandler : ScriptObject
{
    protected Node @GetFirstReplicatedParentOf (Node @localNode)
    {
        Node @scanningNode = localNode;
        while (scanningNode !is null and scanningNode.id >= FIRST_LOCAL_ID)
            scanningNode = scanningNode.parent;
        return scanningNode;
    }

    protected void UnitSelected (Unit @unit)
    {
        node.vars ["selectionType"] = StringHash ("Unit");
        node.vars ["selectedHash"] = unit.hash;
    }

    protected void DistrictSelected (District @district)
    {
        node.vars ["selectionType"] = StringHash ("District");
        node.vars ["selectedHash"] = district.hash;
    }

    protected void SetUnitMoveTarget (Unit @unit, District @target)
    {
        if (unit.unitType != UNIT_COLONIZATORS and unit.unitType != UNIT_TRADERS)
        {
            Array <Variant> networkTasks = node.parent.GetChild ("networkScriptNode").vars ["tasksList"].GetVariantVector ();
            VariantMap taskData;
            taskData ["type"] = CTS_NETWORK_MESSAGE_SEND_PLAYER_ACTION;

            VectorBuffer buffer = VectorBuffer ();
            buffer.WriteInt (PLAYER_ACTION_SET_UNIT_MOVE_TARGET);
            buffer.WriteStringHash (unit.hash);
            buffer.WriteStringHash (target.hash);

            taskData ["buffer"] = buffer;
            networkTasks.Push (Variant (taskData));
            node.parent.GetChild ("networkScriptNode").vars ["tasksList"] = networkTasks;
        }
    }

    protected void ProcessDistrictSelection (Vector3 hitPosition)
    {
        MapMaskUpdater @mapMaskUpdater = scene.GetComponent ("MapMaskUpdater");
        StringHash districtHash = mapMaskUpdater.GetDistrictByPoint (hitPosition);

        Map @map = scene.GetChild ("map").GetComponent ("Map");
        District @district = map.GetDistrictByHash (districtHash);

        if (district !is null)
        {
            StringHash command = node.vars ["command"].GetStringHash ();
            if (command == StringHash ("NoCommand"))
            {
                DistrictSelected (district);
            }

            else if (command == StringHash ("MoveUnit"))
            {
                // Get selected unit.
                StringHash selectedHash = node.vars ["selectedHash"].GetStringHash ();
                Array <Node @> unitsNodes = scene.GetChild ("units").GetChildrenWithComponent ("Unit");
                Unit @unit = unitsNodes [0].GetComponent ("Unit");
                int index = 1;
                while (unit.hash != selectedHash and index < unitsNodes.length)
                {
                    unit = unitsNodes [index].GetComponent ("Unit");
                    index++;
                }

                SetUnitMoveTarget (unit, district);
            }
        }
        else
        {
            ClearSelection ();
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
        node.vars ["command"] = StringHash ("NoCommand");
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
                    if (firstReplicated.HasComponent ("Unit"))
                        UnitSelected (firstReplicated.GetComponent ("Unit"));
                    else
                        ProcessDistrictSelection (result.position);
                }
                else
                    ClearSelection ();
            }
            else
                ClearSelection ();
        }
    }
};
