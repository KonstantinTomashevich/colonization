#include "AngelScript/Utils/ClientUtils.as"

class Client : ScriptObject
{
    protected ActivitiesApplication @activitiesApplication_;
    protected String playerName_;
    protected bool isSceneLoaded_;

    Client ()
    {
        isSceneLoaded_ = false;
    }

    ~Client ()
    {

    }

    void Init ()
    {

    }

    void Start ()
    {
        XMLFile@ style = cache.GetResource ("XMLFile", "UI/DefaultStyle.xml");
        ui.root.defaultStyle = style;

        Node @uiScriptNode = node.CreateChild ("uiScriptNode", LOCAL);
        ScriptInstance @uiScript = uiScriptNode.CreateComponent ("ScriptInstance", LOCAL);
        uiScript.CreateObject (cache.GetResource ("ScriptFile",
                                                         "AngelScript/Client/ClientUi.as"),
                                     "ClientUi");

        Node @networkScriptNode = node.CreateChild ("networkScriptNode", LOCAL);
        ScriptInstance @networkScript = networkScriptNode.CreateComponent ("ScriptInstance", LOCAL);
        networkScript.CreateObject (cache.GetResource ("ScriptFile",
                                                         "AngelScript/Client/ClientNetwork.as"),
                                     "ClientNetwork");

        Node @sceneManagerScriptNode = node.CreateChild ("sceneManagerScriptNode", LOCAL);
        ScriptInstance @sceneManagerScript = sceneManagerScriptNode.CreateComponent ("ScriptInstance", LOCAL);
        sceneManagerScript.CreateObject (cache.GetResource ("ScriptFile",
                                                         "AngelScript/Client/SceneManager.as"),
                                     "SceneManager");

        Node @screenPressesHandlerScriptNode = node.CreateChild ("screenPressesHandlerScriptNode", LOCAL);
        ScriptInstance @screenPressesHandlerScript = screenPressesHandlerScriptNode.CreateComponent ("ScriptInstance", LOCAL);
        screenPressesHandlerScript.CreateObject (cache.GetResource ("ScriptFile",
                                                         "AngelScript/Client/ScreenPressesHandler.as"),
                                     "ScreenPressesHandler");

        SubscribeToEvent ("ServerDisconnected", "HandleServerDisconnected");
        SubscribeToEvent ("ConnectFailed", "HandleConnectFailed");
    }

    void Update (float timeStep)
    {
        if (node.vars ["goToMenuCalled"].GetBool ())
            GoToMainMenuState ();

        if (!isSceneLoaded_)
            isSceneLoaded_ = CheckIsSceneLoaded (scene);
    }

    void Stop ()
    {
        UnsubscribeFromAllEvents ();
    }

    ActivitiesApplication @get_activitiesApplication ()
    {
        return activitiesApplication_;
    }

    void set_activitiesApplication (ActivitiesApplication @activitiesApplication)
    {
        activitiesApplication_ = activitiesApplication;
    }

    String get_playerName ()
    {
        return playerName_;
    }

    void set_playerName (String playerName)
    {
        playerName_ = playerName;
        node.vars ["playerName"] = playerName_;
    }

    void HandleServerDisconnected ()
    {
        ErrorDialog ("Disconnected from server!", "Disconnected from server!\nPress OK to return to main menu.");
        GoToMainMenuState ();
    }

    void GoToMainMenuState ()
    {
        for (int index = 0; index < activitiesApplication_.GetActivitiesCount (); index++)
            activitiesApplication_.StopActivityNextFrame (activitiesApplication_.GetActivityByIndex (index));

        MainMenuActivity @mainMenuActivity = MainMenuActivity ();
        activitiesApplication_.SetupActivityNextFrame (mainMenuActivity);
    }

    void HandleConnectFailed ()
    {
        ErrorDialog ("Connect failed!", "Connect failed!\nPress OK to return to main menu.");
        GoToMainMenuState ();
    }
};
