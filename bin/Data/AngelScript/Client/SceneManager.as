class SceneManager : ScriptObject
{
    protected float CAMERA_MOVE_SPEED = 2.5f;
    protected Node @cameraNode_;
    protected bool isSceneLoaded_;
    protected float beforeDistrictsNamesUpdate_;
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
    
    protected void LoadPrefabOf (Node @replicatedNode, bool asChild, String overridePrefabPath = "")
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
        localNode.name = replicatedNode.name;
    }
    
    protected void UpdateDistrictsNames ()
    {
        Array <Node @> districtsNodes = scene.GetChild ("map").GetChildren ();
        for (int index = 0; index < districtsNodes.length; index++)
        {
            Node @districtNode = districtsNodes [index];
            if (districtNode.GetChild ("local") is null)
            {
                LoadPrefabOf (districtNode, true, "Objects/DistrictNameLocal.xml");
                districtNode.GetChildren () [0].name = "local";
            }
            Vector3 position = districtNode.vars ["colonyPosition"].GetVector3 ();
            districtNode.GetChild ("local").position = position;
            Text3D @text = districtNode.GetChild ("local").GetComponent ("Text3D");
            text.text = districtNode.name;
        }
    }
    
    protected void UpdateUnits ()
    {
        Map @map = node.parent.vars ["map"].GetPtr ();
        UnitsContainer @unitsContainer = node.parent.vars ["unitsContainer"].GetPtr ();
        VariantMap unitsInDistrictsCounts;
        VariantMap placedUnitsInDistrictsCounts;
        
        for (int index = 0; index < unitsContainer.GetUnitsCount(); index++)
        {
            Unit @unit = unitsContainer.GetUnitByIndex (index);
            unitsInDistrictsCounts [unit.position_.name_] = 
                unitsInDistrictsCounts [unit.position_.name_].GetInt () + 1;
        }
        
        Array <Node @> unitsNodes = scene.GetChild ("units").GetChildren ();
        for (int index = 0; index < unitsContainer.GetUnitsCount(); index++)
        {
            Unit @unit = unitsContainer.GetUnitByIndex (index);
            Node @unitNode = unitsNodes [index];
            
            if (unitNode.GetChild ("local") is null)
            {
                if (unit.unitType_ == UNIT_FLEET)
                    LoadPrefabOf (unitNode, true, "Objects/FleetUnitLocal.xml");
                else if (unit.unitType_ == UNIT_TRADERS)
                    LoadPrefabOf (unitNode, true, "Objects/TradersUnitLocal.xml");
                else if (unit.unitType_ == UNIT_COLONIZATORS)
                    LoadPrefabOf (unitNode, true, "Objects/ColonizatorsUnitLocal.xml");
                else if (unit.unitType_ == UNIT_ARMY)
                    LoadPrefabOf (unitNode, true, "Objects/ArmyUnitLocal.xml");
                unitNode.GetChildren () [0].name = "local";
            }
            
            int unitsCount = unitsInDistrictsCounts [unit.position_.name_].GetInt ();
            int placedUnitsCount = placedUnitsInDistrictsCounts [unit.position_.name_].GetInt ();
            placedUnitsInDistrictsCounts [unit.position_.name_] =
                placedUnitsInDistrictsCounts [unit.position_.name_].GetInt () + 1;
                
            float placeAmplitude = 0.25f;
            float placeStep = 2 * placeAmplitude / unitsCount;
            Vector3 position = unit.position_.unitPosition_;  
            position.x += placedUnitsCount * placeStep - placeAmplitude;
            position.y += placedUnitsCount * 0.04f;
            unitNode.GetChild ("local").position = position;
                
            Text3D @text = unitNode.GetChild ("local").GetChild ("playerText").GetComponent ("Text3D");
            text.text = unit.ownerPlayer_;
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
        beforeDistrictsNamesUpdate_ = 0.001f;
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
            LoadPrefabOf (scene.GetChild ("map"), false);
            CreateLocalCamera ();
        }
        
        if (cameraNode_ !is null)
            UpdateCameraPositionByKeyboardInput (timeStep);
            
        if (isSceneLoaded_)
        {
            beforeDistrictsNamesUpdate_ -= timeStep;
            beforeUnitsUpdate_ -= timeStep;
            
            if (beforeDistrictsNamesUpdate_ <= 0.0f)
            {
                UpdateDistrictsNames ();
                beforeDistrictsNamesUpdate_ = 2.0f;
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
