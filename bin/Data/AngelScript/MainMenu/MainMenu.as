class MainMenu : ScriptObject
{
    protected ActivitiesApplication @activitiesApplication_;
    protected int NEW_GAME_SERVER_PORT = 13768;

    protected void SetupBackground ()
    {
        scene.CreateComponent ("Octree");
        XMLFile @backgroundXML = cache.GetResource ("XMLFile", "Objects/MainMenuBackground.xml");
        Node @backgroundNode = node.CreateChild ("background", LOCAL);
        backgroundNode.LoadXML (backgroundXML.root);

        Node @cameraNode = node.GetChild ("background").GetChild ("camera");
        Camera @camera = cameraNode.GetComponent ("Camera");
        Viewport @viewport = Viewport (scene, camera);
        renderer.viewports [0] = viewport;

        XMLFile @renderPathAddition = cache.GetResource ("XMLFile", "RenderPath/Ingame_DeferredHWDepth.xml");
        renderer.viewports [0].renderPath.Load (renderPathAddition);
        renderer.viewports [0].renderPath.SetEnabled ("MapMask", false);
    }

    protected void AddScripts ()
    {
        ScriptInstance @primaryMenuInstance = node.CreateChild ("PrimaryMenu", LOCAL).CreateComponent ("ScriptInstance");
        primaryMenuInstance.CreateObject (cache.GetResource ("ScriptFile",
                                                         "AngelScript/MainMenu/UiHandlers/PrimaryMenu.as"),
                                          "PrimaryMenu");

        ScriptInstance @startGameMenuInstance = node.CreateChild ("StartGameMenu", LOCAL).CreateComponent ("ScriptInstance");
        startGameMenuInstance.CreateObject (cache.GetResource ("ScriptFile",
                                                         "AngelScript/MainMenu/UiHandlers/StartGameMenu.as"),
                                            "StartGameMenu");

        ScriptInstance @joinGameMenuInstance = node.CreateChild ("JoinGameMenu", LOCAL).CreateComponent ("ScriptInstance");
        joinGameMenuInstance.CreateObject (cache.GetResource ("ScriptFile",
                                                         "AngelScript/MainMenu/UiHandlers/JoinGameMenu.as"),
                                           "JoinGameMenu");

        ScriptInstance @mapsListInstance = node.CreateChild ("MapsList", LOCAL).CreateComponent ("ScriptInstance");
        mapsListInstance.CreateObject (cache.GetResource ("ScriptFile",
                                                         "AngelScript/MainMenu/UiHandlers/MapsList.as"),
                                       "MapsList");

        ScriptInstance @uiResizerInstance = node.CreateChild ("UiResizer", LOCAL).CreateComponent ("ScriptInstance");
        uiResizerInstance.CreateObject (cache.GetResource ("ScriptFile",
                                                         "AngelScript/Utils/UiResizer.as"),
                                        "UiResizer");
        uiResizerInstance.SetAttribute ("startElementName_", Variant ("mainMenu"));

        ScriptInstance @screenShotTakerInstance = node.CreateChild ("ScreenShotTaker", LOCAL).CreateComponent ("ScriptInstance");
        screenShotTakerInstance.CreateObject (cache.GetResource ("ScriptFile",
                                                         "AngelScript/Utils/ScreenShotTaker.as"),
                                              "ScreenShotTaker");
    }

    MainMenu ()
    {

    }

    ~MainMenu ()
    {

    }

    void Start ()
    {
        node.AddTag ("ScriptMain");
        node.vars ["serverPort"] = Variant (NEW_GAME_SERVER_PORT);
        ui.root.RemoveAllChildren ();
        XMLFile@ style = cache.GetResource ("XMLFile", "UI/ColonizationUIStyle.xml");
        ui.root.defaultStyle = style;

        UIElement @uiRoot = ui.LoadLayout (cache.GetResource ("XMLFile", "UI/MainMenu.xml"));
        ui.root.AddChild (uiRoot);
        uiRoot.name = "mainMenu";
        AddScripts ();
        SetupBackground ();

        SubscribeToEvent ("StartGameRequest", "HandleStartGameRequest");
        SubscribeToEvent ("JoinGameRequest", "HandleJoinGameRequest");
        SubscribeToEvent ("ExitRequest", "HandleExitRequest");
        SubscribeToEvent ("ShowWindowRequest", "HandleShowWindowRequest");

        VariantMap eventData;
        eventData ["windowName"] = Variant ("primaryMenu");
        SendEvent ("ShowWindowRequest", eventData);
    }

    void Update (float timeStep)
    {

    }

    void Stop ()
    {

    }

    ActivitiesApplication @get_activitiesApplication ()
    {
        return activitiesApplication_;
    }

    void set_activitiesApplication (ActivitiesApplication @activitiesApplication)
    {
        activitiesApplication_ = activitiesApplication;
    }

    void HandleStartGameRequest (StringHash eventType, VariantMap &eventData)
    {
        for (int index = 0; index < activitiesApplication_.GetActivitiesCount (); index++)
        {
            activitiesApplication_.StopActivityNextFrame (activitiesApplication_.GetActivityByIndex (index));
        }

        HostActivity @hostActivity = HostActivity ();
        hostActivity.serverPort = NEW_GAME_SERVER_PORT;
        hostActivity.mapFolder = eventData ["selectedMapFolder"].GetString ();
        hostActivity.mapInfoPath = eventData ["selectedMapInfo"].GetString ();
        activitiesApplication_.SetupActivityNextFrame (hostActivity);

        IngameClientActivity @ingameClientActivity = IngameClientActivity ();
        ingameClientActivity.serverAdress = "localhost";
        ingameClientActivity.serverPort = NEW_GAME_SERVER_PORT;
        ingameClientActivity.playerName = eventData ["nickname"].GetString ();
        ingameClientActivity.playerColor = eventData ["color"].GetColor ();
        activitiesApplication_.SetupActivityNextFrame (ingameClientActivity);
    }

    void HandleJoinGameRequest (StringHash eventType, VariantMap &eventData)
    {
        for (int index = 0; index < activitiesApplication_.GetActivitiesCount (); index++)
        {
            activitiesApplication_.StopActivityNextFrame (activitiesApplication_.GetActivityByIndex (index));
        }

        IngameClientActivity @ingameClientActivity = IngameClientActivity ();
        ingameClientActivity.serverAdress = eventData ["adress"].GetString ();
        ingameClientActivity.serverPort = eventData ["port"].GetInt ();
        ingameClientActivity.playerName = eventData ["nickname"].GetString ();
        ingameClientActivity.playerColor = eventData ["color"].GetColor ();
        activitiesApplication_.SetupActivityNextFrame (ingameClientActivity);
    }

    void HandleExitRequest ()
    {
        for (int index = 0; index < activitiesApplication_.GetActivitiesCount (); index++)
        {
            activitiesApplication_.StopActivityNextFrame (activitiesApplication_.GetActivityByIndex (index));
        }
        engine.Exit ();
    }

    void HandleShowWindowRequest (StringHash eventType, VariantMap &eventData)
    {
        UIElement @uiRoot = ui.root.GetChild ("mainMenu");
        Array <UIElement @> elements = uiRoot.GetChildren ();
        for (int index = 0; index < elements.length; index++)
        {
            elements [index].visible = false;
        }
        uiRoot.GetChild (eventData ["windowName"].GetString ()).visible = true;
    }
};
