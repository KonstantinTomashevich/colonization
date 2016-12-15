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
    
    protected void UpdateSelection ()
    {
        StringHash selectionType = node.parent.GetChild ("screenPressesHandlerScriptNode").
                                    vars ["selectionType"].GetStringHash ();
        if (selectionType == StringHash ("Unit"))
            UpdateUnitSelection ();
        else if (selectionType == StringHash ("District"))
            UpdateDistrictSelection ();
        else
            ClearSelection ();
    }
    
    protected void UpdateUnitSelection ()
    {
        Window @unitInfoWindow = ui.root.GetChild ("ingame").GetChild ("unitInfoWindow");
        unitInfoWindow.visible = true;
        
        UnitsContainer @unitsContainer = node.parent.vars ["unitsContainer"].GetPtr ();
        StringHash unitHash = node.parent.GetChild ("screenPressesHandlerScriptNode").
                                vars ["selectedHash"].GetStringHash ();
        Unit @unit = unitsContainer.GetUnitByHash (unitHash);
            
        Text @ownerText = unitInfoWindow.GetChild ("ownerText");
        ownerText.text = unit.ownerPlayer_ + "'s";
        
        Text @typeText = unitInfoWindow.GetChild ("typeText");
        if (unit.unitType_ == UNIT_FLEET)
            typeText.text = "Fleet";
        else if (unit.unitType_ == UNIT_TRADERS)
            typeText.text = "Traders";
        else if (unit.unitType_ == UNIT_COLONIZATORS)
            typeText.text = "Colonizators";
        else if (unit.unitType_ == UNIT_ARMY)
            typeText.text = "Army";
                
        Text @positionText = unitInfoWindow.GetChild ("positionText");
        positionText.text = "in " + unit.position_.name_;
    }
    
    protected void UpdateDistrictSelection ()
    {
        Window @unitInfoWindow = ui.root.GetChild ("ingame").GetChild ("unitInfoWindow");
        unitInfoWindow.visible = false;
    }
    
    protected void ClearSelection ()
    {
        Window @unitInfoWindow = ui.root.GetChild ("ingame").GetChild ("unitInfoWindow");
        unitInfoWindow.visible = false;
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
            
        if (isSceneLoaded_)
            UpdateSelection ();
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
