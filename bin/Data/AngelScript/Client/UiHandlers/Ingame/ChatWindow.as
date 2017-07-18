#include "AngelScript/Utils/ChatWindowInterface.as"

class ChatWindow : ChatWindowInterface
{
    ChatWindow ()
    {

    }

    ~ChatWindow ()
    {

    }

    void HandleToggleChatWindowClick () override
    {
        Window @chatWindow = GetChatWindow ();
        float windowYModifer = chatWindow.vars ["YModifer"].GetFloat ();
        if (windowYModifer == CHAT_WINDOW_CLOSED_YMODIFER)
        {
            chatWindow.vars ["YModifer"] = Variant (CHAT_WINDOW_OPENED_YMODIFER);
            Text @text = chatWindow.GetChild ("showHideButton").GetChild ("text");
            text.text = "Hide";
        }
        else
        {
            chatWindow.vars ["YModifer"] = Variant (CHAT_WINDOW_CLOSED_YMODIFER);
            Text @text = chatWindow.GetChild ("showHideButton").GetChild ("text");
            text.text = "Show";
        }
    }

    void HandleShowBlockedUsersClick () override
    {
        UIElement @uiRoot = GetChatWindow ().parent;
        uiRoot.GetChild ("chatBlockedPlayersWindow").visible = true;
        uiRoot.GetChild ("chatPrivateReceiversWindow").visible = false;
    }

    void HandleShowPrivateUsersClick () override
    {
        UIElement @uiRoot = GetChatWindow ().parent;
        uiRoot.GetChild ("chatBlockedPlayersWindow").visible = false;
        uiRoot.GetChild ("chatPrivateReceiversWindow").visible = true;
    }

    protected UIElement @GetChatWindow () override
    {
        return ui.root.GetChild ("ingame").GetChild ("chatWindow");
    }
}
