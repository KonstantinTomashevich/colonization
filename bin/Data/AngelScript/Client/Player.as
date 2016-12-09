class Player : ScriptObject
{
    protected LauncherApplication @launcherApplication_;
    protected String playerName_;
    
    Player ()
    {
        
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
        
        SubscribeToEvent ("ServerDisconnected", "HandleServerDisconnected");
        SubscribeToEvent ("ConnectFailed", "HandleConnectFailed");
    }
    
    void Update (float timeStep)
    {
        if (node.vars ["goToMenuCalled"].GetBool ())
            GoToMainMenuState ();
    }
    
    void Stop ()
    {
        UnsubscribeFromAllEvents ();
        Dispose ();
    }
    
    void Dispose ()
    {
        
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

