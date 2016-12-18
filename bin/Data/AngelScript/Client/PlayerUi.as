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
        Window @districtInfoWindow = ui.root.GetChild ("ingame").GetChild ("districtInfoWindow");
        districtInfoWindow.visible = false;
        
        Window @unitInfoWindow = ui.root.GetChild ("ingame").GetChild ("unitInfoWindow");
        unitInfoWindow.visible = true;
        
        UnitsContainer @unitsContainer = node.parent.vars ["unitsContainer"].GetPtr ();
        StringHash unitHash = node.parent.GetChild ("screenPressesHandlerScriptNode").
                                vars ["selectedHash"].GetStringHash ();
        Unit @unit = unitsContainer.GetUnitByHash (unitHash);
        SetRefs (unit, 5);
            
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
        
        String additionalInfo;
        if (unit.unitType_ == UNIT_FLEET)
        {
            FleetUnit @fleetUnit = unit;
            additionalInfo += "War ships count: " + fleetUnit.warShipsCount_ + ".\n";
        }
        
        else if (unit.unitType_ == UNIT_TRADERS)
        {
            TradersUnit @tradersUnit = unit;
            additionalInfo += "Trade goods cost: " + tradersUnit.tradeGoodsCost_ + ".\n";
        }
        
        else if (unit.unitType_ == UNIT_COLONIZATORS)
        {
            ColonizatorsUnit @colonizatorsUnit = unit;
            additionalInfo += "Colonizators count: " + colonizatorsUnit.colonizatorsCount_ + ".\n";
        }
        
        else if (unit.unitType_ == UNIT_ARMY)
        {
            ArmyUnit @armyUnit = unit;
            additionalInfo += "Soldiers count: " + armyUnit.soldiersCount_ + ".\n";
        }
        
        if (unit.way_.length > 0)
        {
            additionalInfo += "Going to: " + unit.way_ [unit.way_.length - 1].name_ + ".\n";
            additionalInfo += "Next waypoint: " + unit.way_ [1].name_ + "\n";
            additionalInfo += "Traveled to next waypoit: " + 
                                FloorToInt (unit.wayToNextDistrictProgressInPercents_) + "%.\n";
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
        
        Map @map = node.parent.vars ["map"].GetPtr ();
        StringHash districtHash = node.parent.GetChild ("screenPressesHandlerScriptNode").
                                vars ["selectedHash"].GetStringHash ();
        District @district = map.GetDistrictByHash (districtHash);
        SetRefs (district, 5);
        
        Text@ nameText = districtInfoWindow.GetChild ("nameText");
        nameText.text = district.name_;
        
        districtInfoWindow.GetChild ("resourcesInfoButton").visible = !district.isSea_;
        districtInfoWindow.GetChild ("populationInfoButton").visible = !district.isSea_;
        districtInfoWindow.GetChild ("colonyEvolutionInfoButton").visible = district.hasColony_;
        
        StringHash infoType = districtInfoWindow.vars ["infoType"].GetStringHash ();
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
        Window @districtInfoWindow = ui.root.GetChild ("ingame").GetChild ("districtInfoWindow");
        districtInfoWindow.visible = false;
        
        Window @unitInfoWindow = ui.root.GetChild ("ingame").GetChild ("unitInfoWindow");
        unitInfoWindow.visible = false;
    }
    
    protected void UpdateDistrictBasicInfo (District @district, Window @districtInfoWindow)
    {
        String infoText = "";
        if (district.isSea_)
            infoText += "Sea district.\n";
        else if (district.hasColony_)
            infoText += "Colonized by " + district.colonyOwnerName_ + ".\n";
        else if (district.isImpassable_)
            infoText += "Uninhabitable.\n";
        else
            infoText += "Can be colonized.\n";
        
        infoText += "Climate: ";
        if (district.climate_ == CLIMATE_COLD)
            infoText += "cold.\n";
        else if (district.climate_ == CLIMATE_DESERT)
            infoText += "desert.\n";
        else if (district.climate_ == CLIMATE_HOT)
            infoText += "hot.\n";
        else if (district.climate_ == CLIMATE_TEMPERATE)
            infoText += "temperate.\n";
        else if (district.climate_ == CLIMATE_TEMPERATE_CONTINENTAL)
            infoText += "temperate continental.\n";
        else if (district.climate_ == CLIMATE_TROPICAL)
            infoText += "tropical.\n";
            
        Text @informationTextUi = districtInfoWindow.GetChild ("informationText");
        informationTextUi.text = infoText;
    }
    
    protected void UpdateDistrictResourcesInfo (District @district, Window @districtInfoWindow)
    {
        String infoText = "";
        infoText += "Farming square: " + FloorToInt (district.farmingSquare_) + ".\n";
        infoText += "Land average fertility: " + FloorToInt (district.landAverageFertility_ * 100) + "%.\n";
        
        infoText += "Climate: ";
        if (district.climate_ == CLIMATE_COLD)
            infoText += "cold.\n";
        else if (district.climate_ == CLIMATE_DESERT)
            infoText += "desert.\n";
        else if (district.climate_ == CLIMATE_HOT)
            infoText += "hot.\n";
        else if (district.climate_ == CLIMATE_TEMPERATE)
            infoText += "temperate.\n";
        else if (district.climate_ == CLIMATE_TEMPERATE_CONTINENTAL)
            infoText += "temperate continental.\n";
        else if (district.climate_ == CLIMATE_TROPICAL)
            infoText += "tropical.\n";
        
        infoText += "Forest square: " + FloorToInt (district.forestsSquare_) + ".\n";
        infoText += "Forest reproductivity: " + Floor (district.forestsReproductivity_ * 100) / 100 + ".\n";
        
        if (district.hasCoalDeposits_)
            infoText += "Has coal deposits.\n";
        if (district.hasIronDeposits_)
            infoText += "Has iron deposits.\n";
        if (district.hasSilverDeposits_)
            infoText += "Has silver deposits.\n";
        if (district.hasGoldDeposits_)
            infoText += "Has gold deposits.\n";
        
        Text @informationTextUi = districtInfoWindow.GetChild ("informationText");
        informationTextUi.text = infoText;
    }
    
    protected void UpdateDistrictRopulationInfo (District @district, Window @districtInfoWindow)
    {
        String infoText = "";
        if (district.hasColony_)
        {
            infoText += "Colony total population: " + (district.mansCount_ + district.womenCount_) + ".\n";
            infoText += "Mans: " + district.mansCount_ + ". Women: " + district.womenCount_ + ".\n";
            infoText += "Average level of life: " + district.averageLevelOfLifePoints_ + ".\n";
            infoText += "\n";
        }
        
        if (district.nativesCount_ > 0)
        {
            infoText += "Natives population: " + district.nativesCount_ + ".\n";
            infoText += "Natives fighting tech level: " + district.nativesFightingTechnologyLevel_ + ".\n";
            infoText += "Natives agressiveness: " + district.nativesAggressiveness_ + ".\n";
            
            infoText += "Natives character: ";
            if (district.nativesCharacter_ == NATIVES_CHARATER_AGRESSIVE)
                infoText += "agressive.\n";
            else if (district.nativesCharacter_ == NATIVES_CHARACTER_MEDIUM)
                infoText += "medium.\n";
            else if (district.nativesCharacter_ == NATIVES_CHARACTER_ISOLATIONIST)
                infoText += "isolationist.\n";
            else if (district.nativesCharacter_ == NATIVES_CHARACTER_FRIENDLY)
                infoText += "friendly.\n";
            else if (district.nativesCharacter_ == NATIVES_CHARACTER_COLD)
                infoText += "cold.\n";
            infoText += "\n";
        }
        
        Text @informationTextUi = districtInfoWindow.GetChild ("informationText");
        informationTextUi.text = infoText;
    }
    
    protected void UpdateDistrictColonyEvolutionInfo (District @district, Window @districtInfoWindow)
    {
        String infoText = "";
        if (district.hasColony_)
        {
            infoText += "Farms evolution: " + Floor (district.farmsEvolutionPoints_ * 100) / 100 + ".\n";
            infoText += "Mines evolution: " + Floor (district.minesEvolutionPoints_ * 100) / 100 + ".\n";
            infoText += "Industry evolution: " + Floor (district.industryEvolutionPoints_ * 100) / 100 + ".\n";
            infoText += "Logistics evolution: " + Floor (district.logisticsEvolutionPoints_ * 100) / 100 + ".\n";
            infoText += "Defense evolution: " + Floor (district.defenseEvolutionPoints_ * 100) / 100 + ".\n";
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
        Window @districtInfoWindow = ui.root.GetChild ("ingame").GetChild ("districtInfoWindow");
        districtInfoWindow.vars ["infoType"] = StringHash ("Basic");
        
        Button @basicInfoButton = districtInfoWindow.GetChild ("basicInfoButton");
        Button @resourcesInfoButton = districtInfoWindow.GetChild ("resourcesInfoButton");
        Button @populationInfoButton = districtInfoWindow.GetChild ("populationInfoButton");
        Button @colonyEvolutionInfoButton = districtInfoWindow.GetChild ("colonyEvolutionInfoButton");
        
        SubscribeToEvent (exitButton, "Released", "HandleExitClick");
        SubscribeToEvent (basicInfoButton, "Released", "HandleBasicInfoClick");
        SubscribeToEvent (resourcesInfoButton, "Released", "HandleResourcesInfoClick");
        SubscribeToEvent (populationInfoButton, "Released", "HandlePopulationInfoClick");
        SubscribeToEvent (colonyEvolutionInfoButton, "Released", "HandleColonyEvolutionInfoClick");
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
};
