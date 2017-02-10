#include "AngelScript/Utils/ClientUtils.as"

class SceneManager : ScriptObject
{
    protected Node @cameraNode_;
    protected Node @lightNode_;
    protected bool isSceneLoaded_;
    protected bool renderPathUpdaterWillBeCreated_;
    protected float untilDistrictsUpdate_;
    protected float untilUnitsUpdate_;

    protected float CAMERA_MOVE_SPEED = 2.5f;
    protected Quaternion SUN_LIGHT_ROTATION = Quaternion (65.0f, 0.0f, 0.0f);
    protected Vector3 CAMERA_DEFAULT_POSITION = Vector3 (2.5f, 8.0f, 2.5f);
    protected Quaternion CAMERA_DEFAULT_ROTATION = Quaternion (50.0f, 0.0f, 0.0f);
    protected float CAMERA_DEFAULT_FAR_CLIP = 100.0f;

    protected float DISTRICTS_UPDATE_DELAY = 1.0f;
    protected float UNITS_UPDATE_DELAY = 0.1f;
    protected int KEY_GO_LEFT = KEY_A;
    protected int KEY_GO_RIGHT = KEY_D;
    protected int KEY_GO_FORWARD = KEY_W;
    protected int KEY_GO_BACK = KEY_S;

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
            unit.node.GetChild ("local").worldRotation = Quaternion (0.0f, 180.0f + rotation.yaw, 0.0f);
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
        StringHash selectedUnitHash;
        // Firstly place selected unit (if any unit selected).
        if (selectionType == StringHash ("Unit") and unitsNodes.length > 0)
        {
            selectedUnitHash = node.parent.vars ["selectedHash"].GetStringHash ();
            Unit @unit = GetUnitByHash (scene, selectedUnitHash);
            if (unit !is null)
            {
                isDistrictOccupied [unit.positionHash] = true;
                PlaceUnit (unit, map);
            }
        }

        for (int index = 0; index < unitsNodes.length; index++)
        {
            Unit @unit = unitsNodes [index].GetComponent ("Unit");
            if (!isDistrictOccupied [unit.positionHash].GetBool ())
            {
                isDistrictOccupied [unit.positionHash] = true;
                PlaceUnit (unit, map);
            }
            else if (unit.hash != selectedUnitHash and unit.node.GetChild ("local") !is null)
            {
                unit.node.GetChild ("local").enabled = false;
            }
        }
    }

    protected void CreateLocalLight ()
    {
        lightNode_ = scene.CreateChild ("light", LOCAL);
        lightNode_.rotation = SUN_LIGHT_ROTATION;
        lightNode_.position = Vector3 (0.0f, 0.0f, 0.0f);

        Light @light = lightNode_.CreateComponent ("Light", LOCAL);
        light.castShadows = true;
        light.lightType = LIGHT_DIRECTIONAL;
    }

    protected void CreateLocalCamera ()
    {
        cameraNode_ = scene.CreateChild ("camera", LOCAL);
        cameraNode_.position = CAMERA_DEFAULT_POSITION;
        cameraNode_.rotation = CAMERA_DEFAULT_ROTATION;

        cameraNode_.CreateComponent ("SoundListener", LOCAL);
        Camera @camera = cameraNode_.CreateComponent ("Camera", LOCAL);
        camera.farClip = CAMERA_DEFAULT_FAR_CLIP;

        Viewport @viewport = Viewport (scene, camera);
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
        if (not ui.root.GetChild ("ingame").GetChild ("chatWindow").GetChild ("messageEdit").focus)
        {
            Vector3 positionDelta;
            if (input.keyDown [KEY_GO_LEFT])
                positionDelta.x -= CAMERA_MOVE_SPEED;
            if (input.keyDown [KEY_GO_RIGHT])
                positionDelta.x += CAMERA_MOVE_SPEED;

            if (input.keyDown [KEY_GO_BACK])
                positionDelta.z -= CAMERA_MOVE_SPEED;
            if (input.keyDown [KEY_GO_FORWARD])
                positionDelta.z += CAMERA_MOVE_SPEED;

            positionDelta = positionDelta * timeStep;
            cameraNode_.worldPosition = cameraNode_.worldPosition + positionDelta;
        }
    }

    SceneManager ()
    {
        isSceneLoaded_ = false;
        untilDistrictsUpdate_ = 0.0f;
        untilUnitsUpdate_ = 0.0f;
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
                untilDistrictsUpdate_ = DISTRICTS_UPDATE_DELAY;
            }

            if (untilUnitsUpdate_ <= 0.0f)
            {
                UpdateUnits ();
                untilUnitsUpdate_ = UNITS_UPDATE_DELAY;
            }
        }
    }

    void Stop ()
    {

    }
};
