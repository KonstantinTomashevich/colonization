#include "AngelScript/Utils/Constants.as"
#include "AngelScript/Utils/ClientUtils.as"

class MapPreviewWindow : ScriptObject
{
    MapPreviewWindow ()
    {

    }

    ~MapPreviewWindow ()
    {

    }

    void Start ()
    {
        Window @mapPreviewWindow = ui.root.GetChild ("waitingForStart").GetChild ("mapPreviewWindow");
        Button @selectMapButton = mapPreviewWindow.GetChild ("selectMapButton");
        Button @showMapInfoButton = mapPreviewWindow.GetChild ("showMapInfoButton");

        SubscribeToEvent (selectMapButton, "Released", "HandleSelectMapClick");
        SubscribeToEvent (showMapInfoButton, "Released", "HandleShowMapInfoClick");
        ScanForMaps ();
    }

    void Update (float timeStep)
    {
        Window @mapPreviewWindow = ui.root.GetChild ("waitingForStart").GetChild ("mapPreviewWindow");
        BorderImage @preview = mapPreviewWindow.GetChild ("mapPreview");
        Text @mapLabel = mapPreviewWindow.GetChild ("mapLabel");

        String selectedMapFolder = scene.vars [VAR_MAP_FOLDER].GetString ();
        XMLFile @infoXML = cache.GetResource ("XMLFile", scene.vars [VAR_MAP_INFO_PATH].GetString ());
        if (infoXML !is null)
        {
            XMLElement filesInfo = infoXML.GetRoot ().GetChild ("mapFiles");
            XMLElement userInfo = infoXML.GetRoot ().GetChild ("userInformation");

            mapLabel.text = "Map: " + userInfo.GetAttribute ("name") + ".";
            preview.texture = cache.GetResource ("Texture2D", selectedMapFolder + filesInfo.GetAttribute ("preview"));
        }

        Node @scriptMain = GetScriptMain (node);
        if (scriptMain !is null)
        {
            Button @selectMapButton = mapPreviewWindow.GetChild ("selectMapButton");
            selectMapButton.visible = scriptMain.vars [ScriptMainVars::IS_ADMIN].GetBool ();
        }
    }

    void Stop ()
    {
        UnsubscribeFromAllEvents ();
    }

    void HandleSelectMapClick ()
    {
        Node @scriptMain = GetScriptMain (node);
        if (scriptMain !is null and scriptMain.vars [ScriptMainVars::IS_ADMIN].GetBool ())
        {
            VariantMap eventData;
            eventData [ShowFunctionalWindowRequest::WINDOW_NAME] = Variant ("mapsListWindow");
            SendEvent (EVENT_SHOW_FUNCTIONAL_WINDOW_REQUEST, eventData);
        }
    }

    void HandleShowMapInfoClick ()
    {
        VariantMap eventData;
        eventData [ShowFunctionalWindowRequest::WINDOW_NAME] = Variant ("mapInfoWindow");
        SendEvent (EVENT_SHOW_FUNCTIONAL_WINDOW_REQUEST, eventData);
    }

    protected void ScanForMaps ()
    {
        Array <String> mapsFolders;
        mapsFolders = GetMapsFoldersNames ();
        Array <String> mapsNamesList;
        for (uint index = 0; index < mapsFolders.length; index++)
        {
            XMLFile @infoXML = cache.GetResource ("XMLFile", MAPS_FOLDER + mapsFolders [index] + MAP_INFO_FILE);
            XMLElement userInfo = infoXML.GetRoot ().GetChild ("userInformation");
            String name = userInfo.GetAttribute ("name");
            mapsNamesList.Push (name);
        }

        GetScriptMain (node).vars ["mapsFoldersList"] = mapsFolders;
        GetScriptMain (node).vars ["mapsList"] = mapsNamesList;
    }
}
