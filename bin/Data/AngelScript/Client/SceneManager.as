#include "AngelScript/Utils/CheckIsSceneLoaded.as"

class SceneManager : ScriptObject
{
    protected float CAMERA_MOVE_SPEED = 2.5f;
    protected Node @cameraNode_;
    protected Node @lightNode_;
    protected bool isSceneLoaded_;
    protected bool renderPathUpdaterWillBeCreated_;
    protected float untilDistrictsUpdate_;
    protected float untilUnitsUpdate_;

    protected void LoadPrefabOf (Node @replicatedNode, bool asChild, String name, String overridePrefabPath = "")
    {
        Node @localNode;
        if (asChild)
            localNode = replicatedNode.CreateChild ("local", LOCAL);
        else
            localNode = scene.GetChild ("locals").CreateChild (replicatedNode.name, LOCAL);

        String prefabPath;
        if (overridePrefabPath.empty)
            prefabPath = replicatedNode.vars ["PrefabXMLPath"].GetString ();
        else
            prefabPath = overridePrefabPath;

        XMLFile @prefabXML = cache.GetResource ("XMLFile", prefabPath);
        localNode.LoadXML (prefabXML.root);
        localNode.name = name;
    }

    protected void UpdateDistricts ()
    {
        Map @map = scene.GetChild ("map").GetComponent ("Map");
        Array <Node @> districtsNodes = scene.GetChild ("map").GetChildrenWithComponent ("District");

        for (int index = 0; index < districtsNodes.length; index++)
        {
            Node @districtNode = districtsNodes [index];
            District @district = districtNode.GetComponent ("District");

            if (district.hasColony)
            {
                if (districtNode.GetChild ("local") is null)
                    LoadPrefabOf (districtNode, true, "local", "Objects/ColonyLocal.xml");
                district.node.GetChild ("local").enabled = true;

                Vector3 colonyPosition = district.colonyPosition;
                districtNode.GetChild ("local").position = colonyPosition;
            }
            else if (district.node.GetChild ("local") !is null)
            {
                district.node.GetChild ("local").enabled = false;
            }
        }
    }

    protected void PlaceUnit (Unit @unit, Map @map)
    {
        if (unit.node.GetChild ("local") is null)
        {
            // TODO: Create prefabs for all unit types, not only for fleets.
            if (unit.unitType == UNIT_FLEET)
                LoadPrefabOf (unit.node, true, "local", "Objects/ShipLocal.xml");
            else if (unit.unitType == UNIT_TRADERS)
                LoadPrefabOf (unit.node, true, "local", "Objects/ShipLocal.xml");
            else if (unit.unitType == UNIT_COLONIZATORS)
                LoadPrefabOf (unit.node, true, "local", "Objects/ShipLocal.xml");
            else if (unit.unitType == UNIT_ARMY)
                LoadPrefabOf (unit.node, true, "local", "Objects/ShipLocal.xml");
        }

        unit.node.GetChild ("local").enabled = true;
        District @district = map.GetDistrictByHash (unit.positionHash);
        unit.node.GetChild ("local").worldPosition = district.unitPosition;

        if (unit.GetWay ().length > 0)
        {
            Array <StringHash> unitWay = unit.GetWay ();
            District @nextWaypoint = map.GetDistrictByHash (unitWay [0]);
            unit.node.GetChild ("local").LookAt (nextWaypoint.unitPosition);
            Quaternion rotation = unit.node.GetChild ("local").worldRotation;
            unit.node.GetChild ("local").worldRotation = Quaternion (0.0f, rotation.yaw, 0.0f);
        }
        else
            unit.node.GetChild ("local").worldRotation = Quaternion ();
    }

    protected void UpdateUnits ()
    {
        Map @map = scene.GetChild ("map").GetComponent ("Map");
        Array <Node @> unitsNodes = scene.GetChild ("units").GetChildrenWithComponent ("Unit");
        VariantMap isDistrictOccupied;

        StringHash selectionType = node.parent.GetChild ("screenPressesHandlerScriptNode").
                                    vars ["selectionType"].GetStringHash ();
        StringHash unitHash;
        // Firstly place selected unit (if any unit selected).
        if (selectionType == StringHash ("Unit") and unitsNodes.length > 0)
        {
            unitHash = node.parent.GetChild ("screenPressesHandlerScriptNode").
                                    vars ["selectedHash"].GetStringHash ();
            Unit @unit = unitsNodes [0].GetComponent ("Unit");
            int index = 1;
            while (unit.hash != unitHash and index < unitsNodes.length)
            {
                unit = unitsNodes [index].GetComponent ("Unit");
                index++;
            }

            isDistrictOccupied [unit.positionHash] = true;
            PlaceUnit (unit, map);
        }

        for (int index = 0; index < unitsNodes.length; index++)
        {
            Unit @unit = unitsNodes [index].GetComponent ("Unit");
            if (!isDistrictOccupied [unit.positionHash].GetBool ())
            {
                isDistrictOccupied [unit.positionHash] = true;
                PlaceUnit (unit, map);
            }
            else if (unit.hash != unitHash and unit.node.GetChild ("local") !is null)
            {
                unit.node.GetChild ("local").enabled = false;
            }
        }
    }

