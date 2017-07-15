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

    protected String ProcessElementText (String text, int elementIndex) override
    {
        PlayerInfo @playerInfo = GetPlayerInfoByNameHash (scene, StringHash (text));
        String result = "" + (elementIndex + 1) + ". " + text;
        if (playerInfo !is null)
        {
            result += " [Found]";
        }
        else
        {
            result += " [Not Found]";
        }
        return result;
    }

    protected bool IsElementToAddCorrect (String element) override
    {
        Node @scriptMain = GetScriptMain (node);
        String playerName = scriptMain.vars [ScriptMainVars::PLAYER_NAME].GetString ();
        return (playerName != element);
    }

    PlayersNamesListEditor ()
    {

    }

    ~PlayersNamesListEditor ()
    {

    }
}
