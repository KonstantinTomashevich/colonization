class Player : ScriptObject
{
    protected LauncherApplication @launcherApplication_;
    protected String playerName_;
    protected float beforeMapUpdate_;
    protected float beforeMapNeighborsUpdate_;
    protected float beforeUnitsUpdate_;
    protected Map @mapPtr_;
    protected UnitsContainer @unitsContainerPtr_;
    protected bool isSceneLoaded_;
    
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
    
    Player ()
    {
        beforeMapUpdate_ = 0.001f;
        beforeMapNeighborsUpdate_ = 0.001f;
        beforeUnitsUpdate_ = 0.001f;
        isSceneLoaded_ = false;
    }
    
    ~Player ()
    {
        
    }
    
    void Init ()
    {
        
    }
    
    void Start ()
    {
        XMLFile@ style = cache.GetResource ("XMLFile", "UI/DefaultStyle.xml");
        ui.root.defaultStyle = style;
        
        Node @uiScriptNode = node.CreateChild ("uiScriptNode", LOCAL);
        ScriptInstance @uiScript = uiScriptNode.CreateComponent ("ScriptInstance", LOCAL);
        uiScript.CreateObject (cache.GetResource ("ScriptFile",
                                                         "AngelScript/Client/PlayerUi.as"),
                                     "PlayerUi");
                                     
        Node @networkScriptNode = node.CreateChild ("networkScriptNode", LOCAL);
        ScriptInstance @networkScript = networkScriptNode.CreateComponent ("ScriptInstance", LOCAL);
        networkScript.CreateObject (cache.GetResource ("ScriptFile",
                                                         "AngelScript/Client/PlayerNetwork.as"),
                                     "PlayerNetwork");
                                     
        Node @sceneManagerScriptNode = node.CreateChild ("sceneManagerScriptNode", LOCAL);
        ScriptInstance @sceneManagerScript = sceneManagerScriptNode.CreateComponent ("ScriptInstance", LOCAL);
        sceneManagerScript.CreateObject (cache.GetResource ("ScriptFile",
                                                         "AngelScript/Client/SceneManager.as"),
                                     "SceneManager");
        
        Node @screenPressesHandlerScriptNode = node.CreateChild ("screenPressesHandlerScriptNode", LOCAL);
        ScriptInstance @screenPressesHandlerScript = screenPressesHandlerScriptNode.CreateComponent ("ScriptInstance", LOCAL);
        screenPressesHandlerScript.CreateObject (cache.GetResource ("ScriptFile",
                                                         "AngelScript/Client/ScreenPressesHandler.as"),
                                     "ScreenPressesHandler");
        
        SubscribeToEvent ("ServerDisconnected", "HandleServerDisconnected");
        SubscribeToEvent ("ConnectFailed", "HandleConnectFailed");
        
        Map @map = Map ();
        AddRef (map);
        node.vars ["map"] = map;
        mapPtr_ = map;
        
        UnitsContainer @unitsContainer = UnitsContainer ();
        AddRef (unitsContainer);
        node.vars ["unitsContainer"] = unitsContainer;
        unitsContainerPtr_ = unitsContainer;
    }
    
    void Update (float timeStep)
    {
        if (node.vars ["goToMenuCalled"].GetBool ())
            GoToMainMenuState ();
            
        if (!isSceneLoaded_)
            CheckIsSceneLoaded ();
            
        if (isSceneLoaded_)
        {
            beforeMapUpdate_ -= timeStep;
            beforeMapNeighborsUpdate_ -= timeStep;
            beforeUnitsUpdate_ -= timeStep;
            
            if (beforeMapUpdate_ <= 0.0f)
            {
                mapPtr_.ReadDataFromNode (scene.GetChild ("map"));
                beforeMapUpdate_ = 1.0f;
            }
            
            if (beforeMapNeighborsUpdate_ <= 0.0f)
            {
                mapPtr_.UpdateNeighborsOfDistricts ();
                beforeMapNeighborsUpdate_ = 10.0f;
            }
            
            if (beforeUnitsUpdate_ <= 0.0f)
            {
                unitsContainerPtr_.ReadDataFromNode (scene.GetChild ("units"), mapPtr_);
                beforeUnitsUpdate_ = 0.1f;
            }
        }
    }
    
    void Stop ()
    {
        UnsubscribeFromAllEvents ();
        SetRefs (mapPtr_, 1);
        SetRefs (unitsContainerPtr_, 1);
    }
    
    LauncherApplication @get_launcherApplication ()
    {
        return launcherApplication_;
    }
    
    void set_launcherApplication (LauncherApplication @launcherApplication)
    {
        launcherApplication_ = launcherApplication;
    }
    
    String get_playerName ()
    {
        return playerName_;
    }
    
    void set_playerName (String playerName)
    {
        playerName_ = playerName;
        node.vars ["playerName"] = playerName_;
    }
    
    void HandleServerDisconnected ()
    {
        ErrorDialog ("Disconnected from server!", "Disconnected from server!\nPress OK to return to main menu.");
        GoToMainMenuState ();
    }
    
    void GoToMainMenuState ()
    {
        for (int index = 0; index < launcherApplication_.GetActivitiesCount (); index++)
            launcherApplication_.StopActivityNextFrame (launcherApplication_.GetActivityByIndex (index));
            
        MainMenuActivity @mainMenuActivity = MainMenuActivity ();
        launcherApplication_.SetupActivityNextFrame (mainMenuActivity);
    }
    
    void HandleConnectFailed ()
    {
        ErrorDialog ("Connect failed!", "Connect failed!\nPress OK to return to main menu.");
        GoToMainMenuState ();
    }
};

