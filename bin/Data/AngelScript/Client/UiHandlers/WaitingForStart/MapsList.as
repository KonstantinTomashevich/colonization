#include "AngelScript/Utils/ClientUtils.as"
#include "AngelScript/Utils/StringListEditorUiHandler.as"

class MapsList : StringListEditorUiHandler
{
    protected Window @GetWindow () override
    {
        return ui.root.GetChild ("waitingForStart").GetChild ("functionalWindows").GetChild ("mapsListWindow");
    }

    protected Array <String> GetElements () override
    {
        return GetScriptMain (node).vars ["mapsList"].GetStringVector ();
    }

    protected void SetElements (Array <String> elements) override
    {
        GetScriptMain (node).vars ["mapsList"] = Variant (elements);
    }

    protected String ProcessElementText (String text, int elementIndex) override
    {
        return ("" + (elementIndex + 1)  + ". " + text);
    }

    protected bool IsElementToAddCorrect (String element) override
    {
        return false;
    }

    protected void Close ()
    {
        VariantMap eventData;
        eventData ["windowName"] = Variant ("actionsWindow");
        SendEvent ("ShowFunctionalWindowRequest", eventData);
    }

    void Update (float timeStep) override
    {
        StringListEditorUiHandler::Update (timeStep);

        Node @scriptMain = GetScriptMain (node);
        Window @window = GetWindow ();
        if (scriptMain !is null)
        {
            if (window.visible and not scriptMain.vars ["isAdmin"].GetBool ())
            {
                Close ();
            }
        }
        else
        {
            if (window.visible)
            {
                Close ();
            }
        }
    }

    // Remove buttons replaced by open buttons.
    void HandleRemoveElementClick (StringHash eventType, VariantMap &eventData) override
    {
        UIElement @element = eventData ["Element"].GetPtr ();
        int elementOffset = element.vars ["ElementOffset"].GetInt ();
        int summaryOffset = elementsShowOffset_ + elementOffset;

        // TODO: Implement request about sending to host.
    }

    void HandleHideClick () override
    {
        Close ();
    }
}
