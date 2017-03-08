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

        UIElement @functionalWindowsRoot = ui.root.GetChild ("waitingForStart").GetChild ("functionalWindows");
        Array <UIElement @> elements = functionalWindowsRoot.GetChildren ();
        for (int index = 0; index < elements.length; index++)
        {
            elements [index].visible = false;
        }

        SubscribeToEvent ("ShowFunctionalWindowRequest", "HandleShowFunctionalWindowRequest");
        VariantMap eventData;
        eventData ["windowName"] = Variant ("actionsWindow");
        SendEvent ("ShowFunctionalWindowRequest", eventData);
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
        for (int index = 0; index < elements.length; index++)
        {
            elements [index].visible = false;
        }
        functionalWindowsRoot.GetChild (eventData ["windowName"].GetString ()).visible = true;
    }
}
