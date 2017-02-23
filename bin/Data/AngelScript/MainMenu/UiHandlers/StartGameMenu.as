class StartGameMenu : ScriptObject
{
    protected XMLFile @style_;
    Array <String> maps_;

    String MAPS_FOLDER = "Data/Maps/";
    String EMPTY_FILTER = "";
    String MAP_INFO_FILE = "/info.xml";

    protected void ScanForMaps ()
    {
        maps_ = fileSystem.ScanDir (MAPS_FOLDER, EMPTY_FILTER, SCAN_DIRS, false);
        // Check maps list and delete items if they aren't maps.
        int index = 0;
        while (index < maps_.length)
        {
            String mapFolder = MAPS_FOLDER + maps_ [index];
            if (fileSystem.FileExists (mapFolder + MAP_INFO_FILE))
            {
                index++;
            }
            else
            {
                maps_.Erase (index);
            }
        }

        Array <String> mapsNamesList;
        for (index = 0; index < maps_.length; index++)
        {
            XMLFile @infoXML = cache.GetResource ("XMLFile", MAPS_FOLDER + maps_ [index] + MAP_INFO_FILE);
            XMLElement userInfo = infoXML.GetRoot ().GetChild ("userInformation");
            String name = userInfo.GetAttribute ("name");
            mapsNamesList.Push (name);
        }
        node.parent.vars ["mapsList"] = mapsNamesList;
    }

    protected void UpdateMapInfo ()
    {
        Window @startGameMenu = ui.root.GetChild ("mainMenu").GetChild ("startGameMenu");
        Text @mapLabel = startGameMenu.GetChild ("mapLabel");
        Text @mapInfo = startGameMenu.GetChild ("mapInfo");
        BorderImage @preview = startGameMenu.GetChild ("mapPreview");

        int selectedMapIndex = node.parent.vars ["selectedMapIndex"].GetInt ();
        String selectedMapFolder = MAPS_FOLDER + maps_ [selectedMapIndex];
        XMLFile @infoXML = cache.GetResource ("XMLFile", selectedMapFolder + MAP_INFO_FILE);
        XMLElement filesInfo = infoXML.GetRoot ().GetChild ("mapFiles");
        XMLElement userInfo = infoXML.GetRoot ().GetChild ("userInformation");

        mapLabel.text = "Map: " + userInfo.GetAttribute ("name") + ".";
        String infoText = "Short info: " + userInfo.GetAttribute ("shortInfo") + "\n\n";
        infoText += "Maximum players: " + userInfo.GetInt ("maxPlayers") + ".\n\n";
        infoText += "Supported victory types: " + userInfo.GetAttribute ("supportedVictoryTypes") + ".";
        mapInfo.text = infoText;
        preview.texture = cache.GetResource ("Texture2D", selectedMapFolder + filesInfo.GetAttribute ("preview"));
    }

    StartGameMenu ()
    {

    }

    ~StartGameMenu ()
    {

    }

    void Start ()
    {
        style_ = cache.GetResource ("XMLFile", "UI/ColonizationUIStyle.xml");
        Window @startGameMenu = ui.root.GetChild ("mainMenu").GetChild ("startGameMenu");
        Button @startGameButton = startGameMenu.GetChild ("startGameButton");
        Button @backButton = startGameMenu.GetChild ("goBackButton");
        Button @selectMapButton = startGameMenu.GetChild ("selectMapButton");

        SubscribeToEvent (startGameButton, "Released", "HandleStartGameClick");
        SubscribeToEvent (backButton, "Released", "HandleGoBackClick");
        SubscribeToEvent (selectMapButton, "Released", "HandleSelectMapClick");
        ScanForMaps ();
    }

    void Update (float timeStep)
    {
        Window @startGameMenu = ui.root.GetChild ("mainMenu").GetChild ("startGameMenu");
        Slider @redSlider = startGameMenu.GetChild ("redSlider");
        Slider @greenSlider = startGameMenu.GetChild ("greenSlider");
        Slider @blueSlider = startGameMenu.GetChild ("blueSlider");

        float r = redSlider.value / redSlider.range;
        float g = greenSlider.value / greenSlider.range;
        float b = blueSlider.value / blueSlider.range;

        BorderImage @colorSample = startGameMenu.GetChild ("colorSample");
        colorSample.color = Color (r, g, b, 1.0f);

        Button @startGameButton = startGameMenu.GetChild ("startGameButton");
        LineEdit @nicknameEdit = startGameMenu.GetChild ("nicknameEdit");
        startGameButton.visible = (nicknameEdit.text != "");
        UpdateMapInfo ();

        int serverPort = node.parent.vars ["serverPort"].GetInt ();
        Text @serverPortLabel = startGameMenu.GetChild ("serverPortLabel");
        serverPortLabel.text = "Server will be started at port " + serverPort + ".";
    }

    void Stop ()
    {

    }

    void HandleStartGameClick ()
    {
        Window @startGameMenu = ui.root.GetChild ("mainMenu").GetChild ("startGameMenu");
        LineEdit @nicknameEdit = startGameMenu.GetChild ("nicknameEdit");
        Slider @redSlider = startGameMenu.GetChild ("redSlider");
        Slider @greenSlider = startGameMenu.GetChild ("greenSlider");
        Slider @blueSlider = startGameMenu.GetChild ("blueSlider");

        String nickname = nicknameEdit.text;
        int selectedMapIndex = node.parent.vars ["selectedMapIndex"].GetInt ();
        String selectedMapFolder = MAPS_FOLDER + maps_ [selectedMapIndex];
        String selectedMapInfo = selectedMapFolder + MAP_INFO_FILE;
        float r = redSlider.value / redSlider.range;
        float g = greenSlider.value / greenSlider.range;
        float b = blueSlider.value / blueSlider.range;

        node.parent.vars ["nickname"] = Variant (nickname);
        node.parent.vars ["color"] = Variant (Color (r, g, b, 1.0f));
        node.parent.vars ["selectedMapFolder"] = Variant (selectedMapFolder);
        node.parent.vars ["selectedMapInfo"] = Variant (selectedMapInfo);
        SendEvent ("StartGameRequest");
    }

    void HandleGoBackClick ()
    {
        ui.root.GetChild ("mainMenu").GetChild ("primaryMenu").visible = true;
        ui.root.GetChild ("mainMenu").GetChild ("startGameMenu").visible = false;
    }

    void HandleSelectMapClick ()
    {
        ui.root.GetChild ("mainMenu").GetChild ("mapsList").visible = true;
    }
}
