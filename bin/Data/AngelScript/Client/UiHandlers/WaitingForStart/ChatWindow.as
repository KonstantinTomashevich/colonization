#include "AngelScript/Utils/Constants.as"
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
        // Ignore because hide button isn't visible.
    }

    void HandleShowBlockedUsersClick () override
    {
        VariantMap eventData;
        eventData [ShowFunctionalWindowRequest::WINDOW_NAME] = Variant ("chatBlockedPlayersWindow");
        SendEvent (EVENT_SHOW_FUNCTIONAL_WINDOW_REQUEST, eventData);
    }

    void HandleShowPrivateUsersClick () override
    {
        VariantMap eventData;
        eventData [ShowFunctionalWindowRequest::WINDOW_NAME] = Variant ("chatPrivateReceiversWindow");
        SendEvent (EVENT_SHOW_FUNCTIONAL_WINDOW_REQUEST, eventData);
    }

    protected UIElement @GetChatWindow () override
    {
        return ui.root.GetChild ("waitingForStart").GetChild ("chatWindow");
    }
}
