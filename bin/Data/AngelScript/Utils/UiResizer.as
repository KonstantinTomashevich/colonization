class UiResizer : ScriptObject
{
    protected IntVector2 lastScreenSize_;
    String startElementName_;
    bool continuousResize_;

    protected void ResizeUi ()
    {
        UIElement @uiRoot = ui.root;
        if (startElementName_ != "UIRoot")
        {
            uiRoot = ui.root.GetChild (startElementName_, true);
        }

        if (uiRoot !is null)
        {
            if (uiRoot !is ui.root)
            {
                ProcessElement (uiRoot);
            }
            ProcessChildren (uiRoot);
        }
    }

    protected void ProcessChildren (UIElement @element)
    {
        Array <UIElement @> children = element.GetChildrenWithTag ("EnableUiResizer");
        for (uint index = 0; index < children.length; index++)
        {
            UIElement @childElement = children [index];
            if (not childElement.internal)
            {
                ProcessElement (childElement);
                ProcessChildren (childElement);
            }
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

        int xPos = int (dependenciesMap [xDependency].GetFloat () * xModifer);
        int yPos = int (dependenciesMap [yDependency].GetFloat () * yModifer);

        int width = int (dependenciesMap [widthDependency].GetFloat () * widthModifer);
        int height = int (dependenciesMap [heightDependency].GetFloat () * heightModifer);

        if (element.vars ["InvertXPosition"].GetBool ())
        {
            StringHash invDependency = StringHash (element.vars ["InvertXDependency"].GetString ());
            float invertedPositionModifer = element.vars ["InvertXModifer"].GetFloat ();
            xPos = int (dependenciesMap [invDependency].GetFloat () * invertedPositionModifer - xPos);
        }

        if (element.vars ["InvertYPosition"].GetBool ())
        {
            StringHash invDependency = StringHash (element.vars ["InvertYDependency"].GetString ());
            float invertedPositionModifer = element.vars ["InvertYModifer"].GetFloat ();
            yPos = int (dependenciesMap [invDependency].GetFloat () * invertedPositionModifer - yPos);
        }

        if (element.vars ["InvertWidth"].GetBool ())
        {
            StringHash invDependency = StringHash (element.vars ["InvertWidthDependency"].GetString ());
            float invertedPositionModifer = element.vars ["InvertWidthModifer"].GetFloat ();
            width = int (dependenciesMap [invDependency].GetFloat () * invertedPositionModifer - width);
        }

        if (element.vars ["InvertHeight"].GetBool ())
        {
            StringHash invDependency = StringHash (element.vars ["InvertHeightDependency"].GetString ());
            float invertedPositionModifer = element.vars ["InvertHeightModifer"].GetFloat ();
            height = int (dependenciesMap [invDependency].GetFloat () * invertedPositionModifer - height);
        }

        if (xDependency != StringHash ("disable") and yDependency != StringHash ("disable"))
        {
            element.SetPosition (xPos, yPos);
        }

        if (widthDependency != StringHash ("disable") and heightDependency != StringHash ("disable"))
        {
            element.SetSize (width, height);
        }

        if (element.typeName == "Text" || element.typeName == "LineEdit")
        {
            StringHash fontSizeDependency = StringHash (element.vars ["FontSizeDependency"].GetString ());
            float fontSizeModifer = element.vars ["FontSizeModifer"].GetFloat ();

            if (element.typeName == "Text")
            {
                Text @text = element;
                text.fontSize = int (dependenciesMap [fontSizeDependency].GetFloat () * fontSizeModifer);
            }
            else if (element.typeName == "LineEdit")
            {
                LineEdit @lineEdit = element;
                lineEdit.textElement.fontSize = int (dependenciesMap [fontSizeDependency].GetFloat () * fontSizeModifer);
            }
        }
    }

    UiResizer ()
    {
        startElementName_ = "UIRoot";
        continuousResize_ = false;
    }

    ~UiResizer ()
    {

    }

    void Start ()
    {
        lastScreenSize_ = IntVector2 (0, 0);
    }

    void PostUpdate (float timeStep)
    {
        if (lastScreenSize_.x != graphics.width or lastScreenSize_.y != graphics.height or continuousResize_)
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
