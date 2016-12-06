class MainMenu : ScriptObject
{
    protected LauncherApplication @launcherApplication_;
    
    MainMenu ()
    {
        
    }
    
    ~MainMenu ()
    {
        
    }
    
    void Start ()
    {
        ui.root.RemoveAllChildren ();
        XMLFile@ style = cache.GetResource ("XMLFile", "UI/DefaultStyle.xml");
        ui.root.defaultStyle = style;
    
        UIElement @uiRoot = ui.LoadLayout (cache.GetResource ("XMLFile", "UI/MainMenu.xml"));
        ui.root.AddChild (uiRoot);
        uiRoot.name = "main_menu";
        
        Button @startGameButton = uiRoot.GetChild ("startGameButton");
        Button @exitButton = uiRoot.GetChild ("exitButton");
        
        SubscribeToEvent (startGameButton, "Released", "HandleStartGameClick");
        SubscribeToEvent (exitButton, "Released", "HandleExitClick");
    }
    
    void Update (float timeStep)
    {
        
    }
    
    void Stop ()
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
    
    void HandleStartGameClick ()
    {
        for (int index = 0; index < launcherApplication_.GetActivitiesCount (); index++)
            launcherApplication_.StopActivityNextFrame (launcherApplication_.GetActivityByIndex (index));
        
        HostActivity @hostActivity = CreateHostActivity ();
        hostActivity.serverPort = 13768;
        launcherApplication_.SetupActivityNextFrame (hostActivity);
        
        IngamePlayerActivity @ingamePlayerActivity = CreateIngamePlayerActivity ();
        ingamePlayerActivity.serverAdress = "localhost";
        ingamePlayerActivity.serverPort = 13768;
        ingamePlayerActivity.playerName = "Konstant";
        launcherApplication_.SetupActivityNextFrame (ingamePlayerActivity);
    }
    
    void HandleExitClick ()
    {
        for (int index = 0; index < launcherApplication_.GetActivitiesCount (); index++)
            launcherApplication_.StopActivityNextFrame (launcherApplication_.GetActivityByIndex (index));
        engine.Exit ();
    }
};
