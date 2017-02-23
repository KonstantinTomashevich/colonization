#include "AngelScript/Utils/ClientUtils.as"

class ScreenPressesHandler : ScriptObject
{
    protected float RAYCAST_RAY_LENGTH = 300.0f;

    protected Node @GetFirstReplicatedParentOf (Node @localNode)
    {
        Node @scanningNode = localNode;
        while (scanningNode !is null and scanningNode.id >= FIRST_LOCAL_ID)
        {
            scanningNode = scanningNode.parent;
        }
        return scanningNode;
    }

    protected void UnitSelected (Unit @unit)
    {
        node.parent.vars ["selectionType"] = StringHash ("Unit");
        node.parent.vars ["selectedHash"] = unit.hash;

        // Inform map mask updater about selection.
        MapMaskUpdater @mapMaskUpdater = scene.GetComponent ("MapMaskUpdater");
        mapMaskUpdater.selectedDistrictHash = StringHash (0);
    }

    protected void DistrictSelected (District @district)
    {
        node.parent.vars ["selectionType"] = StringHash ("District");
        node.parent.vars ["selectedHash"] = district.hash;

        // Inform map mask updater about selection.
        MapMaskUpdater @mapMaskUpdater = scene.GetComponent ("MapMaskUpdater");
        mapMaskUpdater.selectedDistrictHash = district.hash;
    }

    protected void ClearSelection ()
    {
        node.parent.vars ["selectionType"] = StringHash ("None");
        node.parent.vars ["selectedHash"] = StringHash ();

        // Inform map mask updater about selection.
        MapMaskUpdater @mapMaskUpdater = scene.GetComponent ("MapMaskUpdater");
        mapMaskUpdater.selectedDistrictHash = StringHash (0);
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
        node.parent.vars ["currentClickCommand"] = StringHash ("NoCommand");
    }

    protected void ProcessDistrictSelection (Vector3 hitPosition)
    {
        MapMaskUpdater @mapMaskUpdater = scene.GetComponent ("MapMaskUpdater");
        StringHash districtHash = mapMaskUpdater.GetDistrictByPoint (hitPosition);

        Map @map = scene.GetChild ("map").GetComponent ("Map");
        District @district = map.GetDistrictByHash (districtHash);

        if (district !is null)
        {
            StringHash command = node.parent.vars ["currentClickCommand"].GetStringHash ();
            if (command == StringHash ("NoCommand"))
            {
                DistrictSelected (district);
            }

            else if (command == StringHash ("MoveUnit"))
            {
                // Get selected unit.
                StringHash selectedHash = node.parent.vars ["selectedHash"].GetStringHash ();
                Unit @unit = GetUnitByHash (scene, selectedHash);
                if (unit !is null)
                {
                    SetUnitMoveTarget (unit, district);
                }
                // If unit no longer exists, select district instead.
                else
                {
                    DistrictSelected (district);
                }
            }
        }
        else
        {
            ClearSelection ();
        }
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
        node.parent.vars ["currentClickCommand"] = StringHash ("NoCommand");
    }

    void Update (float timeStep)
    {

    }

    void Stop ()
    {

    }

    void HandleScreenPress (StringHash eventType, VariantMap &eventData)
    {
        if (eventData ["Element"].GetPtr () is null && node.parent.vars ["gameState"].GetInt () != GAME_STATE_WAITING_FOR_START)
        {
            Camera @camera = scene.GetChild ("camera").GetComponent ("Camera");
            Ray ray = camera.GetScreenRay (eventData ["X"].GetInt () * 1.0f / graphics.width,
                                           eventData ["Y"].GetInt () * 1.0f / graphics.height);
            RayQueryResult result = octree.RaycastSingle (ray, RAY_TRIANGLE, RAYCAST_RAY_LENGTH, DRAWABLE_GEOMETRY);

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
