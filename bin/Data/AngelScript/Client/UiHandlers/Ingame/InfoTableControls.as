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
        Button @playersListButton = infoTableWindow.GetChild ("playersListButton");
        Button @warsListButton = infoTableWindow.GetChild ("warsListButton");

        SubscribeToEvent (hideButton, "Released", "HandleHideClick");
        SubscribeToEvent (playersListButton, "Released", "HandlePlayersListClick");
        SubscribeToEvent (warsListButton, "Released", "HandleWarsListClick");

        ScriptInstance @infoTablePlayersListInstance = node.CreateChild ("InfoTablePlayersList", LOCAL).CreateComponent ("ScriptInstance");
        infoTablePlayersListInstance.CreateObject (cache.GetResource ("ScriptFile",
                                                      "AngelScript/Client/UiHandlers/Ingame/InfoTableWindows/InfoTablePlayersList.as"),
                                         "InfoTablePlayersList");

        ScriptInstance @infoTableWarsListInstance = node.CreateChild ("InfoTableWarsList", LOCAL).CreateComponent ("ScriptInstance");
        infoTableWarsListInstance.CreateObject (cache.GetResource ("ScriptFile",
                                                      "AngelScript/Client/UiHandlers/Ingame/InfoTableWindows/InfoTableWarsList.as"),
                                         "InfoTableWarsList");

        ScriptInstance @infoTableShowWarInfoInstance = node.CreateChild ("InfoTableShowWarInfo", LOCAL).CreateComponent ("ScriptInstance");
        infoTableShowWarInfoInstance.CreateObject (cache.GetResource ("ScriptFile",
                                                      "AngelScript/Client/UiHandlers/Ingame/InfoTableWindows/InfoTableShowWarInfo.as"),
                                         "InfoTableShowWarInfo");
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

    void HandlePlayersListClick ()
    {
        Window @infoTableWindow = ui.root.GetChild ("ingame").GetChild ("infoTableWindow");
        infoTableWindow.vars ["selectedInfoType"] = Variant ("playersList");
    }

    void HandleWarsListClick ()
    {
        Window @infoTableWindow = ui.root.GetChild ("ingame").GetChild ("infoTableWindow");
        infoTableWindow.vars ["selectedInfoType"] = Variant ("warsList");
    }
}
