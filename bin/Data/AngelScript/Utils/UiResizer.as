class UiResizer : ScriptObject
{
    protected IntVector2 lastScreenSize_;
    String startElementName_;

    protected void ResizeUi ()
    {
        UIElement @uiRoot = ui.root;
        if (startElementName_ != "UIRoot")
            uiRoot = ui.root.GetChild (startElementName_, true);

        if (uiRoot !is null)
        {
            if (uiRoot !is ui.root)
                ProcessElement (uiRoot);
            ProcessChildren (uiRoot);
        }
    }

    protected void ProcessChildren (UIElement @element)
    {
        Array <UIElement @> children = element.GetChildrenWithTag ("EnableUiResizer");
        for (int index = 0; index < children.length; index++)
        {
            UIElement @childElement = children [index];
            ProcessElement (childElement);
            ProcessChildren (childElement);
        }
    }

    protected void ProcessElement (UIElement @element)
    {
        VariantMap dependenciesMap;
        dependenciesMap ["ScreenWidth"] = Variant (graphics.width * 1.0f);
        dependenciesMap ["ScreenHeight"] = Variant (graphics.height * 1.0f);
        dependenciesMap ["ParentWidth"] = Variant (element.parent.width * 1.0f);
        dependenciesMap ["ParentHeight"] = Variant (element.parent.height * 1.0f);

        StringHash widthDependency = StringHash (element.vars ["WidthDependency"].GetString ());
        StringHash heightDependency = StringHash (element.vars ["HeightDependency"].GetString ());

        StringHash xDependency = StringHash (element.vars ["XDependency"].GetString ());
        StringHash yDependency = StringHash (element.vars ["YDependency"].GetString ());

        float widthModifer = element.vars ["WidthModifer"].GetFloat ();
        float heightModifer = element.vars ["HeightModifer"].GetFloat ();

        float xModifer = element.vars ["XModifer"].GetFloat ();
        float yModifer = element.vars ["YModifer"].GetFloat ();

        float xPos = dependenciesMap [xDependency].GetFloat () * xModifer;
        float yPos = dependenciesMap [yDependency].GetFloat () * yModifer;

        if (element.vars ["InvertXPosition"].GetBool ())
        {
            StringHash invDependency = StringHash (element.vars ["InvertXDependency"].GetString ());
            float invertedPositionModifer = element.vars ["InvertXModifer"].GetFloat ();
            xPos = (dependenciesMap [invDependency].GetFloat () * invertedPositionModifer - xPos);
        }

        if (element.vars ["InvertYPosition"].GetBool ())
        {
            StringHash invDependency = StringHash (element.vars ["InvertYDependency"].GetString ());
            float invertedPositionModifer = element.vars ["InvertYModifer"].GetFloat ();
            yPos = (dependenciesMap [invDependency].GetFloat () * invertedPositionModifer - yPos);
        }

        if (xDependency != StringHash ("disable") and yDependency != StringHash ("disable"))
        {
            element.SetPosition (xPos, yPos);
        }

        if (widthDependency != StringHash ("disable") and heightDependency != StringHash ("disable"))
        {
            element.SetSize (dependenciesMap [widthDependency].GetFloat () * widthModifer,
                             dependenciesMap [heightDependency].GetFloat () * heightModifer);
        }

        if (element.typeName == "Text")
        {
            StringHash fontSizeDependency = StringHash (element.vars ["FontSizeDependency"].GetString ());
            float fontSizeModifer = element.vars ["FontSizeModifer"].GetFloat ();

            Text @text = element;
            text.fontSize = dependenciesMap [fontSizeDependency].GetFloat () * fontSizeModifer;
        }
    }

    UiResizer ()
    {

    }

    ~UiResizer ()
    {

    }

    void Start ()
    {
        lastScreenSize_ = IntVector2 (0, 0);
    }

    void Update (float timeStep)
    {
        if (lastScreenSize_.x != graphics.width or lastScreenSize_.y != graphics.height)
        {
            ResizeUi ();
            lastScreenSize_.x = graphics.width;
            lastScreenSize_.y = graphics.height;
        }
    }

    void Stop ()
    {

    }
}
