#include "AngelScript/Utils/ClientUtils.as"

// TODO: Set some ui elements color to player color.
class ClientUi : ScriptObject
{
    protected XMLFile @style_;
    protected bool isSceneLoaded_;

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

        ui.root.CreateChild ("UIElement", "billboardsRoot");
        UIElement @uiRoot = ui.LoadLayout (cache.GetResource ("XMLFile", "UI/Ingame.xml"));
        ui.root.AddChild (uiRoot);
        uiRoot.defaultStyle = style_;
        uiRoot.name = "ingame";

        ScriptInstance @playerInfoWindowInstance = node.CreateChild ("PlayerInfoWindow", LOCAL).CreateComponent ("ScriptInstance");
        playerInfoWindowInstance.CreateObject (cache.GetResource ("ScriptFile",
                                                         "AngelScript/Client/UiHandlers/PlayerInfoWindow.as"),
                                               "PlayerInfoWindow");

        ScriptInstance @menuWindowInstance = node.CreateChild ("MenuWindow", LOCAL).CreateComponent ("ScriptInstance");
        menuWindowInstance.CreateObject (cache.GetResource ("ScriptFile",
                                                      "AngelScript/Client/UiHandlers/MenuWindow.as"),
                                         "MenuWindow");

        ScriptInstance @mapBillboardsInstance = node.CreateChild ("MapBillboards", LOCAL).CreateComponent ("ScriptInstance");
        mapBillboardsInstance.CreateObject (cache.GetResource ("ScriptFile",
                                                     "AngelScript/Client/UiHandlers/MapBillboards.as"),
                                            "MapBillboards");

        ScriptInstance @chatWindowInstance = node.CreateChild ("ChatWindow", LOCAL).CreateComponent ("ScriptInstance");
        chatWindowInstance.CreateObject (cache.GetResource ("ScriptFile",
                                                      "AngelScript/Client/UiHandlers/ChatWindow.as"),
                                         "ChatWindow");

        ScriptInstance @unitSelectedWindowInstance = node.CreateChild ("UnitSelectedWindow", LOCAL).CreateComponent ("ScriptInstance");
        unitSelectedWindowInstance.CreateObject (cache.GetResource ("ScriptFile",
                                                      "AngelScript/Client/UiHandlers/UnitSelectedWindow.as"),
                                         "UnitSelectedWindow");

        ScriptInstance @districtSelectedWindowInstance = node.CreateChild ("DistrictSelectedWindow", LOCAL).CreateComponent ("ScriptInstance");
        districtSelectedWindowInstance.CreateObject (cache.GetResource ("ScriptFile",
                                                      "AngelScript/Client/UiHandlers/DistrictSelectedWindow.as"),
                                         "DistrictSelectedWindow");

        ScriptInstance @chatBlockedPlayersListEditorInstance = node.CreateChild ("ChatBlockedPlayersListEditor", LOCAL).CreateComponent ("ScriptInstance");
        chatBlockedPlayersListEditorInstance.CreateObject (cache.GetResource ("ScriptFile",
                                                      "AngelScript/Client/UiHandlers/ChatBlockedPlayersListEditor.as"),
                                         "ChatBlockedPlayersListEditor");


        ScriptInstance @chatPrivateReceiversListEditorInstance = node.CreateChild ("ChatPrivateReceiversListEditor", LOCAL).CreateComponent ("ScriptInstance");
        chatPrivateReceiversListEditorInstance.CreateObject (cache.GetResource ("ScriptFile",
                                                      "AngelScript/Client/UiHandlers/ChatPrivateReceiversListEditor.as"),
                                         "ChatPrivateReceiversListEditor");

        ScriptInstance @uiResizerInstance = node.CreateChild ("UiResizer", LOCAL).CreateComponent ("ScriptInstance");
        uiResizerInstance.CreateObject (cache.GetResource ("ScriptFile",
                                                         "AngelScript/Utils/UiResizer.as"),
                                        "UiResizer");
        uiResizerInstance.SetAttribute ("startElementName_", Variant ("UIRoot"));
        uiResizerInstance.SetAttribute ("continuousResize_", Variant (true));
    }

    void Update (float timeStep)
    {
        if (!isSceneLoaded_)
        {
            isSceneLoaded_ = CheckIsSceneLoaded (scene);
        }
    }

    void Stop ()
    {

    }
};
