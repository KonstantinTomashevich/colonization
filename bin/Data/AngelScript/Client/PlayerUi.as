class PlayerUi : ScriptObject
{
    protected bool isSceneLoaded_;
    protected float beforeSelectionUpdate_;
    
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
        ui.root.GetChild ("ingame").GetChild ("sendColonizatorsButton").visible = false;
        Window @districtInfoWindow = ui.root.GetChild ("ingame").GetChild ("districtInfoWindow");
        districtInfoWindow.visible = false;
        
        Window @unitInfoWindow = ui.root.GetChild ("ingame").GetChild ("unitInfoWindow");
        unitInfoWindow.visible = true;
        
        StringHash unitHash = node.parent.GetChild ("screenPressesHandlerScriptNode").
                                vars ["selectedHash"].GetStringHash ();
        Array <Node@> unitsNodes = scene.GetChild ("units").GetChildrenWithComponent ("Unit");
        Unit @unit = unitsNodes [0].GetComponent ("Unit");
        int index = 1;
        while (unit.hash != unitHash and index < unitsNodes.length)
        {
            unit = unitsNodes [index].GetComponent ("Unit");
            index++;
        }
            
        Text @ownerText = unitInfoWindow.GetChild ("ownerText");
        ownerText.text = unit.ownerPlayerName + "'s";
        
        Text @typeText = unitInfoWindow.GetChild ("typeText");
        if (unit.unitType == UNIT_FLEET)
            typeText.text = "Fleet";
        else if (unit.unitType == UNIT_TRADERS)
            typeText.text = "Traders";
        else if (unit.unitType == UNIT_COLONIZATORS)
            typeText.text = "Colonizators";
        else if (unit.unitType == UNIT_ARMY)
            typeText.text = "Army";
        
        Map @map = scene.GetChild ("map").GetComponent ("Map");        
        Text @positionText = unitInfoWindow.GetChild ("positionText");
        positionText.text = "in " + map.GetDistrictByHash (unit.positionHash).name;
        
        String additionalInfo;
        if (unit.unitType == UNIT_FLEET)
            additionalInfo += "War ships count: " + unit.fleetUnitWarShipsCount + ".\n";
        
        else if (unit.unitType == UNIT_TRADERS)
            additionalInfo += "Trade goods cost: " + Floor (unit.tradersUnitTradeGoodsCost) + ".\n";
        
        else if (unit.unitType == UNIT_COLONIZATORS)
            additionalInfo += "Colonizators count: " + unit.colonizatorsUnitColonizatorsCount + ".\n";
        
        else if (unit.unitType == UNIT_ARMY)
            additionalInfo += "Soldiers count: " + unit.armyUnitSoldiersCount + ".\n";
        
        if (unit.GetWay ().length > 0)
        {
            Array <StringHash> unitWay = unit.GetWay ();
            additionalInfo += "Going to: " + map.GetDistrictByHash (unitWay [unitWay.length - 1]).name + ".\n";
            additionalInfo += "Next waypoint: " + map.GetDistrictByHash (unitWay [0]).name + "\n";
            additionalInfo += "Traveled to next waypoit: " + 
                                FloorToInt (unit.wayToNextDistrictProgressInPercents) + "%.\n";
        }
        
        Text @anotherText = unitInfoWindow.GetChild ("anotherText");
        anotherText.text = additionalInfo;
    }
    
    protected void UpdateDistrictSelection ()
    {
        Window @unitInfoWindow = ui.root.GetChild ("ingame").GetChild ("unitInfoWindow");
        unitInfoWindow.visible = false;
        
        Window @districtInfoWindow = ui.root.GetChild ("ingame").GetChild ("districtInfoWindow");
        districtInfoWindow.visible = true;
        
        Map @map = scene.GetChild ("map").GetComponent ("Map");  
        StringHash districtHash = node.parent.GetChild ("screenPressesHandlerScriptNode").
                                vars ["selectedHash"].GetStringHash ();
        District @district = map.GetDistrictByHash (districtHash);
        
        String playerName = node.parent.vars ["playerName"].GetString ();
        ui.root.GetChild ("ingame").GetChild ("sendColonizatorsButton").visible =
                                        (!district.isImpassable &&
                                         !district.isSea && (!district.hasColony ||
                                                             (district.hasColony && 
                                                             district.colonyOwnerName == playerName)));
        
        Text@ nameText = districtInfoWindow.GetChild ("nameText");
        nameText.text = district.name;
        
        districtInfoWindow.GetChild ("resourcesInfoButton").visible = !district.isSea;
        districtInfoWindow.GetChild ("populationInfoButton").visible = !district.isSea;
        districtInfoWindow.GetChild ("colonyEvolutionInfoButton").visible = district.hasColony;
        
        StringHash infoType = districtInfoWindow.vars ["infoType"].GetStringHash ();
        UIElement @investButtons = districtInfoWindow.GetChild ("investButtons");
        investButtons.visible = (infoType == StringHash ("ColonyEvolution") and
                                 district.hasColony and
                                 node.parent.vars ["gold"].GetFloat () >= 100.0f and
                                 ui.root.GetChild ("ingame").GetChild ("sendColonizatorsButton").visible);
        
        if (infoType == StringHash ("Basic"))
            UpdateDistrictBasicInfo (district, districtInfoWindow);
        else if (infoType == StringHash ("Resources"))
            UpdateDistrictResourcesInfo (district, districtInfoWindow);
        else if (infoType == StringHash ("Population"))
            UpdateDistrictRopulationInfo (district, districtInfoWindow);
        else if (infoType == StringHash ("ColonyEvolution"))
            UpdateDistrictColonyEvolutionInfo (district, districtInfoWindow);
    }
    
    protected void ClearSelection ()
    {
        ui.root.GetChild ("ingame").GetChild ("sendColonizatorsButton").visible = false;
        Window @districtInfoWindow = ui.root.GetChild ("ingame").GetChild ("districtInfoWindow");
        districtInfoWindow.visible = false;
        
        Window @unitInfoWindow = ui.root.GetChild ("ingame").GetChild ("unitInfoWindow");
        unitInfoWindow.visible = false;
    }
    
    protected void UpdateDistrictBasicInfo (District @district, Window @districtInfoWindow)
    {
        String infoText = "";
        if (district.isSea)
            infoText += "Sea district.\n";
        else if (district.hasColony)
            infoText += "Colonized by " + district.colonyOwnerName + ".\n";
        else if (district.isImpassable)
            infoText += "Uninhabitable.\n";
        else
            infoText += "Can be colonized.\n";
        
        infoText += "Climate: ";
        if (district.climate == CLIMATE_COLD)
            infoText += "cold.\n";
        else if (district.climate == CLIMATE_DESERT)
            infoText += "desert.\n";
        else if (district.climate == CLIMATE_HOT)
            infoText += "hot.\n";
        else if (district.climate == CLIMATE_TEMPERATE)
            infoText += "temperate.\n";
        else if (district.climate == CLIMATE_TEMPERATE_CONTINENTAL)
            infoText += "temperate continental.\n";
        else if (district.climate == CLIMATE_TROPICAL)
            infoText += "tropical.\n";
            
        Text @informationTextUi = districtInfoWindow.GetChild ("informationText");
        informationTextUi.text = infoText;
    }
    
    protected void UpdateDistrictResourcesInfo (District @district, Window @districtInfoWindow)
    {
        String infoText = "";
        infoText += "Farming square: " + FloorToInt (district.farmingSquare) + ".\n";
        infoText += "Land average fertility: " + FloorToInt (district.landAverageFertility * 100) + "%.\n";
        
        infoText += "Climate: ";
        if (district.climate == CLIMATE_COLD)
            infoText += "cold.\n";
        else if (district.climate == CLIMATE_DESERT)
            infoText += "desert.\n";
        else if (district.climate == CLIMATE_HOT)
            infoText += "hot.\n";
        else if (district.climate == CLIMATE_TEMPERATE)
            infoText += "temperate.\n";
        else if (district.climate == CLIMATE_TEMPERATE_CONTINENTAL)
            infoText += "temperate continental.\n";
        else if (district.climate == CLIMATE_TROPICAL)
            infoText += "tropical.\n";
        
        infoText += "Forest square: " + FloorToInt (district.forestsSquare) + ".\n";
        infoText += "Forest reproductivity: " + Floor (district.forestsReproductivity * 100) / 100 + ".\n";
        
        if (district.hasCoalDeposits)
            infoText += "Has coal deposits.\n";
        if (district.hasIronDeposits)
            infoText += "Has iron deposits.\n";
        if (district.hasSilverDeposits)
            infoText += "Has silver deposits.\n";
        if (district.hasGoldDeposits)
            infoText += "Has gold deposits.\n";
        
        Text @informationTextUi = districtInfoWindow.GetChild ("informationText");
        informationTextUi.text = infoText;
    }
    
    protected void UpdateDistrictRopulationInfo (District @district, Window @districtInfoWindow)
    {
        String infoText = "";
        if (district.hasColony)
        {
            infoText += "Colony total population: " + Floor (district.menCount + district.womenCount) + ".\n";
            infoText += "Men: " + Floor (district.menCount) + ". Women: " + Floor (district.womenCount) + ".\n";
            infoText += "Average level of life: " + Floor (district.averageLevelOfLifePoints * 100.0f) / 100.0f + ".\n";
            infoText += "\n";
        }
        
        if (district.nativesCount > 0)
        {
            infoText += "Natives population: " + Floor (district.nativesCount) + ".\n";
            infoText += "Natives fighting tech level: " + Floor (district.nativesFightingTechnologyLevel * 100.0f) / 100.0f + ".\n";
            infoText += "Natives agressiveness: " + Floor (district.nativesAggressiveness * 100.0f) / 100.0f + ".\n";
            
            infoText += "Natives character: ";
            if (district.nativesCharacter == NATIVES_CHARATER_AGGRESSIVE)
                infoText += "agressive.\n";
            else if (district.nativesCharacter == NATIVES_CHARACTER_MEDIUM)
                infoText += "medium.\n";
            else if (district.nativesCharacter == NATIVES_CHARACTER_ISOLATIONIST)
                infoText += "isolationist.\n";
            else if (district.nativesCharacter == NATIVES_CHARACTER_FRIENDLY)
                infoText += "friendly.\n";
            else if (district.nativesCharacter == NATIVES_CHARACTER_COLD)
                infoText += "cold.\n";
            infoText += "\n";
        }
        
        Text @informationTextUi = districtInfoWindow.GetChild ("informationText");
        informationTextUi.text = infoText;
    }
    
    protected void UpdateDistrictColonyEvolutionInfo (District @district, Window @districtInfoWindow)
    {
        String infoText = "";
        if (district.hasColony)
        {
            infoText += "Farms evolution: " + Floor (district.farmsEvolutionPoints * 100) / 100 + ".\n";
            infoText += "Mines evolution: " + Floor (district.minesEvolutionPoints * 100) / 100 + ".\n";
            infoText += "Industry evolution: " + Floor (district.industryEvolutionPoints * 100) / 100 + ".\n";
            infoText += "Logistics evolution: " + Floor (district.logisticsEvolutionPoints * 100) / 100 + ".\n";
            infoText += "Defense evolution: " + Floor (district.defenseEvolutionPoints * 100) / 100 + ".\n";
        }
        
        Text @informationTextUi = districtInfoWindow.GetChild ("informationText");
        informationTextUi.text = infoText;
    }
        
    PlayerUi ()
    {
        isSceneLoaded_ = false;
        beforeSelectionUpdate_ = 0.001f;
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
        ClearSelection ();
        
        Button @exitButton = uiRoot.GetChild ("exitButton");
        Button @sendColonizatorsButton = uiRoot.GetChild ("sendColonizatorsButton");
        Window @districtInfoWindow = ui.root.GetChild ("ingame").GetChild ("districtInfoWindow");
        districtInfoWindow.vars ["infoType"] = StringHash ("Basic");
        
        Button @basicInfoButton = districtInfoWindow.GetChild ("basicInfoButton");
        Button @resourcesInfoButton = districtInfoWindow.GetChild ("resourcesInfoButton");
        Button @populationInfoButton = districtInfoWindow.GetChild ("populationInfoButton");
        Button @colonyEvolutionInfoButton = districtInfoWindow.GetChild ("colonyEvolutionInfoButton");
        
        UIElement @investButtons = districtInfoWindow.GetChild ("investButtons");
        Button @investToFarmsButton = investButtons.GetChild ("investToFarms");
        Button @investToMinesButton = investButtons.GetChild ("investToMines");
        Button @investToIndustryButton = investButtons.GetChild ("investToIndustry");
        Button @investToLogisticsButton = investButtons.GetChild ("investToLogistics");
        Button @investToDefenseButton = investButtons.GetChild ("investToDefense");
        
        SubscribeToEvent (exitButton, "Released", "HandleExitClick");
        SubscribeToEvent (sendColonizatorsButton, "Released", "HandleSendColonizatorsClick");
        SubscribeToEvent (basicInfoButton, "Released", "HandleBasicInfoClick");
        SubscribeToEvent (resourcesInfoButton, "Released", "HandleResourcesInfoClick");
        SubscribeToEvent (populationInfoButton, "Released", "HandlePopulationInfoClick");
        SubscribeToEvent (colonyEvolutionInfoButton, "Released", "HandleColonyEvolutionInfoClick");
        
        SubscribeToEvent (investToFarmsButton, "Released", "HandleInvestClick");
        SubscribeToEvent (investToMinesButton, "Released", "HandleInvestClick");
        SubscribeToEvent (investToIndustryButton, "Released", "HandleInvestClick");
        SubscribeToEvent (investToLogisticsButton, "Released", "HandleInvestClick");
        SubscribeToEvent (investToDefenseButton, "Released", "HandleInvestClick");
    }
    
    void Update (float timeStep)
    {
        beforeSelectionUpdate_ -= timeStep;
        
        Text @playerStatsText = ui.root.GetChild ("ingame").GetChild ("playerStatsInfo");
        String playerStatsInfo = "";
        playerStatsInfo += node.parent.vars ["playerName"].GetString () + "\n";
        playerStatsInfo += "Gold: " + node.parent.vars ["gold"].GetFloat () + "\n";
        playerStatsInfo += "Points: " + node.parent.vars ["points"].GetFloat ();
        playerStatsText.text = playerStatsInfo;
        
        if (!isSceneLoaded_)
            CheckIsSceneLoaded ();
            
        if (isSceneLoaded_ and beforeSelectionUpdate_ <= 0.0f)
        {
            UpdateSelection ();
            beforeSelectionUpdate_ = 1.0f / 30.0f;
        }
    }
    
    void Stop ()
    {
        UnsubscribeFromAllEvents ();
    }
    
    void HandleExitClick ()
    {
        node.parent.vars ["goToMenuCalled"] = true;
    }
    
    void HandleSendColonizatorsClick ()
    {
        Array <Variant> networkTasks = node.parent.GetChild ("networkScriptNode").vars ["tasksList"].GetVariantVector ();
        VariantMap taskData;
        taskData ["type"] = CTS_NETWORK_MESSAGE_SEND_PLAYER_ACTION;
        
        Map @map = node.parent.vars ["map"].GetPtr ();
        StringHash districtHash = node.parent.GetChild ("screenPressesHandlerScriptNode").
                                vars ["selectedHash"].GetStringHash ();
        
        VectorBuffer buffer = VectorBuffer ();
        buffer.WriteInt (PLAYER_ACTION_REQUEST_COLONIZATORS_FROM_EUROPE);
        buffer.WriteStringHash (districtHash);
        
        taskData ["buffer"] = buffer;
        networkTasks.Push (Variant (taskData));
        node.parent.GetChild ("networkScriptNode").vars ["tasksList"] = networkTasks;
    }
    
    void HandleBasicInfoClick ()
    {
        Window @districtInfoWindow = ui.root.GetChild ("ingame").GetChild ("districtInfoWindow");
        districtInfoWindow.vars ["infoType"] = StringHash ("Basic");
    }
    
    void HandleResourcesInfoClick ()
    {
        Window @districtInfoWindow = ui.root.GetChild ("ingame").GetChild ("districtInfoWindow");
        districtInfoWindow.vars ["infoType"] = StringHash ("Resources");
    }
    
    void HandlePopulationInfoClick ()
    {
        Window @districtInfoWindow = ui.root.GetChild ("ingame").GetChild ("districtInfoWindow");
        districtInfoWindow.vars ["infoType"] = StringHash ("Population");
    }
    
    void HandleColonyEvolutionInfoClick ()
    {
        Window @districtInfoWindow = ui.root.GetChild ("ingame").GetChild ("districtInfoWindow");
        districtInfoWindow.vars ["infoType"] = StringHash ("ColonyEvolution");
    }
    
    void HandleInvestClick (StringHash eventType, VariantMap &eventData)
    {
        UIElement @element = eventData ["Element"].GetPtr ();
        StringHash investTypeHash = StringHash (element.vars ["investitionType"].GetString ());
        
        Array <Variant> networkTasks = node.parent.GetChild ("networkScriptNode").vars ["tasksList"].GetVariantVector ();
        VariantMap taskData;
        taskData ["type"] = CTS_NETWORK_MESSAGE_SEND_PLAYER_ACTION;
        StringHash districtHash = node.parent.GetChild ("screenPressesHandlerScriptNode").
                                vars ["selectedHash"].GetStringHash ();
        
        VectorBuffer buffer = VectorBuffer ();
        buffer.WriteInt (PLAYER_ACTION_INVEST_TO_COLONY);
        buffer.WriteStringHash (districtHash);
        buffer.WriteStringHash (investTypeHash);
        buffer.WriteFloat (100.0f);
        
        taskData ["buffer"] = buffer;
        networkTasks.Push (Variant (taskData));
        node.parent.GetChild ("networkScriptNode").vars ["tasksList"] = networkTasks;
    }
};
