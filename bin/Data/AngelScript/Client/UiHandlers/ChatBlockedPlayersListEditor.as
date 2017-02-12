class ChatBlockedPlayersListEditor : ScriptObject
{
    protected int elementsShowOffset_;
    protected float untilElementsScrollUpdate_;
    protected int ELEMENTS_SCROLL_SPEED = 5;

    protected Window @GetWindow ()
    {
        return ui.root.GetChild ("ingame").GetChild ("chatBlockedPlayersWindow");
    }

    protected Array <String> GetElements ()
    {
        return node.parent.vars ["chatBlockedPlayersList"].GetStringVector ();
    }

    protected void SetElements (Array <String> elements)
    {
        node.parent.vars ["chatBlockedPlayersList"] = Variant (elements);
    }

    protected String ProcessElementText (String text)
    {
        return text;
    }

    protected void UpdateElementsScroll ()
    {
        Window @chatBlockedPlayersWindow = GetWindow ();
        Button @upButton = chatBlockedPlayersWindow.GetChild ("upButton");
        Button @downButton = chatBlockedPlayersWindow.GetChild ("downButton");

        if (upButton.pressed and elementsShowOffset_ > 0)
        {
            elementsShowOffset_ -= 1;
        }
        else if (downButton.pressed)
        {
            elementsShowOffset_ += 1;
        }
    }

    protected void CheckElementsShowOffset (Array <UIElement @> &elementsUi, int elementsStringsLength)
    {
        if (elementsShowOffset_ > elementsStringsLength - 1.0f * elementsUi.length)
        {
            if (elementsStringsLength - 1.0f * elementsUi.length < 0)
            {
                elementsShowOffset_ = 0;
                for (int index = 0; index < elementsUi.length; index++)
                {
                    elementsUi [index].visible = (elementsStringsLength - index > 0);
                }
            }
            else
            {
                elementsShowOffset_ = elementsStringsLength - 1.0f * elementsUi.length;
            }
        }
    }

    protected void UpdateElementsInfo (int elementsUiLength, int elementsStringsLength)
    {
        Window @chatBlockedPlayersWindow = GetWindow ();
        Text @elementsInfo = chatBlockedPlayersWindow.GetChild ("elementsInfo");
        elementsInfo.text = "Showing elements from " + (elementsShowOffset_ + 1) + " to " + (elementsShowOffset_ + elementsUiLength + 1)
                            + ", summary count -- " + elementsStringsLength + ".";
    }

    protected void UpdateElements ()
    {
        Window @chatBlockedPlayersWindow = GetWindow ();
        Array <UIElement @> elementsUi = chatBlockedPlayersWindow.GetChild ("elements").GetChildren ();
        Array <String> elementsStrings = GetElements ();
        CheckElementsShowOffset (elementsUi, elementsStrings.length);
        UpdateElementsInfo (elementsUi.length, elementsStrings.length);

        for (int index = 0; index < elementsUi.length; index++)
        {
            int elementIndex = elementsShowOffset_ + index;
            if (elementIndex < elementsStrings.length)
            {
                Text @elementText = elementsUi [index].GetChild ("elementName");
                elementText.text = ProcessElementText (elementsStrings [elementIndex]);
            }
        }
    }

    ChatBlockedPlayersListEditor ()
    {
        elementsShowOffset_ = 0;
        untilElementsScrollUpdate_ = 1.0f / (ELEMENTS_SCROLL_SPEED * 1.0f);
    }

    ~ChatBlockedPlayersListEditor ()
    {

    }

    void Start ()
    {
        Window @chatBlockedPlayersWindow = GetWindow ();
        chatBlockedPlayersWindow.visible = false;
        Button @addButton = chatBlockedPlayersWindow.GetChild ("addButton");
        Button @hideButton = chatBlockedPlayersWindow.GetChild ("hideButton");

        SubscribeToEvent (addButton, "Released", "HandleAddElementClick");
        SubscribeToEvent (hideButton, "Released", "HandleHideClick");

        Array <UIElement @> elementsUi = chatBlockedPlayersWindow.GetChild ("elements").GetChildren ();
        for (int index = 0; index < elementsUi.length; index++)
        {
            Button @removeButton = elementsUi [index].GetChild ("removeButton");
            SubscribeToEvent (removeButton, "Released", "HandleRemoveElementClick");
        }
    }

    void Update (float timeStep)
    {
        if (GetWindow ().visible)
        {
            untilElementsScrollUpdate_ -= timeStep;
            if (untilElementsScrollUpdate_ <= 0.0f)
            {
                UpdateElementsScroll ();
                untilElementsScrollUpdate_ = 1.0f / (ELEMENTS_SCROLL_SPEED * 1.0f);
            }
            UpdateElements ();
        }
    }

    void Stop ()
    {
        UnsubscribeFromAllEvents ();
    }

    void HandleAddElementClick ()
    {
        Window @chatBlockedPlayersWindow = GetWindow ();
        LineEdit @elementToAddEdit = chatBlockedPlayersWindow.GetChild ("elementToAddEdit");
        String element = elementToAddEdit.text;
        elementToAddEdit.text = "";
        elementsShowOffset_ += 1;

        Array <String> elements = GetElements ();
        elements.Push (element);
        SetElements (elements);
    }

    void HandleRemoveElementClick (StringHash eventType, VariantMap &eventData)
    {
        UIElement @element = eventData ["Element"].GetPtr ();
        int elementOffset = element.vars ["ElementOffset"].GetInt ();
        int summaryOffset = elementsShowOffset_ + elementOffset;

        Array <String> elements = GetElements ();
        elements.Erase (summaryOffset);
        SetElements (elements);
    }

    void HandleHideClick ()
    {
        GetWindow ().visible = false;
    }
}
