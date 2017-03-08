#include "AngelScript/Utils/ClientUtils.as"
#include "AngelScript/Utils/StringListEditorUiHandler.as"

class PlayersNamesListEditor : StringListEditorUiHandler
{
    //! Will be setted in inheritors!
    protected String listVarName_;

    protected Window @GetWindow () override
    {
        //! Will be implemented in inheritors!
        return null;
    }

    protected Array <String> GetElements () override
    {
        Node @scriptMain = GetScriptMain (node);
        return scriptMain.vars [listVarName_].GetStringVector ();
    }

    protected void SetElements (Array <String> elements) override
    {
        Node @scriptMain = GetScriptMain (node);
        scriptMain.vars [listVarName_] = Variant (elements);
    }

    protected String ProcessElementText (String text) override
    {
        PlayerInfo @playerInfo = GetPlayerInfoByName (scene, text);
        if (playerInfo !is null)
        {
            return (text + " [Found]");
        }
        else
        {
            return (text + " [Not found]");
        }
    }

    protected bool IsElementToAddCorrect (String element) override
    {
        Node @scriptMain = GetScriptMain (node);
        String playerName = scriptMain.vars ["playerName"].GetString ();
        return (playerName != element);
    }

    PlayersNamesListEditor ()
    {

    }

    ~PlayersNamesListEditor ()
    {

    }
}
