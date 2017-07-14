#include "AngelScript/Utils/Constants.as"
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
        SubscribeToEvent (EVENT_NEW_NETWORK_TASK, "HandleNewNetworkTask");
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
        else if (eventData ["MessageID"].GetInt () == STC_NETWORK_MESSAGE_DIPLOMACY_INFO)
        {
            HandleDiplomacyInfoMessage (eventData);
        }
        else if (eventData ["MessageID"].GetInt () == STC_NETWORK_MESSAGE_DIPLOMACY_OFFER)
        {
            HandleDiplomacyOfferMessage (eventData);
        }
    }

    void HandleNewNetworkTask (StringHash eventType, VariantMap &eventData)
    {
        int taskType = eventData [NewNetworkTask::TASK_TYPE].GetInt ();
        VectorBuffer buffer = eventData [NewNetworkTask::MESSAGE_BUFFER].GetBuffer ();
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
            gameStateChangedEventData [GameStateChanged::OLD_GAME_STATE] = Variant (oldGameState);
            gameStateChangedEventData [GameStateChanged::NEW_GAME_STATE] = Variant (newGameState);
            SendEvent (EVENT_GAME_STATE_CHANGED, gameStateChangedEventData);
        }
    }

    void HandlePlayerStatsMessage (VariantMap &eventData)
    {
        VectorBuffer buffer = eventData ["Data"].GetBuffer ();
        Node @scriptMain = GetScriptMain (node);
        float gold = buffer.ReadFloat ();
        float points = buffer.ReadFloat ();
        scriptMain.vars [ScriptMainVars::GOLD] = gold;
        scriptMain.vars [ScriptMainVars::POINTS] = points;
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
            messageEventData [NewChatMessage::IS_PRIVATE] = Variant (isPrivate);
            messageEventData [NewChatMessage::SENDER] = Variant (sender);
            messageEventData [NewChatMessage::MESSAGE] = Variant (message);
            messageEventData [NewChatMessage::TIME_STAMP] = Variant (timeStamp);
            SendEvent (EVENT_NEW_CHAT_MESSAGE, messageEventData);
        }
    }

    void HandleTextInfoFromServer (VariantMap &eventData)
    {
        VectorBuffer buffer = eventData ["Data"].GetBuffer ();
        String message = buffer.ReadString ();
        String timeStamp = time.timeStamp;
        timeStamp = timeStamp.Substring (TIME_STAMP_SUBSTRING_START, TIME_STAMP_SUBSTRING_LENGTH);

        VariantMap messageEventData = VariantMap ();
        messageEventData [NewChatMessage::IS_PRIVATE] = false;
        messageEventData [NewChatMessage::SENDER] = "Host Automatics";
        messageEventData [NewChatMessage::MESSAGE] = Variant (message);
        messageEventData [NewChatMessage::TIME_STAMP] = Variant (timeStamp);
        SendEvent (EVENT_NEW_CHAT_MESSAGE, messageEventData);
    }

    void HandleGameEndedMessage (VariantMap &eventData)
    {
        VectorBuffer buffer = eventData ["Data"].GetBuffer ();
        String winnerName = buffer.ReadString ();
        String victoryType = buffer.ReadString ();
        String victoryInfo = buffer.ReadString ();

        VariantMap gameEndedEventData;
        gameEndedEventData [GameEnded::WINNER_NAME] = Variant (winnerName);
        gameEndedEventData [GameEnded::VICTORY_TYPE] = Variant (victoryType);
        gameEndedEventData [GameEnded::VICTORY_INFO] = Variant (victoryInfo);
        SendEvent (EVENT_GAME_ENDED, gameEndedEventData);
    }

    void HandleDiplomacyInfoMessage (VariantMap &eventData)
    {
        VectorBuffer buffer = eventData ["Data"].GetBuffer ();
        StringHash infoType = buffer.ReadStringHash ();
        VariantMap infoData = buffer.ReadVariantMap ();

        VariantMap diplomacyInfoEventData;
        diplomacyInfoEventData [DiplomacyInfo::TYPE] = infoType;
        diplomacyInfoEventData [DiplomacyInfo::DATA] = infoData;
        SendEvent (EVENT_DIPLOMACY_INFO, diplomacyInfoEventData);
    }

    void HandleDiplomacyOfferMessage (VariantMap &eventData)
    {
        VectorBuffer buffer = eventData ["Data"].GetBuffer ();
        StringHash offerType = buffer.ReadStringHash ();
        uint offerId = buffer.ReadUInt ();
        float autodeclineTime = buffer.ReadFloat ();
        VariantMap offerData = buffer.ReadVariantMap ();

        VariantMap diplomacyOfferEventData;
        diplomacyOfferEventData [DiplomacyOffer::TYPE] = offerType;
        diplomacyOfferEventData [DiplomacyOffer::ID] = offerId;
        diplomacyOfferEventData [DiplomacyOffer::UNTIL_AUTODECLINE] = autodeclineTime - 1.0f;
        diplomacyOfferEventData [DiplomacyOffer::DATA] = offerData;
        SendEvent (EVENT_DIPLOMACY_OFFER, diplomacyOfferEventData);
    }
};
