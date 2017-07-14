#include "AngelScript/Utils/ClientUtils.as"
#include "AngelScript/Utils/ScriptObjectWithBeforeStop.as"

abstract class StringListEditorUiHandler : ScriptObjectWithBeforeStop
{
    protected uint elementsShowOffset_;
    protected float untilElementsScrollUpdate_;
    protected uint ELEMENTS_SCROLL_SPEED = 5;

    protected Window @GetWindow ()
    {
        //! Will be implemented in inheritors!
        return null;
    }

    protected Array <String> GetElements ()
    {
        //! Will be implemented in inheritors!
        return Array <String> ();
    }

    protected void SetElements (Array <String> elements)
    {
        //! Will be implemented in inheritors!
    }

    protected String ProcessElementText (String text, int elementIndex)
    {
        //! Will be implemented in inheritors!
        return text;
    }

    protected bool IsElementToAddCorrect (String element)
    {
        //! Will be implemented in inheritors!
        return true;
    }

    protected void ProcessElementUi (UIElement @uiElement, int elementIndex, Array <String> &in elementsStrings)
    {
        //! Can be overwriten by inheritors.
        Text @elementText = uiElement.GetChild ("elementName");
        elementText.text = ProcessElementText (elementsStrings [elementIndex], elementIndex);
    }

    protected void UpdateElementsScroll ()
    {
        Window @window = GetWindow ();
        Button @upButton = window.GetChild ("upButton");
        Button @downButton = window.GetChild ("downButton");

        if (upButton.pressed and elementsShowOffset_ > 0)
        {
            elementsShowOffset_ -= 1;
        }
        else if (downButton.pressed)
        {
            elementsShowOffset_ += 1;
        }
    }

    protected void CheckElementsShowOffset (Array <UIElement @> &elementsUi, uint elementsStringsLength)
    {
        if (FloorToInt (elementsShowOffset_) >FloorToInt (FloorToInt (elementsStringsLength) - elementsUi.length))
        {
            if (FloorToInt (elementsStringsLength) - elementsUi.length < 0)
            {
                elementsShowOffset_ = 0;
            }
            else
            {
                elementsShowOffset_ = uint (FloorToInt (elementsStringsLength) - elementsUi.length);
            }
        }
    }

    protected void UpdateElementsInfo (int elementsUiLength, int elementsStringsLength)
    {
        Window @window = GetWindow ();
        Text @elementsInfo = window.GetChild ("elementsInfo");
        elementsInfo.text = "Showing elements from " + (elementsShowOffset_ + 1) + " to " + (elementsShowOffset_ + elementsUiLength)
                            + ", summary count -- " + elementsStringsLength + ".";
    }

    protected void UpdateElements ()
    {
        Window @window = GetWindow ();
        Array <UIElement @> elementsUi = window.GetChild ("elements").GetChildren ();
        Array <String> elementsStrings = GetElements ();
        CheckElementsShowOffset (elementsUi, elementsStrings.length);
        UpdateElementsInfo (elementsUi.length, elementsStrings.length);

        for (uint index = 0; index < elementsUi.length; index++)
        {
            uint elementIndex = elementsShowOffset_ + index;
            if (elementIndex < elementsStrings.length)
            {
                UIElement @uiElement = elementsUi [index];
                uiElement.visible = true;
                ProcessElementUi (uiElement, elementIndex, elementsStrings);
            }
            else
            {
                UIElement @uiElement = elementsUi [index];
                uiElement.visible = false;
            }
        }
    }

    StringListEditorUiHandler ()
    {
        elementsShowOffset_ = 0;
        untilElementsScrollUpdate_ = 1.0f / (ELEMENTS_SCROLL_SPEED * 1.0f);
    }

    ~StringListEditorUiHandler ()
    {

    }

    void Start () override
    {
        ScriptObjectWithBeforeStop::Start ();

        Window @window = GetWindow ();
        Button @addButton = window.GetChild ("addButton");
        Button @hideButton = window.GetChild ("hideButton");

        SubscribeToEvent (addButton, "Released", "HandleAddElementClick");
        SubscribeToEvent (hideButton, "Released", "HandleHideClick");

        Array <UIElement @> elementsUi = window.GetChild ("elements").GetChildren ();
        for (uint index = 0; index < elementsUi.length; index++)
        {
            Button @removeButton = elementsUi [index].GetChild ("removeButton");
            SubscribeToEvent (removeButton, "Released", "HandleRemoveElementClick");
        }

        Node @scriptMain = GetScriptMain (node);
        LineEdit @elementToAddEdit = window.GetChild ("elementToAddEdit");
        RegisterLineEdit (scriptMain, elementToAddEdit);
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

    void BeforeStop (Scene @lastScene, Node @lastNode) override
    {
        Node @scriptMain = GetScriptMain (lastScene);
        Window @window = GetWindow ();
        if (scriptMain !is null and window !is null)
        {
            LineEdit @elementToAddEdit = window.GetChild ("elementToAddEdit");
            UnregisterLineEdit (scriptMain, elementToAddEdit);
        }
    }

    void HandleAddElementClick ()
    {
        Window @window = GetWindow ();
        LineEdit @elementToAddEdit = window.GetChild ("elementToAddEdit");
        String element = elementToAddEdit.text;
        elementToAddEdit.text = "";
        elementsShowOffset_ += 1;

        if (IsElementToAddCorrect (element))
        {
            Array <String> elements = GetElements ();
            elements.Push (element);
            SetElements (elements);
        }
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
