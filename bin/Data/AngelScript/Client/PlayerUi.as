class PlayerUi : ScriptObject
{    
    PlayerUi ()
    {
        
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
