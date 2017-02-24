class JoinGameMenu : ScriptObject
{
    JoinGameMenu ()
    {

    }

    ~JoinGameMenu ()
    {

    }

    void Start ()
    {
        Window @joinGameMenu = ui.root.GetChild ("mainMenu").GetChild ("joinGameMenu");
        Button @joinGameButton = joinGameMenu.GetChild ("joinGameButton");
        Button @backButton = joinGameMenu.GetChild ("goBackButton");

        SubscribeToEvent (joinGameButton, "Released", "HandleJoinGameClick");
        SubscribeToEvent (backButton, "Released", "HandleGoBackClick");
    }

    void Update (float timeStep)
    {
        Window @joinGameMenu = ui.root.GetChild ("mainMenu").GetChild ("joinGameMenu");
        Slider @redSlider = joinGameMenu.GetChild ("redSlider");
        Slider @greenSlider = joinGameMenu.GetChild ("greenSlider");
        Slider @blueSlider = joinGameMenu.GetChild ("blueSlider");

        float r = redSlider.value / redSlider.range;
        float g = greenSlider.value / greenSlider.range;
        float b = blueSlider.value / blueSlider.range;

        BorderImage @colorSample = joinGameMenu.GetChild ("colorSample");
        colorSample.color = Color (r, g, b, 1.0f);

        Button @joinGameButton = joinGameMenu.GetChild ("joinGameButton");
        LineEdit @nicknameEdit = joinGameMenu.GetChild ("nicknameEdit");
        LineEdit @serverAdressEdit = joinGameMenu.GetChild ("serverAdressEdit");
        LineEdit @serverPortEdit = joinGameMenu.GetChild ("serverPortEdit");
        joinGameButton.visible = (nicknameEdit.text != "" and serverAdressEdit.text != "" and serverPortEdit.text != "");
    }

    void Stop ()
    {

    }

    void HandleJoinGameClick ()
    {
        Window @joinGameMenu = ui.root.GetChild ("mainMenu").GetChild ("joinGameMenu");
        LineEdit @nicknameEdit = joinGameMenu.GetChild ("nicknameEdit");
        LineEdit @serverAdressEdit = joinGameMenu.GetChild ("serverAdressEdit");
        LineEdit @serverPortEdit = joinGameMenu.GetChild ("serverPortEdit");
        Slider @redSlider = joinGameMenu.GetChild ("redSlider");
        Slider @greenSlider = joinGameMenu.GetChild ("greenSlider");
        Slider @blueSlider = joinGameMenu.GetChild ("blueSlider");

        String nickname = nicknameEdit.text;
        String adress = serverAdressEdit.text;
        int port = serverPortEdit.text.ToInt ();
        float r = redSlider.value / redSlider.range;
        float g = greenSlider.value / greenSlider.range;
        float b = blueSlider.value / blueSlider.range;

        VariantMap eventData;
        eventData ["nickname"] = Variant (nickname);
        eventData ["color"] = Variant (Color (r, g, b, 1.0f));
        eventData ["adress"] = Variant (adress);
        eventData ["port"] = Variant (port);
        SendEvent ("JoinGameRequest", eventData);
    }

    void HandleGoBackClick ()
    {
        VariantMap eventData;
        eventData ["windowName"] = Variant ("primaryMenu");
        SendEvent ("ShowWindowRequest", eventData);
    }

    void HandleSelectMapClick ()
    {
        ui.root.GetChild ("mainMenu").GetChild ("mapsList").visible = true;
    }
}
