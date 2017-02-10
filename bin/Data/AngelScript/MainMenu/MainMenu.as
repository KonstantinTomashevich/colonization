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

    MainMenu ()
    {

    }

    ~MainMenu ()
    {

    }

    void Start ()
    {
        ui.root.RemoveAllChildren ();
        XMLFile@ style = cache.GetResource ("XMLFile", "UI/ColonizationUIStyle.xml");
        ui.root.defaultStyle = style;

        UIElement @uiRoot = ui.LoadLayout (cache.GetResource ("XMLFile", "UI/MainMenu.xml"));
        ui.root.AddChild (uiRoot);
        uiRoot.name = "main_menu_root";

        Button @startGameButton = uiRoot.GetChild ("start_game_button");
        Button @exitButton = uiRoot.GetChild ("exit_from_game_button");

        SubscribeToEvent (startGameButton, "Released", "HandleStartGameClick");
        SubscribeToEvent (exitButton, "Released", "HandleExitClick");

        ScriptInstance @uiResizerInstance = node.CreateChild ("UiResizer", LOCAL).CreateComponent ("ScriptInstance");
        uiResizerInstance.CreateObject (cache.GetResource ("ScriptFile",
                                                         "AngelScript/Utils/UiResizer.as"),
                                        "UiResizer");
        uiResizerInstance.SetAttribute ("startElementName_", Variant ("main_menu_root"));
        SetupBackground ();
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

    void HandleStartGameClick ()
    {
        for (int index = 0; index < activitiesApplication_.GetActivitiesCount (); index++)
        {
            activitiesApplication_.StopActivityNextFrame (activitiesApplication_.GetActivityByIndex (index));
        }

        HostActivity @hostActivity = HostActivity ();
        hostActivity.serverPort = NEW_GAME_SERVER_PORT;
        activitiesApplication_.SetupActivityNextFrame (hostActivity);

        IngameClientActivity @ingameClientActivity = IngameClientActivity ();
        ingameClientActivity.serverAdress = "localhost";
        ingameClientActivity.serverPort = NEW_GAME_SERVER_PORT;
        ingameClientActivity.playerName = "Konstant";
        ingameClientActivity.playerColor = Color (0.1f, 0.1f, 0.7f, 1.0f);
        activitiesApplication_.SetupActivityNextFrame (ingameClientActivity);
    }

    void HandleExitClick ()
    {
        for (int index = 0; index < activitiesApplication_.GetActivitiesCount (); index++)
        {
            activitiesApplication_.StopActivityNextFrame (activitiesApplication_.GetActivityByIndex (index));
        }
        engine.Exit ();
    }
};
