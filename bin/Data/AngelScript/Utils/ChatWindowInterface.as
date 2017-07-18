#include "AngelScript/Utils/ClientUtils.as"
#include "AngelScript/Utils/ScriptObjectWithBeforeStop.as"

abstract class ChatWindowInterface : ScriptObjectWithBeforeStop
{
    protected Array <VariantMap> messagesList_;
    protected uint messagesShowOffset_;
    protected float untilMessagesScrollUpdate_;
    protected float untilNewMessage_;

    protected uint MESSAGES_SCROLL_SPEED = 5;
    protected uint MAX_MESSAGES_IN_CACHE_COUNT = 40;
    protected uint MAX_MESSAGES_IN_PAGE_COUNT = 7;

    protected float CHAT_WINDOW_OPENED_YMODIFER = 0.35f;
    protected float CHAT_WINDOW_CLOSED_YMODIFER = 0.10f;

    ChatWindowInterface ()
    {
        untilNewMessage_ = 0.0f;
        untilMessagesScrollUpdate_ = 1.0f / (MESSAGES_SCROLL_SPEED * 1.0f);
        messagesShowOffset_ = MAX_MESSAGES_IN_PAGE_COUNT;
    }

    ~ChatWindowInterface ()
    {

    }

    void Start () override
    {
        ScriptObjectWithBeforeStop::Start ();

        Window @chatWindow = GetChatWindow ();
        Button @showHideButton = chatWindow.GetChild ("showHideButton");
        Button @sendPublicMessageButton = chatWindow.GetChild ("sendPublicMessage");
        Button @sendPrivateMessageButton = chatWindow.GetChild ("sendPrivateMessage");
        Button @showBlockedUsersButton = chatWindow.GetChild ("showBlockedUsersButton");
        Button @showPrivateUsersButton = chatWindow.GetChild ("showPrivateUsersButton");

        SubscribeToEvent (showHideButton, "Released", "HandleToggleChatWindowClick");
        SubscribeToEvent (sendPublicMessageButton, "Released", "HandleSendPublicMessageClick");
        SubscribeToEvent (sendPrivateMessageButton, "Released", "HandleSendPrivateMessageClick");
        SubscribeToEvent (showBlockedUsersButton, "Released", "HandleShowBlockedUsersClick");
        SubscribeToEvent (showPrivateUsersButton, "Released", "HandleShowPrivateUsersClick");
        SubscribeToEvent (EVENT_NEW_CHAT_MESSAGE, "HandleNewChatMessage");

        Node @scriptMain = GetScriptMain (node);
        LineEdit @messageEdit = chatWindow.GetChild ("messageEdit");
        RegisterLineEdit (scriptMain, messageEdit);
    }

    void Update (float timeStep)
    {
        untilMessagesScrollUpdate_ -= timeStep;
        if (untilNewMessage_ >= 0.0f)
        {
            untilNewMessage_ -= timeStep;
        }

        if (untilMessagesScrollUpdate_ <= 0.0f)
        {
            UpdateChatMessagesScroll ();
            untilMessagesScrollUpdate_ = 1.0f / (MESSAGES_SCROLL_SPEED * 1.0f);
        }

        UpdateChatMessages ();
        Window @chatWindow = GetChatWindow ();
        chatWindow.GetChild ("sendPublicMessage").visible = (untilNewMessage_ <= 0.0f);
        chatWindow.GetChild ("sendPrivateMessage").visible = (untilNewMessage_ <= 0.0f);
    }

    void Stop ()
    {
        UnsubscribeFromAllEvents ();
    }

    void BeforeStop (Scene @lastScene, Node @lastNode) override
    {
        Node @scriptMain = GetScriptMain (lastScene);
        Window @chatWindow = GetChatWindow ();
        if (scriptMain !is null and chatWindow !is null)
        {
            LineEdit @messageEdit = chatWindow.GetChild ("messageEdit");
            UnregisterLineEdit (scriptMain, messageEdit);
        }
    }

    void HandleToggleChatWindowClick ()
    {
        //! Will be implemented in inheritors!
    }

