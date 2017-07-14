#include "AngelScript/Utils/Constants.as"

class SelectPlayerColorWindow : ScriptObject
{
    protected void Close ()
    {
        VariantMap eventData;
        eventData [ShowFunctionalWindowRequest::WINDOW_NAME] = Variant ("actionsWindow");
        SendEvent (EVENT_SHOW_FUNCTIONAL_WINDOW_REQUEST, eventData);
    }

    SelectPlayerColorWindow ()
    {

    }

    ~SelectPlayerColorWindow ()
    {

    }

    void Start ()
    {
        Window @selectPlayerColorWindow = ui.root.GetChild ("waitingForStart").GetChild ("functionalWindows").
                                          GetChild ("selectPlayerColorWindow");
        Button @hideButton = selectPlayerColorWindow.GetChild ("hideButton");
        Button @selectColorButton = selectPlayerColorWindow.GetChild ("selectColorButton");

        SubscribeToEvent (hideButton, "Released", "HandleHideClick");
        SubscribeToEvent (selectColorButton, "Released", "HandleSelectPlayerColorClick");
    }

    void Update (float timeStep)
    {
        Window @selectPlayerColorWindow = ui.root.GetChild ("waitingForStart").GetChild ("functionalWindows").
                                          GetChild ("selectPlayerColorWindow");
        Slider @redSlider = selectPlayerColorWindow.GetChild ("redSlider");
        Slider @greenSlider = selectPlayerColorWindow.GetChild ("greenSlider");
        Slider @blueSlider = selectPlayerColorWindow.GetChild ("blueSlider");

        float r = redSlider.value / redSlider.range;
        float g = greenSlider.value / greenSlider.range;
        float b = blueSlider.value / blueSlider.range;

        BorderImage @colorSample = selectPlayerColorWindow.GetChild ("colorSample");
        colorSample.color = Color (r, g, b, 1.0f);
    }

    void Stop ()
    {

    }

    void HandleHideClick ()
    {
        Close ();
    }

    void HandleSelectPlayerColorClick ()
    {
        Window @selectPlayerColorWindow = ui.root.GetChild ("waitingForStart").GetChild ("functionalWindows").
                                          GetChild ("selectPlayerColorWindow");
        Slider @redSlider = selectPlayerColorWindow.GetChild ("redSlider");
        Slider @greenSlider = selectPlayerColorWindow.GetChild ("greenSlider");
        Slider @blueSlider = selectPlayerColorWindow.GetChild ("blueSlider");

        float r = redSlider.value / redSlider.range;
        float g = greenSlider.value / greenSlider.range;
        float b = blueSlider.value / blueSlider.range;
        Color color = Color (r, g, b, 1.0f);

        VectorBuffer buffer;
        buffer.WriteColor (color);

        VariantMap eventData;
        eventData [NewNetworkTask::TASK_TYPE] = Variant (CTS_NETWORK_MESSAGE_RESELECT_PLAYER_COLOR);
        eventData [NewNetworkTask::MESSAGE_BUFFER] = Variant (buffer);
        SendEvent (EVENT_NEW_NETWORK_TASK, eventData);
        Close ();
    }
}
