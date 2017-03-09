#include "AngelScript/Utils/ClientUtils.as"

class ClientUi : ScriptObject
{
    protected XMLFile @style_;
    protected bool isSceneLoaded_;

    protected void AddUtilHandlers ()
    {
        Node @utilHandlersNode = node.GetChild ("utilHandlers");
        ScriptInstance @uiResizerInstance = utilHandlersNode.CreateChild ("UiResizer", LOCAL).CreateComponent ("ScriptInstance");
        uiResizerInstance.CreateObject (cache.GetResource ("ScriptFile",
                                                         "AngelScript/Utils/UiResizer.as"),
                                        "UiResizer");
        uiResizerInstance.SetAttribute ("startElementName_", Variant ("UIRoot"));
        uiResizerInstance.SetAttribute ("continuousResize_", Variant (true));

        ScriptInstance @uiPlayerColorPainterInstance = utilHandlersNode.CreateChild ("UiPlayerColorPainter", LOCAL).CreateComponent ("ScriptInstance");
        uiPlayerColorPainterInstance.CreateObject (cache.GetResource ("ScriptFile",
                                                         "AngelScript/Utils/UiPlayerColorPainter.as"),
                                        "UiPlayerColorPainter");
    }

    protected void AddWaitingForStartStateUiHandlers ()
    {
        Node @uiHandlersNode = node.GetChild ("uiHandlers");
        uiHandlersNode.RemoveAllChildren ();
        ui.root.RemoveAllChildren ();

        UIElement @uiRoot = ui.LoadLayout (cache.GetResource ("XMLFile", "UI/WaitingForStart.xml"));
        ui.root.AddChild (uiRoot);
        uiRoot.defaultStyle = style_;
        uiRoot.name = "waitingForStart";

        ScriptInstance @chatWindowInstance = uiHandlersNode.CreateChild ("ChatWindow", LOCAL).CreateComponent ("ScriptInstance");
        chatWindowInstance.CreateObject (cache.GetResource ("ScriptFile",
                                                      "AngelScript/Client/UiHandlers/WaitingForStart/ChatWindow.as"),
                                         "ChatWindow");

        ScriptInstance @functionalWindowsShowerInstance = uiHandlersNode.CreateChild ("FunctionalWindowsShower", LOCAL).CreateComponent ("ScriptInstance");
        functionalWindowsShowerInstance.CreateObject (cache.GetResource ("ScriptFile",
                                                      "AngelScript/Client/UiHandlers/WaitingForStart/FunctionalWindowsShower.as"),
                                         "FunctionalWindowsShower");

        ScriptInstance @playersListInstance = uiHandlersNode.CreateChild ("PlayersList", LOCAL).CreateComponent ("ScriptInstance");
        playersListInstance.CreateObject (cache.GetResource ("ScriptFile",
                                                      "AngelScript/Client/UiHandlers/WaitingForStart/PlayersList.as"),
                                         "PlayersList");
    }