    protected void CreateLocalLight ()
    {
        lightNode_ = scene.CreateChild ("light", LOCAL);
        lightNode_.rotation = Quaternion (65.0f, 0.0f, 0.0f);
        lightNode_.position = Vector3 (0.0f, 0.0f, 0.0f);

        Light @light = lightNode_.CreateComponent ("Light", LOCAL);
        light.castShadows = true;
        light.lightType = LIGHT_DIRECTIONAL;
    }

    protected void CreateLocalCamera ()
    {
        cameraNode_ = scene.CreateChild ("camera", LOCAL);
        cameraNode_.rotation = Quaternion (90.0f, 0.0f, 0.0f);
        cameraNode_.position = Vector3 (2.5f, 15.0f, 2.5f);

        cameraNode_.CreateComponent ("SoundListener", LOCAL);
        Camera @camera = cameraNode_.CreateComponent ("Camera", LOCAL);
        camera.farClip = 100.0f;

        Viewport @viewport = Viewport (scene, cameraNode_.GetComponent ("Camera"));
        renderer.viewports [0] = viewport;
    }

    protected void CreateFogOfWarProcessors ()
    {
        FogOfWarCalculator @fogOfWarCalculator = scene.CreateComponent ("FogOfWarCalculator", LOCAL);
        fogOfWarCalculator.playerName = node.parent.vars ["playerName"].GetString ();

        Node @mapNode = scene.GetChild ("map");
        MapMaskUpdater @mapMaskUpdater = scene.CreateComponent ("MapMaskUpdater", LOCAL);
        mapMaskUpdater.mapMinPoint = mapNode.vars ["mapMinPoint"].GetVector3 ();
        mapMaskUpdater.mapMaxPoint = mapNode.vars ["mapMaxPoint"].GetVector3 ();
    }

    protected void CreateRenderPathUpdater ()
    {
        Node @renderPathUpdateScriptNode = node.parent.CreateChild ("renderPathUpdaterScriptNode", LOCAL);
        ScriptInstance @renderPathUpdaterScript = renderPathUpdateScriptNode.CreateComponent ("ScriptInstance", LOCAL);
        renderPathUpdaterScript.CreateObject (cache.GetResource ("ScriptFile",
                                                         "AngelScript/Client/RenderPathUpdater.as"),
                                              "RenderPathUpdater");
    }

    protected void UpdateCameraPositionByKeyboardInput (float timeStep)
    {
        Vector3 positionDelta;
        if (input.keyDown [KEY_A])
            positionDelta.x -= CAMERA_MOVE_SPEED;
        if (input.keyDown [KEY_D])
            positionDelta.x += CAMERA_MOVE_SPEED;

        if (input.keyDown [KEY_S])
            positionDelta.z -= CAMERA_MOVE_SPEED;
        if (input.keyDown [KEY_W])
            positionDelta.z += CAMERA_MOVE_SPEED;

        positionDelta = positionDelta * timeStep;
        cameraNode_.position = cameraNode_.position + positionDelta;
    }

    SceneManager ()
    {
        isSceneLoaded_ = false;
        untilDistrictsUpdate_ = 0.001f;
        untilUnitsUpdate_ = 0.001f;
    }

    ~SceneManager ()
    {

    }

    void Start ()
    {
        scene.CreateChild ("locals", LOCAL);
    }

    void Update (float timeStep)
    {
        if (!isSceneLoaded_)
            isSceneLoaded_ = CheckIsSceneLoaded (scene);
        else
        {
            if (renderPathUpdaterWillBeCreated_)
            {
                MapMaskUpdater @mapMaskUpdater = scene.GetComponent ("MapMaskUpdater");
                mapMaskUpdater.RecalculateMaskImage ();

                CreateRenderPathUpdater ();
                renderPathUpdaterWillBeCreated_ = false;
            }

            if (scene.GetChild ("map").GetChild ("local") is null)
            {
                LoadPrefabOf (scene.GetChild ("map"), true, "local");
                CreateLocalLight ();
                CreateLocalCamera ();
                CreateFogOfWarProcessors ();
                renderPathUpdaterWillBeCreated_ = true;
            }

            if (cameraNode_ !is null)
                UpdateCameraPositionByKeyboardInput (timeStep);

            untilDistrictsUpdate_ -= timeStep;
            untilUnitsUpdate_ -= timeStep;

            if (untilDistrictsUpdate_ <= 0.0f)
            {
                UpdateDistricts ();
                untilDistrictsUpdate_ = 1.0f;
            }

            if (untilUnitsUpdate_ <= 0.0f)
            {
                UpdateUnits ();
                untilUnitsUpdate_ = 0.1f;
            }
        }
    }

    void Stop ()
    {

    }
};
