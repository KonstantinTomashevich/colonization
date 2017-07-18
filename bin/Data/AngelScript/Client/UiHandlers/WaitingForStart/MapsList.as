#include "AngelScript/Utils/Constants.as"
#include "AngelScript/Utils/ClientUtils.as"
#include "AngelScript/Utils/StringListEditorUiHandler.as"

class MapsList : StringListEditorUiHandler
{
    MapsList ()
    {

    }

    ~MapsList ()
    {
        
    }

    void Update (float timeStep) override
    {
        StringListEditorUiHandler::Update (timeStep);

        Node @scriptMain = GetScriptMain (node);
        Window @window = GetWindow ();
        if (scriptMain !is null and window.visible and not scriptMain.vars [ScriptMainVars::IS_ADMIN].GetBool ())
        {
            Close ();
        }
        else if (scriptMain is null and window.visible)
        {
            Close ();
        }
    }

    // Remove buttons replaced by open buttons.
    void HandleRemoveElementClick (StringHash eventType, VariantMap &eventData) override
    {
        Node @scriptMain = GetScriptMain (node);
        Window @window = GetWindow ();
        if (scriptMain !is null and scriptMain.vars [ScriptMainVars::IS_ADMIN].GetBool ())
        {
            UIElement @element = eventData ["Element"].GetPtr ();
            int elementOffset = element.vars ["ElementOffset"].GetInt ();
            int summaryOffset = elementsShowOffset_ + elementOffset;

            String mapFolder = MAPS_FOLDER + scriptMain.vars ["mapsFoldersList"].GetStringVector () [summaryOffset];
            String mapInfoPath = mapFolder + MAP_INFO_FILE;

            VariantMap selectMapEventData;
            selectMapEventData [HostRequestSelectMap_MAP_FOLDER] = Variant (mapFolder);
            selectMapEventData [HostRequestSelectMap_MAP_INFO_PATH] = Variant (mapInfoPath);
            SendEvent (EVENT_HOST_REQUEST_SELECT_MAP, selectMapEventData);
            Close ();
        }
    }

    void HandleHideClick () override
    {
        Close ();
    }

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
        eventData [ShowFunctionalWindowRequest::WINDOW_NAME] = Variant ("actionsWindow");
        SendEvent (EVENT_SHOW_FUNCTIONAL_WINDOW_REQUEST, eventData);
    }
}