    void HandleSendPublicMessageClick ()
    {
        untilNewMessage_ = PLAYER_NEW_CHAT_MESSAGE_DELAY;
        Window @chatWindow = GetChatWindow ();
        LineEdit @messageEdit = chatWindow.GetChild ("messageEdit");

        String message = messageEdit.text;
        messageEdit.text = "";
        VectorBuffer buffer = VectorBuffer ();
        buffer.WriteString (message);

        VariantMap eventData;
        eventData [NewNetworkTask::TASK_TYPE] = Variant (CTS_NETWORK_MESSAGE_SEND_CHAT_MESSAGE);
        eventData [NewNetworkTask::MESSAGE_BUFFER] = Variant (buffer);
        SendEvent (EVENT_NEW_NETWORK_TASK, eventData);
    }

    void HandleSendPrivateMessageClick ()
    {
        untilNewMessage_ = PLAYER_NEW_CHAT_MESSAGE_DELAY;
        Node @scriptMain = GetScriptMain (node);
        Window @chatWindow = GetChatWindow ();
        LineEdit @messageEdit = chatWindow.GetChild ("messageEdit");

        String message = messageEdit.text;
        Array <String> receivers = scriptMain.vars ["chatPrivateReceiversList"].GetStringVector ();
        messageEdit.text = "";

        VectorBuffer buffer = VectorBuffer ();
        buffer.WriteString (message);
        for (uint index = 0; index < receivers.length; index++)
        {
            buffer.WriteString (receivers [index]);
        }

        VariantMap eventData;
        eventData [NewNetworkTask::TASK_TYPE] = Variant (CTS_NETWORK_MESSAGE_SEND_PRIVATE_MESSAGE);
        eventData [NewNetworkTask::MESSAGE_BUFFER] = Variant (buffer);
        SendEvent (EVENT_NEW_NETWORK_TASK, eventData);
    }

    void HandleShowBlockedUsersClick ()
    {
        //! Will be implemented in inheritors!
    }

    void HandleShowPrivateUsersClick ()
    {
        //! Will be implemented in inheritors!
    }

    void HandleNewChatMessage (StringHash eventType, VariantMap &eventData)
    {
        messagesList_.Push (eventData);
        while (messagesList_.length > MAX_MESSAGES_IN_CACHE_COUNT)
        {
            messagesList_.Erase (0);
        }
    }

    protected UIElement @GetChatWindow ()
    {
        //! Will be implemented in inheritors!
        return null;
    }

    protected void UpdateChatMessagesScroll ()
    {
        Window @chatWindow = GetChatWindow ();
        Button @earlierMessagesButton = chatWindow.GetChild ("earlierMessagesButton");
        Button @laterMessagesButton = chatWindow.GetChild ("laterMessagesButton");

        if (earlierMessagesButton.pressed)
        {
            messagesShowOffset_ += 1;
        }
        else if (laterMessagesButton.pressed)
        {
            messagesShowOffset_ -= 1;
        }
    }

    protected void UpdateChatMessages ()
    {
        String chatText = "";
        Window @chatWindow = GetChatWindow ();
        if (messagesList_.length > 0)
        {
            if (messagesShowOffset_ < MAX_MESSAGES_IN_PAGE_COUNT)
            {
                messagesShowOffset_ = MAX_MESSAGES_IN_PAGE_COUNT;
            }
            else if (messagesShowOffset_ > messagesList_.length and messagesList_.length > MAX_MESSAGES_IN_PAGE_COUNT)
            {
                messagesShowOffset_ = messagesList_.length;
            }

            int startIndex = messagesList_.length - messagesShowOffset_;
            if (startIndex < 0)
            {
                startIndex = 0;
            }

            Text @messagesInfo = chatWindow.GetChild ("messagesInfo");
            messagesInfo.text = "Showing messages from " + (startIndex + 1) + " to " + (startIndex + MAX_MESSAGES_IN_PAGE_COUNT) + ".";

            for (uint index = uint (startIndex);
                (index < messagesList_.length) and (index < (uint (startIndex) + MAX_MESSAGES_IN_PAGE_COUNT));
                index++)
            {
                VariantMap messageData = messagesList_ [index];
                bool isPrivate = messageData [NewChatMessage::IS_PRIVATE].GetBool ();
                String sender = messageData [NewChatMessage::SENDER].GetString ();
                String message = messageData [NewChatMessage::MESSAGE].GetString ();
                String timeStamp = messageData [NewChatMessage::TIME_STAMP].GetString ();

                chatText += (isPrivate ? "[Private] [" : "[Public] [") + timeStamp + "] " +
                            sender + ": " + message + "\n";
            }
        }

        Text @chatMessagesText = chatWindow.GetChild ("chatMessages");
        chatMessagesText.text = chatText;
    }
}
