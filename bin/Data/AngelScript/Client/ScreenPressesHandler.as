#include "AngelScript/Utils/Constants.as"
#include "AngelScript/Utils/ClientUtils.as"

class ScreenPressesHandler : ScriptObject
{
    protected Node @GetFirstReplicatedParentOf (Node @localNode)
    {
        Node @scanningNode = localNode;
        while (scanningNode !is null and scanningNode.id >= FIRST_LOCAL_ID)
        {
            scanningNode = scanningNode.parent;
        }
        return scanningNode;
    }

    protected void UnitSelected (Unit @unit, Node @scriptMain)
    {
        scriptMain.vars ["selectionType"] = StringHash ("Unit");
        scriptMain.vars ["selectedHash"] = unit.hash;

        // Inform map mask updater about selection.
        MapMaskUpdater @mapMaskUpdater = scene.GetComponent ("MapMaskUpdater");
        mapMaskUpdater.selectedDistrictHash = StringHash (0);
    }

    protected void DistrictSelected (District @district, Node @scriptMain)
    {
        scriptMain.vars ["selectionType"] = StringHash ("District");
        scriptMain.vars ["selectedHash"] = district.hash;

        // Inform map mask updater about selection.
        MapMaskUpdater @mapMaskUpdater = scene.GetComponent ("MapMaskUpdater");
        mapMaskUpdater.selectedDistrictHash = district.hash;
    }

    protected void ClearSelection (Node @scriptMain)
    {
        scriptMain.vars ["selectionType"] = StringHash ("None");
        scriptMain.vars ["selectedHash"] = StringHash ();
        scriptMain.vars ["currentClickCommand"] = StringHash ("NoCommand");

        // Inform map mask updater about selection.
        MapMaskUpdater @mapMaskUpdater = scene.GetComponent ("MapMaskUpdater");
        mapMaskUpdater.selectedDistrictHash = StringHash (0);
    }

    protected void SetUnitMoveTarget (Unit @unit, District @target)
    {
        Node @scriptMain = GetScriptMain (node);
        if (unit.unitType != UNIT_COLONIZATORS and unit.unitType != UNIT_TRADERS)
        {
            VectorBuffer buffer = VectorBuffer ();
            buffer.WriteInt (PLAYER_ACTION_SET_UNIT_MOVE_TARGET);
            buffer.WriteStringHash (unit.hash);
            buffer.WriteStringHash (target.hash);

            VariantMap eventData;
            eventData ["taskType"] = Variant (CTS_NETWORK_MESSAGE_SEND_PLAYER_ACTION);
            eventData ["messageBuffer"] = Variant (buffer);
            SendEvent ("NewNetworkTask", eventData);
        }
        scriptMain.vars ["currentClickCommand"] = StringHash ("NoCommand");
    }

    protected void SendBuildWarShipAction (District @requester, District @buildTarget)
    {
        Node @scriptMain = GetScriptMain (node);
        GameConfiguration @configuration = scene.GetComponent ("GameConfiguration");

        String playerName = scriptMain.vars ["playerName"].GetString ();
        float playerGold = scriptMain.vars ["gold"].GetFloat ();

        if (requester.hasColony and requester.colonyOwnerName == playerName and
            requester.menCount > configuration.oneWarShipCrew and
            requester.IsNeighborsWith (buildTarget.hash) and playerGold > configuration.oneWarShipBuildingCost)
        {
            VariantMap actionData;
            actionData [ColonyActions_BuildWarShip_TARGET_DISTRICT] = Variant (buildTarget.hash);

            VectorBuffer buffer = VectorBuffer ();
            buffer.WriteInt (PLAYER_ACTION_ADD_COLONY_ACTION);
            buffer.WriteStringHash (requester.hash);
            buffer.WriteStringHash (ColonyActions_BUILD_WAR_SHIP);
            buffer.WriteVariantMap (actionData);

            VariantMap eventData;
            eventData ["taskType"] = Variant (CTS_NETWORK_MESSAGE_SEND_PLAYER_ACTION);
            eventData ["messageBuffer"] = Variant (buffer);
            SendEvent ("NewNetworkTask", eventData);
        }
        scriptMain.vars ["currentClickCommand"] = StringHash ("NoCommand");
    }

    protected void ProcessDistrictSelection (Vector3 hitPosition, Node @scriptMain)
    {
        MapMaskUpdater @mapMaskUpdater = scene.GetComponent ("MapMaskUpdater");
        StringHash districtHash = mapMaskUpdater.GetDistrictByPoint (hitPosition);

        Map @map = scene.GetChild ("map").GetComponent ("Map");
        District @district = map.GetDistrictByHash (districtHash);

        if (district !is null)
        {
            StringHash command = scriptMain.vars ["currentClickCommand"].GetStringHash ();
            if (command == StringHash ("NoCommand"))
            {
                DistrictSelected (district, scriptMain);
            }

            else if (command == StringHash ("MoveUnit"))
            {
                // Get selected unit.
                StringHash selectedHash = scriptMain.vars ["selectedHash"].GetStringHash ();
                Unit @unit = GetUnitByHash (scene, selectedHash);
                if (unit !is null)
                {
                    SetUnitMoveTarget (unit, district);
                }
                // If unit no longer exists, select district instead.
                else
                {
                    DistrictSelected (district, scriptMain);
                }
            }

            else if (command == StringHash ("BuildWarShip"))
            {
                // Get selected unit.
                StringHash selectedHash = scriptMain.vars ["selectedHash"].GetStringHash ();
                District @requesterDistrict = map.GetDistrictByHash (selectedHash);
                SendBuildWarShipAction (requesterDistrict, district);
            }
        }
        else
        {
            ClearSelection (scriptMain);
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
        Node @scriptMain = GetScriptMain (node);
        scriptMain.vars ["currentClickCommand"] = StringHash ("NoCommand");
    }

    void Update (float timeStep)
    {

    }

    void Stop ()
    {

    }

    void HandleScreenPress (StringHash eventType, VariantMap &eventData)
    {
        Node @scriptMain = GetScriptMain (node);
        if (eventData ["Element"].GetPtr () is null && scriptMain.vars ["gameState"].GetInt () != GAME_STATE_WAITING_FOR_START)
        {
            Camera @camera = scene.GetChild ("camera").GetComponent ("Camera");
            Ray ray = camera.GetScreenRay (eventData ["X"].GetInt () * 1.0f / graphics.width,
                                           eventData ["Y"].GetInt () * 1.0f / graphics.height);
            RayQueryResult result = octree.RaycastSingle (ray, RAY_TRIANGLE, DEFAULT_RAYCAST_RAY_LENGTH, DRAWABLE_GEOMETRY);

            if (result.node !is null)
            {
                Node @firstReplicated = GetFirstReplicatedParentOf (result.node);
                if (firstReplicated !is scene)
                {
                    if (firstReplicated.HasComponent ("Unit"))
                    {
                        UnitSelected (firstReplicated.GetComponent ("Unit"), scriptMain);
                    }
                    else
                    {
                        ProcessDistrictSelection (result.position, scriptMain);
                    }
                }
                else
                {
                    ClearSelection (scriptMain);
                }
            }
            else
            {
                ClearSelection (scriptMain);
            }
        }
    }
};
