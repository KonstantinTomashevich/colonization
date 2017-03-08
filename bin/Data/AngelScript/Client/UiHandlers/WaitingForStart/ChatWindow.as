#include "AngelScript/Utils/ChatWindowInterface.as"

class ChatWindow : ChatWindowInterface
{
    protected UIElement @GetChatWindow () override
    {
        return ui.root.GetChild ("waitingForStart").GetChild ("chatWindow");
    }

    ChatWindow ()
    {

    }

    ~ChatWindow ()
    {

    }

    void HandleToggleChatWindowClick () override
    {
        // Ignore because hide button isn't visible.
    }

    void HandleShowBlockedUsersClick () override
    {
        VariantMap eventData;
        eventData ["windowName"] = Variant ("chatBlockedPlayersWindow");
        SendEvent ("ShowFunctionalWindowRequest", eventData);
    }

    void HandleShowPrivateUsersClick () override
    {
        VariantMap eventData;
        eventData ["windowName"] = Variant ("chatPrivateReceiversWindow");
        SendEvent ("ShowFunctionalWindowRequest", eventData);
    }
}
