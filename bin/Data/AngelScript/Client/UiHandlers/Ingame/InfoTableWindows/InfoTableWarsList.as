#include "AngelScript/Utils/ClientUtils.as"
#include "AngelScript/Utils/StringListEditorUiHandler.as"

class InfoTableWarsList : StringListEditorUiHandler
{
    protected Array <StringHash> visibleWarsHashes_;

    InfoTableWarsList ()
    {

    }

    ~InfoTableWarsList ()
    {

    }

    void HandleAddElementClick () override
    {
        // Ignore, elements can't be added by user to this list.
    }

    void HandleRemoveElementClick (StringHash eventType, VariantMap &eventData) override
    {
        UIElement @element = eventData ["Element"].GetPtr ();
        int elementOffset = element.vars ["ElementOffset"].GetInt ();
        int summaryOffset = elementsShowOffset_ + elementOffset;

        Window @warsList = GetWindow ();
        warsList.parent.vars ["selectedInfoType"] = "showWarInfo";
        warsList.parent.vars ["elementToShowHash"] = Variant (visibleWarsHashes_ [summaryOffset]);
    }

    void HandleHideClick () override
    {
        // Ignore, this window can't be closed.
    }

    protected Window @GetWindow () override
    {
        return ui.root.GetChild ("ingame").GetChild ("infoTableWindow").GetChild ("warsList");
    }

    protected Array <String> GetElements () override
    {
        visibleWarsHashes_.Clear ();
        Array <String> elements = GetWarsShortnamesList (scene, visibleWarsHashes_);
        return elements;
    }

    protected void SetElements (Array <String> elements) override
    {
        // Ignore, elements can't be added by user to this list.
    }

    protected String ProcessElementText (String text, int elementIndex) override
    {
        String result = "" + (elementIndex + 1) + ". " + text;
        return result;
    }

    protected bool IsElementToAddCorrect (String element) override
    {
        // Ignore, elements can't be added by user to this list.
        return false;
    }

    protected void ProcessElementUi (UIElement @uiElement, int elementIndex, Array <String> &in elementsStrings) override
    {
        StringListEditorUiHandler::ProcessElementUi (uiElement, elementIndex, elementsStrings);
    }
}
