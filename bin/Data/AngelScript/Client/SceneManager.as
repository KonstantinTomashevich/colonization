class SceneManager : ScriptObject
{
    protected float CAMERA_MOVE_SPEED = 2.5f;
    protected Node @cameraNode_;
    
    protected void LoadPrefabOf (Node @replicatedNode)
    {
        Node @localNode = scene.GetChild ("locals").CreateChild ("newLocal", LOCAL);
        XMLFile @prefabXML = cache.GetResource ("XMLFile", 
                                                replicatedNode.vars ["PrefabXMLPath"].GetString ());
        localNode.LoadXML (prefabXML.root);
        localNode.name = replicatedNode.name;
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
        if (scene.GetChild ("map") !is null and scene.GetChild ("locals").GetChild ("map") is null)
        {
            LoadPrefabOf (scene.GetChild ("map"));
            CreateLocalCamera ();
        }
        
        if (cameraNode_ !is null)
            UpdateCameraPositionByKeyboardInput (timeStep);
    }
    
    void Stop ()
    {
        
    }
};
