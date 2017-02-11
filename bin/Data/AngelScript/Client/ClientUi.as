#include "AngelScript/Utils/ClientUtils.as"

// TODO: Set some ui elements color to player color.
class ClientUi : ScriptObject
{
    protected XMLFile @style_;
    protected bool isSceneLoaded_;
    protected float untilSelectionUpdate_;

    protected float COLONIZATORS_EXPEDITION_COST = 100.0f;
    protected float DEFAULT_INVESTITION_SIZE = 100.0f;
    protected float SELECTION_UPDATE_DELAY = 0.02f;

    protected void UpdateSelection ()
    {
        StringHash selectionType = node.parent.vars ["selectionType"].GetStringHash ();
        if (selectionType == StringHash ("Unit"))
        {
            UpdateUnitSelection ();
        }
        else if (selectionType == StringHash ("District"))
        {
            UpdateDistrictSelection ();
        }
        else
        {
            ClearSelection ();
        }
    }

    protected void UpdateUnitSelection ()
    {
        Window @districtInfoWindow = ui.root.GetChild ("ingame").GetChild ("districtInfoWindow");
        districtInfoWindow.visible = false;

        Window @unitInfoWindow = ui.root.GetChild ("ingame").GetChild ("unitInfoWindow");
        unitInfoWindow.visible = true;

        StringHash unitHash = node.parent.vars ["selectedHash"].GetStringHash ();
        Unit @unit = GetUnitByHash (scene, unitHash);
        if (unit !is null)
        {
            Text @ownerText = unitInfoWindow.GetChild ("ownerText");
            ownerText.text = unit.ownerPlayerName + "'s";

            Text @typeText = unitInfoWindow.GetChild ("typeText");
            if (unit.unitType == UNIT_FLEET)
            {
                typeText.text = "Fleet";
            }
            else if (unit.unitType == UNIT_TRADERS)
            {
                typeText.text = "Traders";
            }
            else if (unit.unitType == UNIT_COLONIZATORS)
            {
                typeText.text = "Colonizators";
            }
            else if (unit.unitType == UNIT_ARMY)
            {
                typeText.text = "Army";
            }

            Button @moveToButton = unitInfoWindow.GetChild ("moveToButton");
            if (unit.unitType == UNIT_FLEET or unit.unitType == UNIT_ARMY)
            {
                moveToButton.visible = true;
            }
            else
            {
                moveToButton.visible = false;
            }

            Map @map = scene.GetChild ("map").GetComponent ("Map");
            Text @positionText = unitInfoWindow.GetChild ("positionText");
            positionText.text = "in " + map.GetDistrictByHash (unit.positionHash).name;

            String additionalInfo;
            if (unit.unitType == UNIT_FLEET)
            {
                additionalInfo += "War ships count: " + unit.fleetUnitWarShipsCount + ".\n";
            }

            else if (unit.unitType == UNIT_TRADERS)
            {
                additionalInfo += "Trade goods cost: " + Floor (unit.tradersUnitTradeGoodsCost * 100.0f) / 100.0f + ".\n";
            }

            else if (unit.unitType == UNIT_COLONIZATORS)
            {
                additionalInfo += "Colonizators count: " + unit.colonizatorsUnitColonizatorsCount + ".\n";
            }

            else if (unit.unitType == UNIT_ARMY)
            {
                additionalInfo += "Soldiers count: " + unit.armyUnitSoldiersCount + ".\n";
            }

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
        else
        {
            // Clear selection if unit isn't exists.
            ClearSelection ();
        }
    }

    protected void UpdateDistrictSelection ()
    {
        Window @unitInfoWindow = ui.root.GetChild ("ingame").GetChild ("unitInfoWindow");
        unitInfoWindow.visible = false;

        Window @districtInfoWindow = ui.root.GetChild ("ingame").GetChild ("districtInfoWindow");
        districtInfoWindow.visible = true;

        Map @map = scene.GetChild ("map").GetComponent ("Map");
        StringHash districtHash = node.parent.vars ["selectedHash"].GetStringHash ();
        District @district = map.GetDistrictByHash (districtHash);

        String playerName = node.parent.vars ["playerName"].GetString ();
        Text@ nameText = districtInfoWindow.GetChild ("nameText");
        nameText.text = district.name;

        districtInfoWindow.GetChild ("resourcesInfoButton").visible = !district.isSea;
        districtInfoWindow.GetChild ("populationInfoButton").visible = !district.isSea;
        districtInfoWindow.GetChild ("colonyEvolutionInfoButton").visible = district.hasColony;

        StringHash infoType = districtInfoWindow.vars ["infoType"].GetStringHash ();
        UIElement @investButtons = districtInfoWindow.GetChild ("investButtons");
        investButtons.visible = (infoType == StringHash ("ColonyEvolution") and
                                 district.hasColony and
                                 node.parent.vars ["gold"].GetFloat () >= DEFAULT_INVESTITION_SIZE and
                                 district.colonyOwnerName == playerName);

        Button @sendColonizatorsButton = districtInfoWindow.GetChild ("sendColonizatorsButton");
        sendColonizatorsButton.visible = (infoType == StringHash ("Basic")) and
                                         not district.isSea and
                                         not district.isImpassable and
                                         (district.colonyOwnerName == playerName or not district.hasColony) and
                                         node.parent.vars ["gold"].GetFloat () >= COLONIZATORS_EXPEDITION_COST;

        if (infoType == StringHash ("Basic"))
        {
            UpdateDistrictBasicInfo (district, districtInfoWindow);
        }
        else if (infoType == StringHash ("Resources"))
        {
            UpdateDistrictResourcesInfo (district, districtInfoWindow);
        }
        else if (infoType == StringHash ("Population"))
        {
            UpdateDistrictPopulationInfo (district, districtInfoWindow);
        }
        else if (infoType == StringHash ("ColonyEvolution"))
        {
            UpdateDistrictColonyEvolutionInfo (district, districtInfoWindow);
        }
    }

    protected void ClearSelection ()
    {
        Window @districtInfoWindow = ui.root.GetChild ("ingame").GetChild ("districtInfoWindow");
        districtInfoWindow.visible = false;

        Window @unitInfoWindow = ui.root.GetChild ("ingame").GetChild ("unitInfoWindow");
        unitInfoWindow.visible = false;

        node.parent.vars ["selectionType"] = StringHash ("None");
        node.parent.vars ["selectedHash"] = StringHash ();
    }

    protected void UpdateDistrictBasicInfo (District @district, Window @districtInfoWindow)
    {
        String infoText = "";
        if (district.isSea)
        {
            infoText += "Sea district.\n";
        }
        else if (district.hasColony)
        {
            infoText += "Colonized by " + district.colonyOwnerName + ".\n";
        }
        else if (district.isImpassable)
        {
            infoText += "Uninhabitable.\n";
        }
        else
        {
            infoText += "Can be colonized.\n";
        }

        infoText += "Climate: ";
        if (district.climate == CLIMATE_COLD)
        {
            infoText += "cold.\n";
        }
        else if (district.climate == CLIMATE_DESERT)
        {
            infoText += "desert.\n";
        }
        else if (district.climate == CLIMATE_HOT)
        {
            infoText += "hot.\n";
        }
        else if (district.climate == CLIMATE_TEMPERATE)
        {
            infoText += "temperate.\n";
        }
        else if (district.climate == CLIMATE_TEMPERATE_CONTINENTAL)
        {
            infoText += "temperate continental.\n";
        }
        else if (district.climate == CLIMATE_TROPICAL)
        {
            infoText += "tropical.\n";
        }

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
        {
            infoText += "cold.\n";
        }
        else if (district.climate == CLIMATE_DESERT)
        {
            infoText += "desert.\n";
        }
        else if (district.climate == CLIMATE_HOT)
        {
            infoText += "hot.\n";
        }
        else if (district.climate == CLIMATE_TEMPERATE)
        {
            infoText += "temperate.\n";
        }
        else if (district.climate == CLIMATE_TEMPERATE_CONTINENTAL)
        {
            infoText += "temperate continental.\n";
        }
        else if (district.climate == CLIMATE_TROPICAL)
        {
            infoText += "tropical.\n";
        }

        infoText += "Forest square: " + FloorToInt (district.forestsSquare) + ".\n";
        infoText += "Forest reproductivity: " + Floor (district.forestsReproductivity * 100) / 100 + ".\n";

        if (district.hasCoalDeposits)
        {
            infoText += "Has coal deposits.\n";
        }
        if (district.hasIronDeposits)
        {
            infoText += "Has iron deposits.\n";
        }
        if (district.hasSilverDeposits)
        {
            infoText += "Has silver deposits.\n";
        }
        if (district.hasGoldDeposits)
        {
            infoText += "Has gold deposits.\n";
        }

        Text @informationTextUi = districtInfoWindow.GetChild ("informationText");
        informationTextUi.text = infoText;
    }

    protected void UpdateDistrictPopulationInfo (District @district, Window @districtInfoWindow)
    {
        String infoText = "";
        if (district.hasColony)
        {
            infoText += "Colony total population: " + (Floor (district.menCount) + Floor (district.womenCount)) + ".\n";
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

    ClientUi ()
    {
        isSceneLoaded_ = false;
        untilSelectionUpdate_ = 0.0f;
    }

    ~ClientUi ()
    {

    }

    void Start ()
    {
        ui.root.RemoveAllChildren ();
        style_ = cache.GetResource ("XMLFile", "UI/ColonizationUIStyle.xml");
        ui.root.defaultStyle = style_;

        ui.root.CreateChild ("UIElement", "billboardsRoot");
        UIElement @uiRoot = ui.LoadLayout (cache.GetResource ("XMLFile", "UI/Ingame.xml"));
        ui.root.AddChild (uiRoot);
        uiRoot.defaultStyle = style_;
        uiRoot.name = "ingame";
        ClearSelection ();

        Window @districtInfoWindow = ui.root.GetChild ("ingame").GetChild ("districtInfoWindow");
        Window @unitInfoWindow = ui.root.GetChild ("ingame").GetChild ("unitInfoWindow");
        districtInfoWindow.vars ["infoType"] = StringHash ("Basic");

        Button @basicInfoButton = districtInfoWindow.GetChild ("basicInfoButton");
        Button @resourcesInfoButton = districtInfoWindow.GetChild ("resourcesInfoButton");
        Button @populationInfoButton = districtInfoWindow.GetChild ("populationInfoButton");
        Button @colonyEvolutionInfoButton = districtInfoWindow.GetChild ("colonyEvolutionInfoButton");
        Button @sendColonizatorsButton = districtInfoWindow.GetChild ("sendColonizatorsButton");

        UIElement @investButtons = districtInfoWindow.GetChild ("investButtons");
        Button @investToFarmsButton = investButtons.GetChild ("investToFarms");
        Button @investToMinesButton = investButtons.GetChild ("investToMines");
        Button @investToIndustryButton = investButtons.GetChild ("investToIndustry");
        Button @investToLogisticsButton = investButtons.GetChild ("investToLogistics");
        Button @investToDefenseButton = investButtons.GetChild ("investToDefense");
        Button @moveToButton = unitInfoWindow.GetChild ("moveToButton");

        SubscribeToEvent (basicInfoButton, "Released", "HandleBasicInfoClick");
        SubscribeToEvent (resourcesInfoButton, "Released", "HandleResourcesInfoClick");
        SubscribeToEvent (populationInfoButton, "Released", "HandlePopulationInfoClick");
        SubscribeToEvent (colonyEvolutionInfoButton, "Released", "HandleColonyEvolutionInfoClick");
        SubscribeToEvent (sendColonizatorsButton, "Released", "HandleSendColonizatorsClick");

        SubscribeToEvent (investToFarmsButton, "Released", "HandleInvestClick");
        SubscribeToEvent (investToMinesButton, "Released", "HandleInvestClick");
        SubscribeToEvent (investToIndustryButton, "Released", "HandleInvestClick");
        SubscribeToEvent (investToLogisticsButton, "Released", "HandleInvestClick");
        SubscribeToEvent (investToDefenseButton, "Released", "HandleInvestClick");
        SubscribeToEvent (moveToButton, "Released", "HandleMoveUnitToClick");

        ScriptInstance @playerInfoWindowInstance = node.CreateChild ("PlayerInfoWindow", LOCAL).CreateComponent ("ScriptInstance");
        playerInfoWindowInstance.CreateObject (cache.GetResource ("ScriptFile",
                                                         "AngelScript/Client/UiHandlers/PlayerInfoWindow.as"),
                                               "PlayerInfoWindow");

        ScriptInstance @menuWindowInstance = node.CreateChild ("MenuWindow", LOCAL).CreateComponent ("ScriptInstance");
        menuWindowInstance.CreateObject (cache.GetResource ("ScriptFile",
                                                      "AngelScript/Client/UiHandlers/MenuWindow.as"),
                                         "MenuWindow");

        ScriptInstance @mapBillboardsInstance = node.CreateChild ("MapBillboards", LOCAL).CreateComponent ("ScriptInstance");
        mapBillboardsInstance.CreateObject (cache.GetResource ("ScriptFile",
                                                     "AngelScript/Client/UiHandlers/MapBillboards.as"),
                                            "MapBillboards");

        ScriptInstance @chatWindowInstance = node.CreateChild ("ChatWindow", LOCAL).CreateComponent ("ScriptInstance");
        chatWindowInstance.CreateObject (cache.GetResource ("ScriptFile",
                                                      "AngelScript/Client/UiHandlers/ChatWindow.as"),
                                         "ChatWindow");

        ScriptInstance @uiResizerInstance = node.CreateChild ("UiResizer", LOCAL).CreateComponent ("ScriptInstance");
        uiResizerInstance.CreateObject (cache.GetResource ("ScriptFile",
                                                         "AngelScript/Utils/UiResizer.as"),
                                        "UiResizer");
        uiResizerInstance.SetAttribute ("startElementName_", Variant ("UIRoot"));
        uiResizerInstance.SetAttribute ("continuousResize_", Variant (true));
    }

    void Update (float timeStep)
    {
        untilSelectionUpdate_ -= timeStep;
        if (!isSceneLoaded_)
        {
            isSceneLoaded_ = CheckIsSceneLoaded (scene);
        }
        else
        {
            if (untilSelectionUpdate_ <= 0.0f)
            {
                UpdateSelection ();
                untilSelectionUpdate_ = SELECTION_UPDATE_DELAY;
            }
        }
    }

    void Stop ()
    {

    }

    void HandleSendColonizatorsClick ()
    {
        Array <Variant> networkTasks = node.parent.GetChild ("networkScriptNode").vars ["tasksList"].GetVariantVector ();
        VariantMap taskData;
        taskData ["type"] = CTS_NETWORK_MESSAGE_SEND_PLAYER_ACTION;

        Map @map = node.parent.vars ["map"].GetPtr ();
        StringHash districtHash = node.parent.vars ["selectedHash"].GetStringHash ();

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
        StringHash districtHash = node.parent.vars ["selectedHash"].GetStringHash ();

        VectorBuffer buffer = VectorBuffer ();
        buffer.WriteInt (PLAYER_ACTION_INVEST_TO_COLONY);
        buffer.WriteStringHash (districtHash);
        buffer.WriteStringHash (investTypeHash);
        buffer.WriteFloat (DEFAULT_INVESTITION_SIZE);

        taskData ["buffer"] = buffer;
        networkTasks.Push (Variant (taskData));
        node.parent.GetChild ("networkScriptNode").vars ["tasksList"] = networkTasks;
    }

    void HandleMoveUnitToClick ()
    {
        node.parent.vars ["currentClickCommand"] = StringHash ("MoveUnit");
    }
};
