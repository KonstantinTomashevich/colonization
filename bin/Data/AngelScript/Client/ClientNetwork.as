class ClientNetwork : ScriptObject
{
    protected int TIME_STAMP_SUBSCTRING_START = 11;
    protected int TIME_STAMP_SUBSCTRING_LENGTH = 8;

    ClientNetwork ()
    {

    }

    ~ClientNetwork ()
    {

    }

    void Start ()
    {
        node.vars ["TasksList"] = Array <Variant> ();
        SubscribeToEvent ("NetworkMessage", "HandleNetworkMessage");
    }

    void Update (float timeStep)
    {
        Array <Variant> tasksList = node.vars ["TasksList"].GetVariantVector ();
        while (!tasksList.empty)
        {
            VariantMap taskData = tasksList [0].GetVariantMap ();
            int taskType = taskData ["type"].GetInt ();

            if (taskType == CTS_NETWORK_MESSAGE_SEND_CHAT_MESSAGE or
                taskType == CTS_NETWORK_MESSAGE_SEND_PRIVATE_MESSAGE or
                taskType == CTS_NETWORK_MESSAGE_SEND_PLAYER_ACTION)
            {
                VectorBuffer buffer = taskData ["buffer"].GetBuffer ();
                network.serverConnection.SendMessage (taskType, true, false, buffer);
            }
            tasksList.Erase (0);
        }
        node.vars ["TasksList"] = tasksList;
    }

    void Stop ()
    {

    }

    void HandleNetworkMessage (StringHash eventType, VariantMap &eventData)
    {
        if (eventData ["MessageID"].GetInt () == STC_NETWORK_MESSAGE_SEND_GAME_STATE)
        {
            HandleGameStateMessage (eventData);
        }
        else if (eventData ["MessageID"].GetInt () == STC_NETWORK_MESSAGE_SEND_PLAYER_STATS)
        {
            HandlePlayerStatsMessage (eventData);
        }
        else if (eventData ["MessageID"].GetInt () == STC_NETWORK_MESSAGE_CHAT_MESSAGE)
        {
            HandleChatMessage (eventData);
        }
    }

    void HandleGameStateMessage (VariantMap &eventData)
    {
        VectorBuffer buffer = eventData ["Data"].GetBuffer ();
        node.parent.vars ["gameState"] = Variant (buffer.ReadInt ());
    }

    void HandlePlayerStatsMessage (VariantMap &eventData)
    {
        VectorBuffer buffer = eventData ["Data"].GetBuffer ();
        float gold = buffer.ReadFloat ();
        float points = buffer.ReadFloat ();
        node.parent.vars ["gold"] = gold;
        node.parent.vars ["points"] = points;
    }

    void HandleChatMessage (VariantMap &eventData)
    {
        VectorBuffer buffer = eventData ["Data"].GetBuffer ();
        bool isPrivate = buffer.ReadBool ();
        String sender = buffer.ReadString ();
        String message = buffer.ReadString ();
        String timeStamp = time.timeStamp;
        timeStamp = timeStamp.Substring (TIME_STAMP_SUBSCTRING_START, TIME_STAMP_SUBSCTRING_LENGTH);

        Array <String> blockedUsersList = node.parent.GetChild ("uiScriptNode").vars ["chatBlockedPlayersList"].GetStringVector ();
        if (blockedUsersList.Find (sender) < 0)
        {
            Array <Variant> messagesList = node.parent.GetChild ("uiScriptNode").vars ["messagesList"].GetVariantVector ();
            VariantMap messageData = VariantMap ();
            messageData ["isPrivate"] = Variant (isPrivate);
            messageData ["sender"] = Variant (sender);
            messageData ["message"] = Variant (message);
            messageData ["timeStamp"] = Variant (timeStamp);
            messagesList.Push (Variant (messageData));
            node.parent.GetChild ("uiScriptNode").vars ["messagesList"] = Variant (messagesList);
        }
    }
};
