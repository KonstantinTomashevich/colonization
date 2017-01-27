#include "AngelScript/Utils/CheckIsSceneLoaded.as"
#include "AngelScript/Utils/GetUnitByHash.as"
#include "AngelScript/Utils/GetPlayerInfoByName.as"
#include "AngelScript/Utils/GetUnitsInDistrict.as"

class ClientUi : ScriptObject
{
    protected bool isSceneLoaded_;
    protected float untilSelectionUpdate_;
    protected UIElement @billboardsRoot_;
    protected XMLFile @style_;
    protected XMLFile @billboardXML_;
    protected XMLFile @unitIconXML_;

    protected void UpdateSelection ()
    {
        StringHash selectionType = node.parent.vars ["selectionType"].GetStringHash ();
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

        StringHash unitHash = node.parent.vars ["selectedHash"].GetStringHash ();
        Unit @unit = GetUnitByHash (scene, unitHash);
        if (unit !is null)
        {

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
                additionalInfo += "Trade goods cost: " + Floor (unit.tradersUnitTradeGoodsCost * 100.0f) / 100.0f + ".\n";

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

        node.parent.vars ["selectionType"] = StringHash ("None");
        node.parent.vars ["selectedHash"] = StringHash ();
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

    protected void ProcessDistrictBillboards ()
    {
        Map @map = scene.GetChild ("map").GetComponent ("Map");
        Node @cameraNode = scene.GetChild ("camera");
        int nextBillboardIndex = 0;

        if (cameraNode !is null)
        {
            Camera @camera = cameraNode.GetComponent ("Camera");
            for (int index = 0; index < map.GetDistrictsCount (); index++)
            {
                District @district = map.GetDistrictByIndex (index);
                Vector2 screenPoint = camera.WorldToScreenPoint (district.unitPosition + Vector3 (0.0f, 3.5f, 0.0f));
                if (screenPoint.x > -0.1f and screenPoint.x < 1.1f and
                    screenPoint.y > -0.1f and screenPoint.y < 1.1f)
                {
                    if (nextBillboardIndex == billboardsRoot_.GetNumChildren (false))
                    {
                        billboardsRoot_.LoadChildXML (billboardXML_.GetRoot (), style_);
                    }

                    UIElement @billboard = billboardsRoot_.GetChildren () [nextBillboardIndex];
                    billboard.SetSize (graphics.height * 0.3f, graphics.height * 0.075f);
                    billboard.SetPosition (screenPoint.x * graphics.width - billboard.width / 2,
                        screenPoint.y * graphics.height - billboard.height / 2);

                    UpdateDistrictBillboardTitle (billboard, district);
                    UpdateDistrictBillboardBackground (billboard, district);
                    UpdateDistrictBillboardUnitsSection (billboard, district);
                    nextBillboardIndex++;
                }
            }
        }

        while (nextBillboardIndex < billboardsRoot_.GetNumChildren (false))
        {
            billboardsRoot_.GetChildren () [nextBillboardIndex].Remove ();
        }
    }

    protected void UpdateDistrictBillboardTitle (UIElement @billboard, District @district)
    {
        Text @title = billboard.GetChild ("title");
        title.fontSize = graphics.height * 0.01f;
        String titleText = district.name + "\n";
        if (district.isSea)
            titleText += "[under water]";
        else if (district.isImpassable)
            titleText += "[impassable]";
        else if (district.hasColony)
            titleText += "[colony of " + district.colonyOwnerName + "]";
        else
            titleText += "[can be colonized]";
        title.text = titleText;
    }

    protected void UpdateDistrictBillboardBackground (UIElement @billboard, District @district)
    {
        BorderImage @background = billboard.GetChild ("background");
        if (district.hasColony)
        {
            PlayerInfo @playerInfo = GetPlayerInfoByName (scene, district.colonyOwnerName);
            if (playerInfo !is null)
            {
                background.color = playerInfo.color;
            }
            else
            {
                background.color = Color (0.5f, 0.5f, 0.5f, 1.0f);
            }
        }
        else
        {
            background.color = Color (0.5f, 0.5f, 0.5f, 1.0f);
        }
    }

    protected void UpdateDistrictBillboardUnitsSection (UIElement @billboard, District @district)
    {
        // TODO: This algorithm can be optimized!
        Array <Unit @> unitsInDistrict = GetUnitsInDistrict (scene, district.hash);
        UIElement @unitsElement = billboard.GetChild ("units");

        if (unitsInDistrict.length > 0)
        {
            int index = 0;
            for (index = 0; index < unitsInDistrict.length; index++)
            {
                Unit @unit = unitsInDistrict [index];
                int size = billboard.height * 0.45f;
                int offset = billboard.height * 0.1f + size * index * 1.1f;

                if (index == unitsElement.GetNumChildren (false))
                {
                    unitsElement.LoadChildXML (unitIconXML_.GetRoot (), style_);
                }

                UIElement @unitElement = unitsElement.GetChildren () [index];
                unitElement.SetSize (size, size);
                unitElement.SetPosition (offset, billboard.height * 0.5f);

                UpdateUnitIconTypeIcon (unitElement, unit);
                UpdateUnitIconButton (unitElement, unit);
            }

            while (index < unitsElement.GetNumChildren (false))
            {
                unitsElement.GetChildren () [index].Remove ();
            }
        }
        else
        {
            unitsElement.RemoveAllChildren ();
        }
    }

    protected void UpdateUnitIconTypeIcon (UIElement @unitElement, Unit @unit)
    {
        BorderImage @typeIcon = unitElement.GetChild ("typeIcon");
        if (unit.unitType == UNIT_FLEET)
        {
            typeIcon.texture = cache.GetResource ("Texture2D", "Textures/FleetIcon.png");
        }
        else if (unit.unitType == UNIT_COLONIZATORS)
        {
            typeIcon.texture = cache.GetResource ("Texture2D", "Textures/ColonizatorsIcon.png");
        }
        else if (unit.unitType == UNIT_TRADERS)
        {
            typeIcon.texture = cache.GetResource ("Texture2D", "Textures/TradersIcon.png");
        }
        else if (unit.unitType == UNIT_ARMY)
        {
            typeIcon.texture = cache.GetResource ("Texture2D", "Textures/ArmyIcon.png");
        }
    }

    protected void UpdateUnitIconButton (UIElement @unitElement, Unit @unit)
    {
        Button @backgroundButton = unitElement.GetChild ("background");
        PlayerInfo @playerInfo = GetPlayerInfoByName (scene, unit.ownerPlayerName);
        backgroundButton.vars ["unitHash"] = Variant (unit.hash);

        SubscribeToEvent (backgroundButton, "Released", "SelectUnitClick");
        if (playerInfo !is null)
        {
            backgroundButton.color = playerInfo.color;
        }
        else
            backgroundButton.color = Color (0.5f, 0.5f, 0.5f, 1.0f);
    }

    ClientUi ()
    {
        isSceneLoaded_ = false;
        untilSelectionUpdate_ = 0.001f;
    }

    ~ClientUi ()
    {

    }

    void Start ()
    {
        ui.root.RemoveAllChildren ();
        style_ = cache.GetResource ("XMLFile", "UI/DefaultStyle.xml");
        billboardXML_ = cache.GetResource ("XMLFile", "UI/Billboard.xml");
        unitIconXML_ = cache.GetResource ("XMLFile", "UI/UnitIcon.xml");
        ui.root.defaultStyle = style_;

        billboardsRoot_ = ui.root.CreateChild ("UIElement");
        UIElement @uiRoot = ui.LoadLayout (cache.GetResource ("XMLFile", "UI/Ingame.xml"));
        ui.root.AddChild (uiRoot);
        uiRoot.defaultStyle = style_;
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
        untilSelectionUpdate_ -= timeStep;

        Text @playerStatsText = ui.root.GetChild ("ingame").GetChild ("playerStatsInfo");
        String playerStatsInfo = "";
        playerStatsInfo += node.parent.vars ["playerName"].GetString () + "\n";
        playerStatsInfo += "Gold: " + node.parent.vars ["gold"].GetFloat () + "\n";
        playerStatsInfo += "Points: " + node.parent.vars ["points"].GetFloat ();
        playerStatsText.text = playerStatsInfo;

        if (!isSceneLoaded_)
            isSceneLoaded_ = CheckIsSceneLoaded (scene);
        else
        {
            ProcessDistrictBillboards ();
            if (untilSelectionUpdate_ <= 0.0f)
            {
                UpdateSelection ();
                untilSelectionUpdate_ = 1.0f / 30.0f;
            }
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

    void SelectUnitClick (StringHash eventType, VariantMap &eventData)
    {
        UIElement @element = eventData ["Element"].GetPtr ();
        StringHash unitHash = element.vars ["unitHash"].GetStringHash ();
        node.parent.vars ["selectionType"] = StringHash ("Unit");
        node.parent.vars ["selectedHash"] = unitHash;
    }
};
