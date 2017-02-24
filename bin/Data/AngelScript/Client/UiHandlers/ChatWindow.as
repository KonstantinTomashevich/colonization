#include "AngelScript/Utils/ClientUtils.as"

class ChatWindow : ScriptObject
{
    protected int messagesShowOffset_;
    protected float untilMessagesScrollUpdate_;
    protected float untilNewMessage_;

    protected int MESSAGES_SCROLL_SPEED = 5;
    protected int MAX_MESSAGES_IN_CACHE_COUNT = 40;
    protected int MAX_MESSAGES_IN_PAGE_COUNT = 7;

    protected float CHAT_WINDOW_OPENED_YMODIFER = 0.35f;
    protected float CHAT_WINDOW_CLOSED_YMODIFER = 0.10f;

    protected void UpdateChatMessagesScroll ()
    {
        Window @chatWindow = ui.root.GetChild ("ingame").GetChild ("chatWindow");
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
        Array <Variant> messagesList = node.parent.vars ["messagesList"].GetVariantVector ();
        while (messagesList.length > MAX_MESSAGES_IN_CACHE_COUNT)
        {
            messagesList.Erase (0);
        }

        String chatText = "";
        Window @chatWindow = ui.root.GetChild ("ingame").GetChild ("chatWindow");
        if (messagesList.length > 0)
        {
            if (messagesShowOffset_ < MAX_MESSAGES_IN_PAGE_COUNT)
            {
                messagesShowOffset_ = MAX_MESSAGES_IN_PAGE_COUNT;
            }
            else if (messagesShowOffset_ > messagesList.length and messagesList.length > MAX_MESSAGES_IN_PAGE_COUNT)
            {
                messagesShowOffset_ = messagesList.length;
            }

            int startIndex = messagesList.length - messagesShowOffset_;
            if (startIndex < 0)
            {
                startIndex = 0;
            }

            Text @messagesInfo = chatWindow.GetChild ("messagesInfo");
            messagesInfo.text = "Showing messages from " + (startIndex + 1) + " to " + (startIndex + MAX_MESSAGES_IN_PAGE_COUNT + 1) + ".";

            for (int index = startIndex;
                (index < messagesList.length) and (index < (startIndex + MAX_MESSAGES_IN_PAGE_COUNT));
                index++)
            {
                VariantMap messageData = messagesList [index].GetVariantMap ();
                bool isPrivate = messageData ["isPrivate"].GetBool ();
                String sender = messageData ["sender"].GetString ();
                String message = messageData ["message"].GetString ();
                String timeStamp = messageData ["timeStamp"].GetString ();

                chatText += (isPrivate ? "[Private] [" : "[Public] [") + timeStamp + "] " +
                            sender + ": " + message + "\n";
            }
        }

        Text @chatMessagesText = chatWindow.GetChild ("chatMessages");
        chatMessagesText.text = chatText;
    }

    ChatWindow ()
    {
        untilNewMessage_ = 0.0f;
        untilMessagesScrollUpdate_ = 1.0f / (MESSAGES_SCROLL_SPEED * 1.0f);
        messagesShowOffset_ = MAX_MESSAGES_IN_PAGE_COUNT;
    }

    ~ChatWindow ()
    {

    }

    void Start ()
    {
        Window @chatWindow = ui.root.GetChild ("ingame").GetChild ("chatWindow");
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

        LineEdit @messageEdit = chatWindow.GetChild ("messageEdit");
        RegisterLineEdit (node.parent.parent, messageEdit);
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
        ui.root.GetChild ("ingame").GetChild ("chatWindow").GetChild ("sendPublicMessage").visible = (untilNewMessage_ <= 0.0f);
        ui.root.GetChild ("ingame").GetChild ("chatWindow").GetChild ("sendPrivateMessage").visible = (untilNewMessage_ <= 0.0f);
    }

    void Stop ()
    {
        UnsubscribeFromAllEvents ();
    }

    void HandleToggleChatWindowClick ()
    {
        Window @chatWindow = ui.root.GetChild ("ingame").GetChild ("chatWindow");
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

    void HandleSendPublicMessageClick ()
    {
        untilNewMessage_ = PLAYER_NEW_CHAT_MESSAGE_DELAY;
        Window @chatWindow = ui.root.GetChild ("ingame").GetChild ("chatWindow");
        LineEdit @messageEdit = chatWindow.GetChild ("messageEdit");

        String message = messageEdit.text;
        messageEdit.text = "";
        VectorBuffer buffer = VectorBuffer ();
        buffer.WriteString (message);

        VariantMap eventData;
        eventData ["taskType"] = Variant (CTS_NETWORK_MESSAGE_SEND_CHAT_MESSAGE);
        eventData ["messageBuffer"] = Variant (buffer);
        SendEvent ("NewNetworkTask", eventData);
    }

    void HandleSendPrivateMessageClick ()
    {
        untilNewMessage_ = PLAYER_NEW_CHAT_MESSAGE_DELAY;
        Window @chatWindow = ui.root.GetChild ("ingame").GetChild ("chatWindow");
        LineEdit @messageEdit = chatWindow.GetChild ("messageEdit");

        String message = messageEdit.text;
        Array <String> receivers = node.parent.vars ["chatPrivateReceiversList"].GetStringVector ();
        messageEdit.text = "";

        VectorBuffer buffer = VectorBuffer ();
        buffer.WriteString (message);
        for (int index = 0; index < receivers.length; index++)
        {
            buffer.WriteString (receivers [index]);
        }

        VariantMap eventData;
        eventData ["taskType"] = Variant (CTS_NETWORK_MESSAGE_SEND_PRIVATE_MESSAGE);
        eventData ["messageBuffer"] = Variant (buffer);
        SendEvent ("NewNetworkTask", eventData);
    }

    void HandleShowBlockedUsersClick ()
    {
        ui.root.GetChild ("ingame").GetChild ("chatBlockedPlayersWindow").visible = true;
        ui.root.GetChild ("ingame").GetChild ("chatPrivateReceiversWindow").visible = false;
    }

    void HandleShowPrivateUsersClick ()
    {
        ui.root.GetChild ("ingame").GetChild ("chatBlockedPlayersWindow").visible = false;
        ui.root.GetChild ("ingame").GetChild ("chatPrivateReceiversWindow").visible = true;
    }
}
