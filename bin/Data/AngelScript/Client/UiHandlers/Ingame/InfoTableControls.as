class InfoTableControls : ScriptObject
{
    InfoTableControls ()
    {

    }

    ~InfoTableControls ()
    {

    }

    void Start ()
    {
        Window @infoTableWindow = ui.root.GetChild ("ingame").GetChild ("infoTableWindow");
        Button @hideButton = infoTableWindow.GetChild ("hideButton");
        SubscribeToEvent (hideButton, "Released", "HandleHideClick");

        ScriptInstance @infoTablePlayersListInstance = node.CreateChild ("InfoTablePlayersList", LOCAL).CreateComponent ("ScriptInstance");
        infoTablePlayersListInstance.CreateObject (cache.GetResource ("ScriptFile",
                                                      "AngelScript/Client/UiHandlers/Ingame/InfoTableWindows/InfoTablePlayersList.as"),
                                         "InfoTablePlayersList");
    }

    void Update (float timeStep)
    {
        Window @infoTableWindow = ui.root.GetChild ("ingame").GetChild ("infoTableWindow");
        String selectedInfoType = infoTableWindow.vars ["selectedInfoType"].GetString ();

        Array <UIElement @> elements = infoTableWindow.GetChildren ();
        for (uint index = 0; index < elements.length; index++)
        {
            UIElement @element = elements [index];
            if (element.vars ["infoWindow"].GetBool ())
            {
                element.visible = false;
            }
        }
        infoTableWindow.GetChild (selectedInfoType).visible = true;
    }

    void Stop ()
    {

    }

    void HandleHideClick ()
    {
        Window @infoTableWindow = ui.root.GetChild ("ingame").GetChild ("infoTableWindow");
        infoTableWindow.visible = false;
    }
}
