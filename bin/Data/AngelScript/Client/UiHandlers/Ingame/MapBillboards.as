#include "AngelScript/Utils/Constants.as"
#include "AngelScript/Utils/ClientUtils.as"

class MapBillboards : ScriptObject
{
    protected UIElement @billboardsRoot_;
    protected XMLFile @style_;
    protected XMLFile @billboardXML_;
    protected XMLFile @unitIconXML_;
    protected bool isSceneLoaded_;

    protected Vector3 BILLBOARD_WORLD_POSITION_OFFSET = Vector3 (0.0f, 1.0f, 0.0f);
    protected Vector2 BILLBOARD_MIN_SCREEN_POINT = Vector2 (-0.2f, -0.2f);
    protected Vector2 BILLBOARD_MAX_SCREEN_POINT = Vector2 (1.2f, 1.2f);

    protected void ProcessMapBillboards ()
    {
        Map @map = scene.GetChild ("map").GetComponent ("Map");
        Node @cameraNode = scene.GetChild ("camera");
        uint nextBillboardIndex = 0;

        if (cameraNode !is null)
        {
            Camera @camera = cameraNode.GetComponent ("Camera");
            for (uint index = 0; index < map.GetDistrictsCount (); index++)
            {
                District @district = map.GetDistrictByIndex (index);
                Vector2 screenPoint = camera.WorldToScreenPoint (district.unitPosition + BILLBOARD_WORLD_POSITION_OFFSET);
                if (screenPoint.x > BILLBOARD_MIN_SCREEN_POINT.x and screenPoint.x < BILLBOARD_MAX_SCREEN_POINT.x and
                    screenPoint.y > BILLBOARD_MIN_SCREEN_POINT.y and screenPoint.y < BILLBOARD_MAX_SCREEN_POINT.y)
                {
                    if (nextBillboardIndex == billboardsRoot_.GetNumChildren (false))
                    {
                        billboardsRoot_.LoadChildXML (billboardXML_.GetRoot (), style_);
                    }

                    UIElement @billboard = billboardsRoot_.GetChildren () [nextBillboardIndex];
                    billboard.SetPosition (int (screenPoint.x * graphics.width - billboard.width / 2),
                        int (screenPoint.y * graphics.height - billboard.height / 2));

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
        String titleText = district.name + "\n";
        if (district.isSea)
        {
            titleText += "[under water]";
        }
        else if (district.isImpassable)
        {
            titleText += "[impassable]";
        }
        else if (district.hasColony)
        {
            titleText += "[colony of " + district.colonyOwnerName + "]";
        }
        else
        {
            titleText += "[can be colonized]";
        }
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
                background.color = NEUTRAL_COLOR;
            }
        }
        else
        {
            background.color = NEUTRAL_COLOR;
        }
    }

    protected void UpdateDistrictBillboardUnitsSection (UIElement @billboard, District @district)
    {
        FogOfWarCalculator @fogOfWarCalculator = scene.GetComponent ("FogOfWarCalculator");
        // TODO: This algorithm can be optimized!
        Array <Unit @> unitsInDistrict = GetUnitsInDistrict (scene, district.hash);
        UIElement @unitsElement = billboard.GetChild ("units");

        if (unitsInDistrict.length > 0 and fogOfWarCalculator.IsDistrictVisible (district.hash))
        {
            uint index = 0;
            for (index = 0; index < unitsInDistrict.length; index++)
            {
                Unit @unit = unitsInDistrict [index];
                if (index == unitsElement.GetNumChildren (false))
                {
                    unitsElement.LoadChildXML (unitIconXML_.GetRoot (), style_);
                }

                UIElement @unitElement = unitsElement.GetChildren () [index];
                unitElement.SetPosition (unitsElement.height * index, 0);

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
            typeIcon.SetStyle ("FleetIcon", style_);
        }
        else if (unit.unitType == UNIT_COLONIZATORS)
        {
            typeIcon.SetStyle ("ColonizatorsIcon", style_);
        }
        else if (unit.unitType == UNIT_TRADERS)
        {
            typeIcon.SetStyle ("TradersIcon", style_);
        }
        else if (unit.unitType == UNIT_ARMY)
        {
            typeIcon.SetStyle ("ArmyIcon", style_);
        }
    }

    protected void UpdateUnitIconButton (UIElement @unitElement, Unit @unit)
    {
        Button @backgroundButton = unitElement.GetChild ("selectButton");
        PlayerInfo @playerInfo = GetPlayerInfoByName (scene, unit.ownerPlayerName);
        backgroundButton.vars ["unitHash"] = Variant (unit.hash);

        SubscribeToEvent (backgroundButton, "Released", "HandleSelectUnitClick");
        if (playerInfo !is null)
        {
            backgroundButton.color = playerInfo.color;
        }
        else
        {
            backgroundButton.color = NEUTRAL_COLOR;
        }
    }

    MapBillboards ()
    {
        isSceneLoaded_ = false;
    }

    ~MapBillboards ()
    {

    }

    void Start ()
    {
        style_ = cache.GetResource ("XMLFile", "UI/ColonizationUIStyle.xml");
        billboardXML_ = cache.GetResource ("XMLFile", "UI/Billboard.xml");
        unitIconXML_ = cache.GetResource ("XMLFile", "UI/UnitIcon.xml");

        billboardsRoot_ = ui.root.GetChild ("billboardsRoot");
        billboardsRoot_.AddTag ("EnableUiResizer");
    }

    void Update (float timeStep)
    {
        Node @scriptMain = GetScriptMain (node);
        if (!isSceneLoaded_ and scriptMain.vars ["gameState"].GetInt () != GAME_STATE_WAITING_FOR_START)
        {
            isSceneLoaded_ = CheckIsSceneLoaded (scene);
        }
        else if (scriptMain.vars ["gameState"].GetInt () != GAME_STATE_WAITING_FOR_START and
                 scene.GetChild ("map") !is null and scene.GetChild ("units") !is null)
        {
            ProcessMapBillboards ();
        }
    }

    void Stop ()
    {
        UnsubscribeFromAllEvents ();
    }

    void HandleSelectUnitClick (StringHash eventType, VariantMap &eventData)
    {
        Node @scriptMain = GetScriptMain (node);
        UIElement @element = eventData ["Element"].GetPtr ();
        StringHash unitHash = element.vars ["unitHash"].GetStringHash ();
        scriptMain.vars ["selectionType"] = StringHash ("Unit");
        scriptMain.vars ["selectedHash"] = unitHash;
    }
}
