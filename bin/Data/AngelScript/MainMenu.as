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
        XMLFile@ style = cache.GetResource ("XMLFile", "UI/DefaultStyle.xml");
        ui.root.defaultStyle = style;
    
        UIElement @uiRoot = ui.LoadLayout (cache.GetResource ("XMLFile", "UI/MainMenu.xml"));
        ui.root.AddChild (uiRoot);
        
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
        ui.root.RemoveAllChildren ();
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
        
    }
    
    void HandleExitClick ()
    {
        for (int index = 0; index < launcherApplication_.GetActivitiesCount (); index++)
            launcherApplication_.StopActivity (launcherApplication_.GetActivityByIndex (index));
        engine.Exit ();
    }
};
