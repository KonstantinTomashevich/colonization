class PlayerUi : ScriptObject
{
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
        
    PlayerUi ()
    {
        isSceneLoaded_ = false;
    }
    
    ~PlayerUi ()
    {
        
    }
    
    void Start ()
    {
        ui.root.RemoveAllChildren ();
        XMLFile@ style = cache.GetResource ("XMLFile", "UI/DefaultStyle.xml");
        ui.root.defaultStyle = style;
        
        UIElement @uiRoot = ui.LoadLayout (cache.GetResource ("XMLFile", "UI/Ingame.xml"));
        ui.root.AddChild (uiRoot);
        uiRoot.defaultStyle = style;
        uiRoot.name = "ingame";
        
        Button @exitButton = uiRoot.GetChild ("exitButton");
        
        SubscribeToEvent (exitButton, "Released", "HandleExitClick");
    }
    
    void Update (float timeStep)
    {
        Text @playerStatsText = ui.root.GetChild ("ingame").GetChild ("playerStatsInfo");
        String playerStatsInfo = "";
        playerStatsInfo += node.parent.vars ["playerName"].GetString () + "\n";
        playerStatsInfo += "Gold: " + node.parent.vars ["gold"].GetFloat () + "\n";
        playerStatsInfo += "Points: " + node.parent.vars ["points"].GetFloat ();
        playerStatsText.text = playerStatsInfo;
        
        if (!isSceneLoaded_)
            CheckIsSceneLoaded ();
    }
    
    void Stop ()
    {
        UnsubscribeFromAllEvents ();
    }
    
    void HandleExitClick ()
    {
        node.parent.vars ["goToMenuCalled"] = true;
    }
};
