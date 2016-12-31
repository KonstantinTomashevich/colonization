class SceneManager : ScriptObject
{
    protected float CAMERA_MOVE_SPEED = 2.5f;
    protected Node @cameraNode_;
    protected bool isSceneLoaded_;
    protected float beforeDistrictsUpdate_;
    protected float beforeUnitsUpdate_;
    
    protected void CheckIsSceneLoaded ()
    {
        if (scene.vars ["ReplicatedNodesCount"].GetInt () != 0)
        {
            Array <Node @> children = scene.GetChildren (true);
            int replicated = 0;
            for (int index = 0; index < children.length; index++)
                if (children [index].id < FIRST_LOCAL_ID)
                    replicated++;
            isSceneLoaded_ = (replicated == scene.vars ["ReplicatedNodesCount"].GetInt ());
        }
        else
            isSceneLoaded_ = false;
    }
    
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
                    LoadPrefabOf (districtNode, true, "localColony", "Objects/ColonyLocal.xml");
                
                Vector3 colonyPosition = district.colonyPosition;
                districtNode.GetChild ("localColony").position = colonyPosition;
                Text3D @text = districtNode.GetChild ("localColony").GetChild ("playerText").GetComponent ("Text3D");
                text.text = district.colonyOwnerName;
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
    
    protected void CreateLocalCamera ()
    {
        cameraNode_ = scene.CreateChild ("camera", LOCAL);
        cameraNode_.rotation = Quaternion (90, 0, 0);
        cameraNode_.position = Vector3 (2.5f, 2.5f, 2.5f);
        
        cameraNode_.CreateComponent ("SoundListener", LOCAL);
        Camera @camera = cameraNode_.CreateComponent ("Camera", LOCAL);
        camera.farClip = 50.0f;
        
        Viewport @viewport = Viewport (scene, cameraNode_.GetComponent ("Camera"));
        renderer.viewports [0] = viewport;
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
        beforeDistrictsUpdate_ = 0.001f;
        beforeUnitsUpdate_ = 0.001f;
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
            CheckIsSceneLoaded ();
            
        if (isSceneLoaded_ and scene.GetChild ("locals").GetChild ("map") is null)
        {
            LoadPrefabOf (scene.GetChild ("map"), false, "map");
            CreateLocalCamera ();
        }
        
        if (cameraNode_ !is null)
            UpdateCameraPositionByKeyboardInput (timeStep);
            
        if (isSceneLoaded_)
        {
            beforeDistrictsUpdate_ -= timeStep;
            beforeUnitsUpdate_ -= timeStep;
            
            if (beforeDistrictsUpdate_ <= 0.0f)
            {
                UpdateDistricts ();
                beforeDistrictsUpdate_ = 1.0f;
            }
            
            if (beforeUnitsUpdate_ <= 0.0f)
            {
                UpdateUnits ();
                beforeUnitsUpdate_ = 0.1f;
            }
        }
    }
    
    void Stop ()
    {
        
    }
};
