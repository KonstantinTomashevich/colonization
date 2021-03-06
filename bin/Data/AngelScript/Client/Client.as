#include "AngelScript/Utils/Constants.as"
#include "AngelScript/Utils/ClientUtils.as"

class Client : ScriptObject
{
    protected ActivitiesApplication @activitiesApplication_;

    Client ()
    {

    }

    ~Client ()
    {

    }

    void Init ()
    {

    }

    void Start ()
    {
        node.AddTag ("ScriptMain");
        AddCoreScripts ();

        SubscribeToEvent ("ServerDisconnected", "HandleServerDisconnected");
        SubscribeToEvent ("ConnectFailed", "HandleConnectFailed");
        SubscribeToEvent (EVENT_GO_TO_MAIN_MENU_REQUEST, "HandleGoToMainMenuRequest");
    }

    void Update (float timeStep)
    {

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
        return node.vars [ScriptMainVars::PLAYER_NAME].GetString ();
    }

    void set_playerName (String playerName)
    {
        node.vars [ScriptMainVars::PLAYER_NAME] = playerName;
        log.Info ("PlayerName setted");
        log.Info (node.vars [ScriptMainVars::PLAYER_NAME].GetString ());
    }

    bool get_isAdmin ()
    {
        return node.vars [ScriptMainVars::IS_ADMIN].GetBool ();
    }

    void set_isAdmin (bool isAdmin)
    {
        node.vars [ScriptMainVars::IS_ADMIN] = Variant (isAdmin);
    }

    void HandleServerDisconnected ()
    {
        ErrorDialog ("Disconnected from server!", "Disconnected from server!\nPress OK to return to main menu.");
        GoToMainMenuState ();
    }

    void GoToMainMenuState ()
    {
        for (uint index = 0; index < activitiesApplication_.activitiesCount; index++)
        {
            activitiesApplication_.StopActivityNextFrame (activitiesApplication_.GetActivityByIndex (index));
        }

        MainMenuActivity @mainMenuActivity = MainMenuActivity ();
        activitiesApplication_.SetupActivityNextFrame (mainMenuActivity);
    }

    void HandleConnectFailed ()
    {
        ErrorDialog ("Connect failed!", "Connect failed!\nPress OK to return to main menu.");
        GoToMainMenuState ();
    }

    void HandleGoToMainMenuRequest ()
    {
        GoToMainMenuState ();
    }

    protected void AddCoreScripts ()
    {
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

        ScriptInstance @screenShotTakerInstance = node.CreateChild ("ScreenShotTaker", LOCAL).CreateComponent ("ScriptInstance");
        screenShotTakerInstance.CreateObject (cache.GetResource ("ScriptFile",
                                                         "AngelScript/Utils/ScreenShotTaker.as"),
                                              "ScreenShotTaker");
    }
};
