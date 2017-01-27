class MainMenu : ScriptObject
{
    protected ActivitiesApplication @activitiesApplication_;

    MainMenu ()
    {

    }

    ~MainMenu ()
    {

    }

    void Start ()
    {
        ui.root.RemoveAllChildren ();
        XMLFile@ style = cache.GetResource ("XMLFile", "UI/DefaultStyle.xml");
        ui.root.defaultStyle = style;

        UIElement @uiRoot = ui.LoadLayout (cache.GetResource ("XMLFile", "UI/MainMenu.xml"));
        ui.root.AddChild (uiRoot);
        uiRoot.name = "main_menu";

        Button @startGameButton = uiRoot.GetChild ("startGameButton");
        Button @exitButton = uiRoot.GetChild ("exitButton");

        SubscribeToEvent (startGameButton, "Released", "HandleStartGameClick");
        SubscribeToEvent (exitButton, "Released", "HandleExitClick");
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
            activitiesApplication_.StopActivityNextFrame (activitiesApplication_.GetActivityByIndex (index));

        HostActivity @hostActivity = HostActivity ();
        hostActivity.serverPort = 13768;
        activitiesApplication_.SetupActivityNextFrame (hostActivity);

        IngameClientActivity @ingameClientActivity = IngameClientActivity ();
        ingameClientActivity.serverAdress = "localhost";
        ingameClientActivity.serverPort = 13768;
        ingameClientActivity.playerName = "Konstant";
        ingameClientActivity.playerColor = Color (0.1f, 0.1f, 0.7f, 1.0f);
        activitiesApplication_.SetupActivityNextFrame (ingameClientActivity);
    }

    void HandleExitClick ()
    {
        for (int index = 0; index < activitiesApplication_.GetActivitiesCount (); index++)
            activitiesApplication_.StopActivityNextFrame (activitiesApplication_.GetActivityByIndex (index));
        engine.Exit ();
    }
};
