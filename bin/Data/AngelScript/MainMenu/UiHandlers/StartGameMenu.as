class StartGameMenu : ScriptObject
{
    StartGameMenu ()
    {

    }

    ~StartGameMenu ()
    {

    }

    void Start ()
    {
        Window @startGameMenu = ui.root.GetChild ("mainMenu").GetChild ("startGameMenu");
        Button @startGameButton = startGameMenu.GetChild ("startGameButton");
        Button @backButton = startGameMenu.GetChild ("goBackButton");

        SubscribeToEvent (startGameButton, "Released", "HandleStartGameClick");
        SubscribeToEvent (backButton, "Released", "HandleGoBackClick");
    }

    void Update (float timeStep)
    {
        Window @startGameMenu = ui.root.GetChild ("mainMenu").GetChild ("startGameMenu");
        Slider @redSlider = startGameMenu.GetChild ("redSlider");
        Slider @greenSlider = startGameMenu.GetChild ("greenSlider");
        Slider @blueSlider = startGameMenu.GetChild ("blueSlider");

        float r = redSlider.value / redSlider.range;
        float g = greenSlider.value / greenSlider.range;
        float b = blueSlider.value / blueSlider.range;

        BorderImage @colorSample = startGameMenu.GetChild ("colorSample");
        colorSample.color = Color (r, g, b, 1.0f);

        Button @startGameButton = startGameMenu.GetChild ("startGameButton");
        LineEdit @nicknameEdit = startGameMenu.GetChild ("nicknameEdit");
        startGameButton.visible = (nicknameEdit.text != "");
    }

    void Stop ()
    {

    }

    void HandleStartGameClick ()
    {
        Window @startGameMenu = ui.root.GetChild ("mainMenu").GetChild ("startGameMenu");
        LineEdit @nicknameEdit = startGameMenu.GetChild ("nicknameEdit");
        Slider @redSlider = startGameMenu.GetChild ("redSlider");
        Slider @greenSlider = startGameMenu.GetChild ("greenSlider");
        Slider @blueSlider = startGameMenu.GetChild ("blueSlider");

        String nickname = nicknameEdit.text;
        float r = redSlider.value / redSlider.range;
        float g = greenSlider.value / greenSlider.range;
        float b = blueSlider.value / blueSlider.range;

        node.parent.vars ["nickname"] = Variant (nickname);
        node.parent.vars ["color"] = Variant (Color (r, g, b, 1.0f));
        SendEvent ("StartGameRequest");
    }

    void HandleGoBackClick ()
    {
        ui.root.GetChild ("mainMenu").GetChild ("primaryMenu").visible = true;
        ui.root.GetChild ("mainMenu").GetChild ("startGameMenu").visible = false;
    }
}
