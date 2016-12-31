class MainMenu : ScriptObject
{
    protected ActivitiesApplication @activitiesApplication_;
    
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
    
    ActivitiesApplication @get_activitiesApplication ()
    {
        return activitiesApplication_;
    }
    
    void set_activitiesApplication (ActivitiesApplication @activitiesApplication)
    {
        activitiesApplication_ = activitiesApplication;
    }
    
    void HandleStartGameClick ()
    {
        for (int index = 0; index < activitiesApplication_.GetActivitiesCount (); index++)
            activitiesApplication_.StopActivityNextFrame (activitiesApplication_.GetActivityByIndex (index));
        
        HostActivity @hostActivity = HostActivity ();
        hostActivity.serverPort = 13768;
        activitiesApplication_.SetupActivityNextFrame (hostActivity);
        
        IngamePlayerActivity @ingamePlayerActivity = IngamePlayerActivity ();
        ingamePlayerActivity.serverAdress = "localhost";
        ingamePlayerActivity.serverPort = 13768;
        ingamePlayerActivity.playerName = "Konstant";
        activitiesApplication_.SetupActivityNextFrame (ingamePlayerActivity);
    }
    
    void HandleExitClick ()
    {
        for (int index = 0; index < activitiesApplication_.GetActivitiesCount (); index++)
            activitiesApplication_.StopActivityNextFrame (activitiesApplication_.GetActivityByIndex (index));
        engine.Exit ();
    }
};
