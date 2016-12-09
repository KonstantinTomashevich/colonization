class Player : ScriptObject
{
    protected LauncherApplication @launcherApplication_;
    protected String playerName_;
    protected float beforeMapUpdate_;
    protected float beforeMapNeighborsUpdate_;
    protected Map @mapPtr_;
    
    Player ()
    {
        beforeMapUpdate_ = 0.001f;
        beforeMapNeighborsUpdate_ = 0.001f;
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
        
        SubscribeToEvent ("ServerDisconnected", "HandleServerDisconnected");
        SubscribeToEvent ("ConnectFailed", "HandleConnectFailed");
        
        Map @map = CreateMap ();
        AddRef (map);
        node.vars ["map"] = map;
        mapPtr_ = map;
    }
    
    void Update (float timeStep)
    {
        if (node.vars ["goToMenuCalled"].GetBool ())
            GoToMainMenuState ();
            
        if (scene.GetChild ("map") !is null)
        {
            beforeMapUpdate_ -= timeStep;
            beforeMapNeighborsUpdate_ -= timeStep;
            
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
        }
    }
    
    void Stop ()
    {
        UnsubscribeFromAllEvents ();
        SetRefs (mapPtr_, 1);
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
            
        MainMenuActivity @mainMenuActivity = CreateMainMenuActivity ();
        launcherApplication_.SetupActivityNextFrame (mainMenuActivity);
    }
    
    void HandleConnectFailed ()
    {
        ErrorDialog ("Connect failed!", "Connect failed!\nPress OK to return to main menu.");
        GoToMainMenuState ();
    }
};

