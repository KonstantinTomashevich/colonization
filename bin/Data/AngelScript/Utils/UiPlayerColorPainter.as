class UiPlayerColorPainter : ScriptObject
{
    Color playerColor_;

    UiPlayerColorPainter ()
    {
        playerColor_ = Color (0.5f, 0.5f, 0.5f, 1.0f);
    }

    ~UiPlayerColorPainter ()
    {

    }

    void Start ()
    {

    }

    void Update (float timeStep)
    {
        Array <UIElement @> elements = ui.root.GetChildrenWithTag ("SetPlayerColor", true);
        for (int index = 0; index < elements.length; index++)
        {
            elements [index].color = playerColor_;
        }
    }

    void Stop ()
    {

    }
}
