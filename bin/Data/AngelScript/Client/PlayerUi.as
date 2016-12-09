class PlayerUi : ScriptObject
{
    protected Array <Button @> districtsButtons_;
    protected float beforeDistrictsButtonsUpdate_;
    
    protected void AddDistrictButton ()
    {
        Button @button = ui.root.GetChild ("ingame").CreateChild ("Button", 
                                                                  "district_button_" + districtsButtons_.length);
        button.SetStyleAuto ();
        button.defaultStyle = button.parent.defaultStyle;
        
        Text @buttonText = button.CreateChild ("Text", "text");
        buttonText.SetStyleAuto ();
        buttonText.SetAlignment (HA_CENTER, VA_CENTER);
        districtsButtons_.Push (button);
    }
    
    protected void UpdateDistrictsButtons ()
    {
        Map @map = node.parent.vars ["map"].GetPtr ();
        while (districtsButtons_.length < map.GetDistrictsCount ())
            AddDistrictButton ();
        
        Camera @camera = scene.GetChild ("camera").GetComponent ("Camera");
        int width = graphics.width;
        int height = graphics.height;
        
        for (int index = 0; index < districtsButtons_.length; index++)
        {
            District @district = map.GetDistrictByIndex (index);
            Button @button = districtsButtons_ [index];
            Text @buttonText = button.GetChild ("text");
            buttonText.text = district.name_;
            
            Vector2 point = camera.WorldToScreenPoint (scene.GetChild ("map").position +
                                                       district.colonyPosition_);
            point.x *= width;
            point.y *= height;
            
            button.SetPosition (point.x - height * 0.1f, point.y - height * 0.025f);
            button.SetSize (height * 0.2f, height * 0.05f);
            buttonText.fontSize = height * 0.03f;
            
        }
    }
        
    PlayerUi ()
    {
        beforeDistrictsButtonsUpdate_ = 0.001f;
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
        
        if (scene.GetChild ("map") !is null)
            UpdateDistrictsButtons ();
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