    protected void AddPlayingStateUiHandlers ()
    {
        Node @uiHandlersNode = node.GetChild ("uiHandlers");
        uiHandlersNode.RemoveAllChildren ();
        ui.root.RemoveAllChildren ();

        ui.root.CreateChild ("UIElement", "billboardsRoot");
        UIElement @uiRoot = ui.LoadLayout (cache.GetResource ("XMLFile", "UI/Ingame.xml"));
        ui.root.AddChild (uiRoot);
        uiRoot.defaultStyle = style_;
        uiRoot.name = "ingame";

        uiRoot.GetChild ("chatPrivateReceiversWindow").visible = false;
        uiRoot.GetChild ("chatBlockedPlayersWindow").visible = false;

        ScriptInstance @playerInfoWindowInstance = uiHandlersNode.CreateChild ("PlayerInfoWindow", LOCAL).CreateComponent ("ScriptInstance");
        playerInfoWindowInstance.CreateObject (cache.GetResource ("ScriptFile",
                                                         "AngelScript/Client/UiHandlers/Ingame/PlayerInfoWindow.as"),
                                               "PlayerInfoWindow");

        ScriptInstance @menuWindowInstance = uiHandlersNode.CreateChild ("MenuWindow", LOCAL).CreateComponent ("ScriptInstance");
        menuWindowInstance.CreateObject (cache.GetResource ("ScriptFile",
                                                      "AngelScript/Client/UiHandlers/Ingame/MenuWindow.as"),
                                         "MenuWindow");

        ScriptInstance @mapBillboardsInstance = uiHandlersNode.CreateChild ("MapBillboards", LOCAL).CreateComponent ("ScriptInstance");
        mapBillboardsInstance.CreateObject (cache.GetResource ("ScriptFile",
                                                     "AngelScript/Client/UiHandlers/Ingame/MapBillboards.as"),
                                            "MapBillboards");

        ScriptInstance @chatWindowInstance = uiHandlersNode.CreateChild ("ChatWindow", LOCAL).CreateComponent ("ScriptInstance");
        chatWindowInstance.CreateObject (cache.GetResource ("ScriptFile",
                                                      "AngelScript/Client/UiHandlers/Ingame/ChatWindow.as"),
                                         "ChatWindow");

        ScriptInstance @unitSelectedWindowInstance = uiHandlersNode.CreateChild ("UnitSelectedWindow", LOCAL).CreateComponent ("ScriptInstance");
        unitSelectedWindowInstance.CreateObject (cache.GetResource ("ScriptFile",
                                                      "AngelScript/Client/UiHandlers/Ingame/UnitSelectedWindow.as"),
                                         "UnitSelectedWindow");

        ScriptInstance @districtSelectedWindowInstance = uiHandlersNode.CreateChild ("DistrictSelectedWindow", LOCAL).CreateComponent ("ScriptInstance");
        districtSelectedWindowInstance.CreateObject (cache.GetResource ("ScriptFile",
                                                      "AngelScript/Client/UiHandlers/Ingame/DistrictSelectedWindow.as"),
                                         "DistrictSelectedWindow");

        ScriptInstance @chatBlockedPlayersListEditorInstance = uiHandlersNode.CreateChild ("ChatBlockedPlayersListEditor", LOCAL).CreateComponent ("ScriptInstance");
        chatBlockedPlayersListEditorInstance.CreateObject (cache.GetResource ("ScriptFile",
                                                      "AngelScript/Client/UiHandlers/Ingame/ChatBlockedPlayersListEditor.as"),
                                         "ChatBlockedPlayersListEditor");


        ScriptInstance @chatPrivateReceiversListEditorInstance = uiHandlersNode.CreateChild ("ChatPrivateReceiversListEditor", LOCAL).CreateComponent ("ScriptInstance");
        chatPrivateReceiversListEditorInstance.CreateObject (cache.GetResource ("ScriptFile",
                                                      "AngelScript/Client/UiHandlers/Ingame/ChatPrivateReceiversListEditor.as"),
                                         "ChatPrivateReceiversListEditor");

        ScriptInstance @gameEndedWindowInstance = uiHandlersNode.CreateChild ("GameEndedWindow", LOCAL).CreateComponent ("ScriptInstance");
        gameEndedWindowInstance.CreateObject (cache.GetResource ("ScriptFile",
                                                         "AngelScript/Client/UiHandlers/Ingame/GameEndedWindow.as"),
                                               "GameEndedWindow");
    }

    ClientUi ()
    {
        isSceneLoaded_ = false;
    }

    ~ClientUi ()
    {

    }

    void Start ()
    {
        ui.root.RemoveAllChildren ();
        style_ = cache.GetResource ("XMLFile", "UI/ColonizationUIStyle.xml");
        ui.root.defaultStyle = style_;
        node.CreateChild ("uiHandlers", LOCAL);
        node.CreateChild ("utilHandlers", LOCAL);
        AddUtilHandlers ();
        SubscribeToEvent ("GameStateChanged", "HandleGameStateChanged");
    }

    void Update (float timeStep)
    {
        Node @scriptMain = GetScriptMain (node);
        if (!isSceneLoaded_ and scriptMain.vars ["gameState"].GetInt () != GAME_STATE_WAITING_FOR_START)
        {
            isSceneLoaded_ = CheckIsSceneLoaded (scene);
        }
        else
        {
            PlayerInfo @playerInfo = GetPlayerInfoByName (scene, scriptMain.vars ["playerName"].GetString ());
            if (playerInfo !is null)
            {
                ScriptInstance @uiPlayerColorPainterInstance = node.GetChild ("utilHandlers").
                            GetChild ("UiPlayerColorPainter").GetComponent ("ScriptInstance");
                uiPlayerColorPainterInstance.SetAttribute ("playerColor_", Variant (playerInfo.color));
            }
        }
    }

    void Stop ()
    {

    }

    void HandleGameStateChanged (StringHash eventType, VariantMap &eventData)
    {
        int newGameState = eventData ["newGameState"].GetInt ();
        if (newGameState == GAME_STATE_WAITING_FOR_START)
        {
            AddWaitingForStartStateUiHandlers ();
        }
        else if (newGameState == GAME_STATE_PLAYING)
        {
            AddPlayingStateUiHandlers ();
        }
        else if (newGameState == GAME_STATE_FINISHED)
        {
            // Nothing now.
        }
    }
};
