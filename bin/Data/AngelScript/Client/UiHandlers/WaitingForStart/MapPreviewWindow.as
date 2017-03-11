#include "AngelScript/Utils/ClientUtils.as"

class MapPreviewWindow : ScriptObject
{
    protected void ScanForMaps ()
    {
        Array <String> mapsFolders;
        mapsFolders = GetMapsFoldersNames ();
        Array <String> mapsNamesList;
        for (int index = 0; index < mapsFolders.length; index++)
        {
            XMLFile @infoXML = cache.GetResource ("XMLFile", MAPS_FOLDER + mapsFolders [index] + MAP_INFO_FILE);
            XMLElement userInfo = infoXML.GetRoot ().GetChild ("userInformation");
            String name = userInfo.GetAttribute ("name");
            mapsNamesList.Push (name);
        }

        GetScriptMain (node).vars ["mapsFoldersList"] = mapsFolders;
        GetScriptMain (node).vars ["mapsList"] = mapsNamesList;
    }

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

        String selectedMapFolder = scene.vars ["MapFolder"].GetString ();
        XMLFile @infoXML = cache.GetResource ("XMLFile", scene.vars ["MapInfoPath"].GetString ());
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
            selectMapButton.visible = scriptMain.vars ["isAdmin"].GetBool ();
        }
    }

    void Stop ()
    {
        UnsubscribeFromAllEvents ();
    }

    void HandleSelectMapClick ()
    {
        Node @scriptMain = GetScriptMain (node);
        if (scriptMain !is null and scriptMain.vars ["isAdmin"].GetBool ())
        {
            VariantMap eventData;
            eventData ["windowName"] = Variant ("mapsListWindow");
            SendEvent ("ShowFunctionalWindowRequest", eventData);
        }
    }

    void HandleShowMapInfoClick ()
    {
        VariantMap eventData;
        eventData ["windowName"] = Variant ("mapInfoWindow");
        SendEvent ("ShowFunctionalWindowRequest", eventData);
    }
}
