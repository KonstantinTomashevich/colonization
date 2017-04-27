#include "AngelScript/Utils/Constants.as"
#include "AngelScript/Utils/ClientUtils.as"

class DistrictSelectedWindow : ScriptObject
{
    protected bool isSceneLoaded_;
    protected float untilSelectionUpdate_;
    protected float SELECTION_UPDATE_DELAY = 0.02f;

    protected void UpdateDistrictSelection ()
    {
        Node @scriptMain = GetScriptMain (node);
        Window @districtInfoWindow = ui.root.GetChild ("ingame").GetChild ("districtInfoWindow");
        districtInfoWindow.visible = true;

        Map @map = scene.GetChild ("map").GetComponent ("Map");
        GameConfiguration @configuration = scene.GetComponent ("GameConfiguration");
        StringHash districtHash = scriptMain.vars ["selectedHash"].GetStringHash ();
        District @district = map.GetDistrictByHash (districtHash);

        String playerName = scriptMain.vars ["playerName"].GetString ();
        Text@ nameText = districtInfoWindow.GetChild ("nameText");
        nameText.text = district.name;

        districtInfoWindow.GetChild ("resourcesInfoButton").visible = !district.isSea;
        districtInfoWindow.GetChild ("populationInfoButton").visible = !district.isSea;
        districtInfoWindow.GetChild ("colonyEvolutionInfoButton").visible = district.hasColony;

        StringHash infoType = districtInfoWindow.vars ["infoType"].GetStringHash ();
        UIElement @investButtons = districtInfoWindow.GetChild ("investButtons");
        investButtons.visible = (infoType == StringHash ("ColonyEvolution") and
                                 district.hasColony and
                                 scriptMain.vars ["gold"].GetFloat () >= DEFAULT_INVESTITION_SIZE and
                                 district.colonyOwnerName == playerName);

        Button @sendColonizatorsButton = districtInfoWindow.GetChild ("sendColonizatorsButton");
        float colonizatorsExpeditionCost = COLONIZATORS_EXPEDITION_SIZE * configuration.oneColonizatorSendingCost;
        sendColonizatorsButton.visible = (infoType == StringHash ("Basic")) and
                                         not district.isSea and
                                         not district.isImpassable and
                                         (district.colonyOwnerName == playerName or not district.hasColony) and
                                         scriptMain.vars ["gold"].GetFloat () > colonizatorsExpeditionCost;
        Text @sendColonizatorsButtonText = sendColonizatorsButton.GetChild ("text");
        sendColonizatorsButtonText.text = "Send 100 colonizators (cost: " + int (colonizatorsExpeditionCost) +
                                      " gold).";


        Button @buildWarShipButton = districtInfoWindow.GetChild ("buildWarShipButton");
        buildWarShipButton.visible = (infoType == StringHash ("Basic")) and
                                         district.hasColony and
                                         district.colonyOwnerName == playerName and
                                         district.menCount > configuration.oneWarShipCrew and
                                         scriptMain.vars ["gold"].GetFloat () > configuration.oneWarShipBuildingCost;
        Text @buildWarShipButtonText = buildWarShipButton.GetChild ("text");
        buildWarShipButtonText.text = "Build war ship (cost: " + int (configuration.oneWarShipBuildingCost) +
                                      " + " + int (configuration.oneWarShipCrew) + " men as crew).";

        Button @openDiplomacyButton = districtInfoWindow.GetChild ("openDiplomacyButton");
        openDiplomacyButton.visible = (infoType == StringHash ("Basic")) and
                                         district.hasColony and
                                         district.colonyOwnerName != playerName;

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
        infoText += "Farming square: " + Floor (district.farmingSquare) + ".\n";
        infoText += "Land average fertility: " + Floor (district.landAverageFertility * 100) + "%.\n";

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

        infoText += "Forest square: " + Floor (district.forestsSquare) + ".\n";
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

    DistrictSelectedWindow ()
    {
        isSceneLoaded_ = false;
        untilSelectionUpdate_ = 0.0f;
    }

    ~DistrictSelectedWindow ()
    {

    }

    void Start ()
    {
        Window @districtInfoWindow = ui.root.GetChild ("ingame").GetChild ("districtInfoWindow");
        districtInfoWindow.vars ["infoType"] = StringHash ("Basic");

        Button @basicInfoButton = districtInfoWindow.GetChild ("basicInfoButton");
        Button @resourcesInfoButton = districtInfoWindow.GetChild ("resourcesInfoButton");
        Button @populationInfoButton = districtInfoWindow.GetChild ("populationInfoButton");
        Button @colonyEvolutionInfoButton = districtInfoWindow.GetChild ("colonyEvolutionInfoButton");
        Button @sendColonizatorsButton = districtInfoWindow.GetChild ("sendColonizatorsButton");
        Button @buildWarShipButton = districtInfoWindow.GetChild ("buildWarShipButton");

        UIElement @investButtons = districtInfoWindow.GetChild ("investButtons");
        Button @investToFarmsButton = investButtons.GetChild ("investToFarms");
        Button @investToMinesButton = investButtons.GetChild ("investToMines");
        Button @investToIndustryButton = investButtons.GetChild ("investToIndustry");
        Button @investToLogisticsButton = investButtons.GetChild ("investToLogistics");
        Button @investToDefenseButton = investButtons.GetChild ("investToDefense");

        SubscribeToEvent (basicInfoButton, "Released", "HandleBasicInfoClick");
        SubscribeToEvent (resourcesInfoButton, "Released", "HandleResourcesInfoClick");
        SubscribeToEvent (populationInfoButton, "Released", "HandlePopulationInfoClick");
        SubscribeToEvent (colonyEvolutionInfoButton, "Released", "HandleColonyEvolutionInfoClick");
        SubscribeToEvent (sendColonizatorsButton, "Released", "HandleSendColonizatorsClick");
        SubscribeToEvent (buildWarShipButton, "Released", "HandleBuildWarShipClick");

        SubscribeToEvent (investToFarmsButton, "Released", "HandleInvestClick");
        SubscribeToEvent (investToMinesButton, "Released", "HandleInvestClick");
        SubscribeToEvent (investToIndustryButton, "Released", "HandleInvestClick");
        SubscribeToEvent (investToLogisticsButton, "Released", "HandleInvestClick");
        SubscribeToEvent (investToDefenseButton, "Released", "HandleInvestClick");
    }

    void Update (float timeStep)
    {
        Node @scriptMain = GetScriptMain (node);
        if (!isSceneLoaded_ and scriptMain.vars ["gameState"].GetInt () != GAME_STATE_WAITING_FOR_START)
        {
            isSceneLoaded_ = CheckIsSceneLoaded (scene);
        }
        else if (isSceneLoaded_)
        {
            untilSelectionUpdate_ -= timeStep;
            if (untilSelectionUpdate_ <= 0.0f)
            {
                Window @districtInfoWindow = ui.root.GetChild ("ingame").GetChild ("districtInfoWindow");
                StringHash selectionType = scriptMain.vars ["selectionType"].GetStringHash ();

                if (selectionType == StringHash ("District"))
                {
                    UpdateDistrictSelection ();
                }
                else
                {
                    districtInfoWindow.visible = false;
                }
                untilSelectionUpdate_ = SELECTION_UPDATE_DELAY;
            }
        }
    }

    void Stop ()
    {
        UnsubscribeFromAllEvents ();
    }

    void HandleSendColonizatorsClick ()
    {
        Node @scriptMain = GetScriptMain (node);
        StringHash districtHash = scriptMain.vars ["selectedHash"].GetStringHash ();
        VectorBuffer buffer = VectorBuffer ();
        buffer.WriteInt (PLAYER_ACTION_REQUEST_COLONIZATORS_FROM_EUROPE);
        buffer.WriteStringHash (districtHash);
        buffer.WriteInt (COLONIZATORS_EXPEDITION_SIZE);

        VariantMap eventData;
        eventData ["taskType"] = Variant (CTS_NETWORK_MESSAGE_SEND_PLAYER_ACTION);
        eventData ["messageBuffer"] = Variant (buffer);
        SendEvent ("NewNetworkTask", eventData);
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
        Node @scriptMain = GetScriptMain (node);
        UIElement @element = eventData ["Element"].GetPtr ();
        StringHash investTypeHash = StringHash (element.vars ["investitionType"].GetString ());
        StringHash districtHash = scriptMain.vars ["selectedHash"].GetStringHash ();

        VectorBuffer buffer = VectorBuffer ();
        buffer.WriteInt (PLAYER_ACTION_INVEST_TO_COLONY);
        buffer.WriteStringHash (districtHash);
        buffer.WriteStringHash (investTypeHash);
        buffer.WriteFloat (DEFAULT_INVESTITION_SIZE);

        VariantMap taskData;
        eventData ["taskType"] = Variant (CTS_NETWORK_MESSAGE_SEND_PLAYER_ACTION);
        eventData ["messageBuffer"] = Variant (buffer);
        SendEvent ("NewNetworkTask", eventData);
    }

    void HandleBuildWarShipClick ()
    {
        Node @scriptMain = GetScriptMain (node);
        scriptMain.vars ["currentClickCommand"] = StringHash ("BuildWarShip");
    }
}
