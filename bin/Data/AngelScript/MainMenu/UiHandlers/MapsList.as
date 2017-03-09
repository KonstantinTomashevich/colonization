#include "AngelScript/Utils/StringListEditorUiHandler.as"

class MapsList : StringListEditorUiHandler
{
    protected Window @GetWindow () override
    {
        return ui.root.GetChild ("mainMenu").GetChild ("mapsList");
    }

    protected Array <String> GetElements () override
    {
        return node.parent.vars ["mapsList"].GetStringVector ();
    }

    protected void SetElements (Array <String> elements) override
    {
        node.parent.vars ["mapsList"] = Variant (elements);
    }

    protected String ProcessElementText (String text, int elementIndex) override
    {
        return ("" + (elementIndex + 1)  + ". " + text);
    }

    protected bool IsElementToAddCorrect (String element) override
    {
        return false;
    }

    // Remove buttons replaced by open buttons.
    void HandleRemoveElementClick (StringHash eventType, VariantMap &eventData) override
    {
        UIElement @element = eventData ["Element"].GetPtr ();
        int elementOffset = element.vars ["ElementOffset"].GetInt ();
        int summaryOffset = elementsShowOffset_ + elementOffset;
        node.parent.vars ["selectedMapIndex"] = Variant (summaryOffset);
    }
}
