class SceneManager : ScriptObject
{
    protected float CAMERA_MOVE_SPEED = 2.5f;
    protected Node @cameraNode_;
    protected bool isSceneLoaded_;
    protected float beforeDistrictsNamesUpdate_;
    
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
            if (beforeDistrictsNamesUpdate_ <= 0.0f)
            {
                UpdateDistrictsNames ();
                beforeDistrictsNamesUpdate_ = 1.0f;
            }
        }
    }
    
    void Stop ()
    {
        
    }
};
