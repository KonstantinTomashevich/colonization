class Player : ScriptObject
{
    protected LauncherApplication @launcherApplication_;
    
    Player ()
    {
        
    }
    
    ~Player ()
    {
        
    }
    
    void Start ()
    {
        XMLFile@ style = cache.GetResource ("XMLFile", "UI/DefaultStyle.xml");
        ui.root.defaultStyle = style;
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
};

