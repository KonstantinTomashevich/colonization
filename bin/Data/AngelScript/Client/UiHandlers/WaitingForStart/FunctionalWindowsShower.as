#include "AngelScript/Utils/Constants.as"

class FunctionalWindowsShower : ScriptObject
{
    FunctionalWindowsShower ()
    {

    }

    ~FunctionalWindowsShower ()
    {

    }

    void Start ()
    {
        Node @uiHandlersNode = node.parent;
        ScriptInstance @chatBlockedPlayersListEditorInstance = uiHandlersNode.CreateChild ("ChatBlockedPlayersListEditor", LOCAL).CreateComponent ("ScriptInstance");
        chatBlockedPlayersListEditorInstance.CreateObject (cache.GetResource ("ScriptFile",
                                                      "AngelScript/Client/UiHandlers/WaitingForStart/ChatBlockedPlayersListEditor.as"),
                                         "ChatBlockedPlayersListEditor");


        ScriptInstance @chatPrivateReceiversListEditorInstance = uiHandlersNode.CreateChild ("ChatPrivateReceiversListEditor", LOCAL).CreateComponent ("ScriptInstance");
        chatPrivateReceiversListEditorInstance.CreateObject (cache.GetResource ("ScriptFile",
                                                      "AngelScript/Client/UiHandlers/WaitingForStart/ChatPrivateReceiversListEditor.as"),
                                         "ChatPrivateReceiversListEditor");

        ScriptInstance @mapsListInstance = uiHandlersNode.CreateChild ("MapsList", LOCAL).CreateComponent ("ScriptInstance");
        mapsListInstance.CreateObject (cache.GetResource ("ScriptFile",
                                                      "AngelScript/Client/UiHandlers/WaitingForStart/MapsList.as"),
                                         "MapsList");

        ScriptInstance @mapInfoWindowInstance = uiHandlersNode.CreateChild ("MapInfoWindow", LOCAL).CreateComponent ("ScriptInstance");
        mapInfoWindowInstance.CreateObject (cache.GetResource ("ScriptFile",
                                                      "AngelScript/Client/UiHandlers/WaitingForStart/MapInfoWindow.as"),
                                         "MapInfoWindow");

        ScriptInstance @actionsWindowInstance = uiHandlersNode.CreateChild ("ActionsWindow", LOCAL).CreateComponent ("ScriptInstance");
        actionsWindowInstance.CreateObject (cache.GetResource ("ScriptFile",
                                                      "AngelScript/Client/UiHandlers/WaitingForStart/ActionsWindow.as"),
                                         "ActionsWindow");

        ScriptInstance @selectPlayerColorWindowInstance = uiHandlersNode.CreateChild ("SelectPlayerColorWindow", LOCAL).CreateComponent ("ScriptInstance");
        selectPlayerColorWindowInstance.CreateObject (cache.GetResource ("ScriptFile",
                                                      "AngelScript/Client/UiHandlers/WaitingForStart/SelectPlayerColorWindow.as"),
                                         "SelectPlayerColorWindow");

        UIElement @functionalWindowsRoot = ui.root.GetChild ("waitingForStart").GetChild ("functionalWindows");
        Array <UIElement @> elements = functionalWindowsRoot.GetChildren ();
        for (uint index = 0; index < elements.length; index++)
        {
            elements [index].visible = false;
        }

        SubscribeToEvent (EVENT_SHOW_FUNCTIONAL_WINDOW_REQUEST, "HandleShowFunctionalWindowRequest");
        VariantMap eventData;
        eventData [ShowFunctionalWindowRequest::WINDOW_NAME] = Variant ("actionsWindow");
        SendEvent (EVENT_SHOW_FUNCTIONAL_WINDOW_REQUEST, eventData);
    }

    void Update (float timeStep)
    {

    }

    void Stop ()
    {

    }

    void HandleShowFunctionalWindowRequest (StringHash eventType, VariantMap &eventData)
    {
        UIElement @functionalWindowsRoot = ui.root.GetChild ("waitingForStart").GetChild ("functionalWindows");
        Array <UIElement @> elements = functionalWindowsRoot.GetChildren ();
        for (uint index = 0; index < elements.length; index++)
        {
            elements [index].visible = false;
        }
        functionalWindowsRoot.GetChild (eventData [ShowFunctionalWindowRequest::WINDOW_NAME].GetString ()).visible = true;
    }
}
