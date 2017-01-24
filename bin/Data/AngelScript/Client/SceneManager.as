#include "AngelScript/Client/CheckIsSceneLoaded.as"

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

            if (districtNode.GetChild ("localName") is null)
                LoadPrefabOf (districtNode, true, "localName", "Objects/DistrictNameLocal.xml");

            Vector3 position = district.colonyPosition;
            position.z += Abs (district.unitPosition.z - district.colonyPosition.z) / 2;
            districtNode.GetChild ("localName").worldPosition = position;
            Text3D @text = districtNode.GetChild ("localName").GetChild ("text").GetComponent ("Text3D");
            text.text = district.name;

            if (district.hasColony)
            {
                if (districtNode.GetChild ("localColony") is null)
                    LoadPrefabOf (districtNode, true, "localColony", "Objects/ColonyIconLocal.xml");

                Vector3 colonyPosition = district.colonyPosition;
                districtNode.GetChild ("localColony").position = colonyPosition;
                Text3D @colonyText = districtNode.GetChild ("localColony").GetChild ("playerText").GetComponent ("Text3D");
                colonyText.text = district.colonyOwnerName;
            }
        }
    }

    protected void UpdateUnits ()
    {
        Map @map = scene.GetChild ("map").GetComponent ("Map");
        Array <Node @> unitsNodes = scene.GetChild ("units").GetChildrenWithComponent ("Unit");
        VariantMap unitsInDistrictsCounts;
        VariantMap placedUnitsInDistrictsCounts;

        for (int index = 0; index < unitsNodes.length; index++)
        {
            Unit @unit = unitsNodes [index].GetComponent ("Unit");
            unitsInDistrictsCounts [unit.positionHash] =
                unitsInDistrictsCounts [unit.positionHash].GetInt () + 1;
        }

        for (int index = 0; index < unitsNodes.length; index++)
        {
            Node @unitNode = unitsNodes [index];
            Unit @unit = unitNode.GetComponent ("Unit");

            if (unitNode.GetChild ("local") is null)
            {
                if (unit.unitType == UNIT_FLEET)
                    LoadPrefabOf (unitNode, true, "local", "Objects/FleetUnitLocal.xml");
                else if (unit.unitType == UNIT_TRADERS)
                    LoadPrefabOf (unitNode, true, "local", "Objects/TradersUnitLocal.xml");
                else if (unit.unitType == UNIT_COLONIZATORS)
                    LoadPrefabOf (unitNode, true, "local", "Objects/ColonizatorsUnitLocal.xml");
                else if (unit.unitType == UNIT_ARMY)
                    LoadPrefabOf (unitNode, true, "local", "Objects/ArmyUnitLocal.xml");
            }

            int unitsCount = unitsInDistrictsCounts [unit.positionHash].GetInt ();
            int placedUnitsCount = placedUnitsInDistrictsCounts [unit.positionHash].GetInt ();
            placedUnitsInDistrictsCounts [unit.positionHash] = placedUnitsCount + 1;

            float placeAmplitude = 0.25f;
            float placeStep = 2 * placeAmplitude / unitsCount;
            Vector3 position = map.GetDistrictByHash (unit.positionHash).unitPosition;
            position.x += placedUnitsCount * placeStep - placeAmplitude;
            position.y += placedUnitsCount * 0.035f;
            unitNode.GetChild ("local").worldPosition = position;

            Text3D @text = unitNode.GetChild ("local").GetChild ("playerText").GetComponent ("Text3D");
            text.text = unit.ownerPlayerName;
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
        cameraNode_.rotation = Quaternion (45, 0, 0);
        cameraNode_.position = Vector3 (2.5f, 4.0f, 2.5f);

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
