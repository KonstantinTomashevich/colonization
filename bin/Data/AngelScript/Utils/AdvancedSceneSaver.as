class AdvancedSceneSaver : ScriptObject
{
    String path_;
    bool saveSceneReplicatedComponents_;
    bool saveSceneLocalComponents_;
    bool saveReplicatedNodes_;
    bool saveLocalNodes_;
    bool runOnNextFrame_;
    
    protected void ProcessNode (Node @nodeToProcess, Node @newParent)
    {
        CreateMode createMode;
        if (nodeToProcess.id < FIRST_LOCAL_ID)
            createMode = REPLICATED;
        else
            createMode = LOCAL;
        Node @newNode = nodeToProcess.Clone (createMode);
        newNode.RemoveAllChildren ();
        uint idCopy = newNode.id;
        newParent.AddChild (newNode);
        newNode.id = idCopy;
        
        Array <Node @> currentChildren = nodeToProcess.GetChildren (false);
        for (int index = 0; index < currentChildren.length; index++)
        {
            Node @currentNode = currentChildren [index];
            if (not currentNode.temporary and 
                ((currentNode.id < FIRST_LOCAL_ID and saveReplicatedNodes_) or
                (currentNode.id >= FIRST_LOCAL_ID and saveLocalNodes_)))
                ProcessNode (currentNode, newNode);
        }
    }
    
    AdvancedSceneSaver ()
    {
        path_ = "<Path to save scene xml>";
        saveSceneReplicatedComponents_ = true;
        saveSceneLocalComponents_ = false;
        saveReplicatedNodes_ = true;
        saveLocalNodes_ = false;
        runOnNextFrame_ = false;
    }
    
    ~AdvancedSceneSaver ()
    {
        
    }
    
    void Start ()
    {
        
    }
    
    void Update (float timeStep)
    {
        if (runOnNextFrame_)
        {
            Scene @newScene = Scene ();
            newScene.name = scene.name;
            newScene.timeScale = scene.timeScale;
            newScene.smoothingConstant = scene.smoothingConstant;
            newScene.snapThreshold = scene.snapThreshold;
            
            for (int index = 0; index < scene.vars.length; index++)
                newScene.vars [scene.vars.keys [index]] = scene.vars.values [index];
            newScene.SetAttribute ("Variable Names", scene.GetAttribute ("Variable Names"));
                
            for (int index = 0; index < scene.tags.length; index++)
                newScene.AddTag (scene.tags [index]);
            
            Array <Node @> children = scene.GetChildren (false);
            for (int index = 0; index < children.length; index++)
            {
                Node @currentNode = children [index];
                if (not currentNode.temporary and 
                    ((currentNode.id < FIRST_LOCAL_ID and saveReplicatedNodes_) or
                    (currentNode.id >= FIRST_LOCAL_ID and saveLocalNodes_)))
                    ProcessNode (currentNode, newScene);
            }
            
            Array <Component @> components = scene.GetComponents ();
            for (int index = 0; index < components.length; index++)
            {
                Component @currentComponent = components [index];
                CreateMode createMode;
                if (currentComponent.id < FIRST_LOCAL_ID)
                    createMode = REPLICATED;
                else
                    createMode = LOCAL;
                
                if (not currentComponent.temporary and
                    ((currentComponent.id < FIRST_LOCAL_ID and saveSceneReplicatedComponents_) or
                     (currentComponent.id >= FIRST_LOCAL_ID and saveSceneLocalComponents_)))
                    newScene.CloneComponent (currentComponent, createMode, currentComponent.id);
            }
            
            File file (path_, FILE_WRITE);
            newScene.SaveXML (file);
            file.Close ();                
            runOnNextFrame_ = false;
        }
    }
    
    void Stop ()
    {
        
    }
};
