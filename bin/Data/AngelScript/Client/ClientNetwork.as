#include "AngelScript/Utils/ClientUtils.as"

class ClientNetwork : ScriptObject
{
    protected int TIME_STAMP_SUBSTRING_START = 11;
    protected int TIME_STAMP_SUBSTRING_LENGTH = 8;

    ClientNetwork ()
    {

    }

    ~ClientNetwork ()
    {

    }

    void Start ()
    {
        SubscribeToEvent ("NetworkMessage", "HandleNetworkMessage");
        SubscribeToEvent ("NewNetworkTask", "HandleNewNetworkTask");
    }

    void Update (float timeStep)
    {

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
        else if (eventData ["MessageID"].GetInt () == STC_NETWORK_MESSAGE_TEXT_INFO_FROM_SERVER)
        {
            HandleTextInfoFromServer (eventData);
        }
        else if (eventData ["MessageID"].GetInt () == STC_NETWORK_MESSAGE_SEND_PLAYER_STATS)
        {
            HandlePlayerStatsMessage (eventData);
        }
        else if (eventData ["MessageID"].GetInt () == STC_NETWORK_MESSAGE_CHAT_MESSAGE)
        {
            HandleChatMessage (eventData);
        }
        else if (eventData ["MessageID"].GetInt () == STC_NETWORK_MESSAGE_GAME_ENDED)
        {
            HandleGameEndedMessage (eventData);
        }
    }

    void HandleNewNetworkTask (StringHash eventType, VariantMap &eventData)
    {
        int taskType = eventData ["taskType"].GetInt ();
        VectorBuffer buffer = eventData ["messageBuffer"].GetBuffer ();
        network.serverConnection.SendMessage (taskType, true, false, buffer);
    }

    void HandleGameStateMessage (VariantMap &eventData)
    {
        VectorBuffer buffer = eventData ["Data"].GetBuffer ();
        Node @scriptMain = GetScriptMain (node);

        int newGameState = buffer.ReadInt ();
        int oldGameState = scriptMain.vars ["gameState"].GetInt ();
        scriptMain.vars ["gameState"] = Variant (newGameState);

        if (oldGameState != newGameState)
        {
            VariantMap gameStateChangedEventData;
            gameStateChangedEventData ["oldGameState"] = Variant (oldGameState);
            gameStateChangedEventData ["newGameState"] = Variant (newGameState);
            SendEvent ("GameStateChanged", gameStateChangedEventData);
        }
    }

    void HandlePlayerStatsMessage (VariantMap &eventData)
    {
        VectorBuffer buffer = eventData ["Data"].GetBuffer ();
        Node @scriptMain = GetScriptMain (node);
        float gold = buffer.ReadFloat ();
        float points = buffer.ReadFloat ();
        scriptMain.vars ["gold"] = gold;
        scriptMain.vars ["points"] = points;
    }

    void HandleChatMessage (VariantMap &eventData)
    {
        VectorBuffer buffer = eventData ["Data"].GetBuffer ();
        Node @scriptMain = GetScriptMain (node);
        bool isPrivate = buffer.ReadBool ();
        String sender = buffer.ReadString ();
        String message = buffer.ReadString ();
        String timeStamp = time.timeStamp;
        timeStamp = timeStamp.Substring (TIME_STAMP_SUBSTRING_START, TIME_STAMP_SUBSTRING_LENGTH);

        Array <String> blockedUsersList = scriptMain.vars ["chatBlockedPlayersList"].GetStringVector ();
        if (blockedUsersList.Find (sender) < 0)
        {
            VariantMap messageEventData = VariantMap ();
            messageEventData ["isPrivate"] = Variant (isPrivate);
            messageEventData ["sender"] = Variant (sender);
            messageEventData ["message"] = Variant (message);
            messageEventData ["timeStamp"] = Variant (timeStamp);
            SendEvent ("NewChatMessage", messageEventData);
        }
    }

    void HandleTextInfoFromServer (VariantMap &eventData)
    {
        VectorBuffer buffer = eventData ["Data"].GetBuffer ();
        String message = buffer.ReadString ();
        String timeStamp = time.timeStamp;
        timeStamp = timeStamp.Substring (TIME_STAMP_SUBSTRING_START, TIME_STAMP_SUBSTRING_LENGTH);

        VariantMap messageEventData = VariantMap ();
        messageEventData ["isPrivate"] = false;
        messageEventData ["sender"] = "Host Automatics";
        messageEventData ["message"] = Variant (message);
        messageEventData ["timeStamp"] = Variant (timeStamp);
        SendEvent ("NewChatMessage", messageEventData);
    }

    void HandleGameEndedMessage (VariantMap &eventData)
    {
        VectorBuffer buffer = eventData ["Data"].GetBuffer ();
        String winnerName = buffer.ReadString ();
        String victoryType = buffer.ReadString ();
        String victoryInfo = buffer.ReadString ();

        VariantMap gameEndedEventData;
        gameEndedEventData ["winnerName"] = Variant (winnerName);
        gameEndedEventData ["victoryType"] = Variant (victoryType);
        gameEndedEventData ["victoryInfo"] = Variant (victoryInfo);
        SendEvent ("GameEnded", gameEndedEventData);
    }
};
