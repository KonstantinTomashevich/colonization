#include "AngelScript/Utils/Constants.as"
#include "AngelScript/Utils/ClientUtils.as"

class DiplomacyMessagesProcessor : ScriptObject
{
    Array <VariantMap> diplomacyMessages_;

    protected void ProcessOffersAutodeclineTime (float timeStep)
    {
        uint index = 0;
        while (index < diplomacyMessages_.length)
        {
            VariantMap message = diplomacyMessages_ [index];
            if (message ["messageType"].GetUInt () == DIPLOMACY_MESSAGE_OFFER)
            {
                float untilAutodecline = message ["untilAutodecline"].GetFloat ();
                untilAutodecline -= timeStep;
                if (untilAutodecline <= 0.0f)
                {
                    diplomacyMessages_.Erase (index);
                }
                else
                {
                    message ["untilAutodecline"] = untilAutodecline;
                    diplomacyMessages_ [index] = message;
                    index++;
                }
            }
            else
            {
                index++;
            }
        }
    }

    protected void ProcessCurrentDiplomacyInfo (Window @diplomacyMessagesWindow, VariantMap &infoData)
    {
        StringHash infoType = infoData [DiplomacyInfo::TYPE].GetStringHash ();
        VariantMap infoDataMap = infoData [DiplomacyInfo::DATA].GetVariantMap ();

        Text @messageType = diplomacyMessagesWindow.GetChild ("messageType");
        if (infoType == DIPLOMACY_INFO_WAR_STARTED)
        {
            messageType.text = GetWarStartedInfoTitle (infoDataMap);
        }
        else if (infoType == DIPLOMACY_INFO_PEACE_TREATY_ACCEPTED)
        {
            messageType.text = GetPeaceTreatyAcceptedInfoTitle (infoDataMap);
        }
        else if (infoType == DIPLOMACY_INFO_PEACE_TREATY_DECLINED)
        {
            messageType.text = GetPeaceTreatyDeclinedInfoTitle (infoDataMap);
        }

        Text @messageText = diplomacyMessagesWindow.GetChild ("messageText");
        if (infoType == DIPLOMACY_INFO_WAR_STARTED)
        {
            messageText.text = GetWarStartedInfoText (infoDataMap);
        }
        else if (infoType == DIPLOMACY_INFO_PEACE_TREATY_ACCEPTED)
        {
            messageText.text = GetPeaceTreatyAcceptedInfoText (infoDataMap);
        }
        else if (infoType == DIPLOMACY_INFO_PEACE_TREATY_DECLINED)
        {
            messageText.text = GetPeaceTreatyDeclinedInfoText (infoDataMap);
        }

        diplomacyMessagesWindow.GetChild ("closeInfoButton").visible = true;
        diplomacyMessagesWindow.GetChild ("declineOfferButton").visible = false;
        diplomacyMessagesWindow.GetChild ("acceptOfferButton").visible = false;
    }

    protected void ProcessCurrentDiplomacyOffer (Window @diplomacyMessagesWindow, VariantMap &offerData)
    {
        StringHash offerType = offerData [DiplomacyOffer::TYPE].GetStringHash ();
        VariantMap offerDataMap = offerData [DiplomacyOffer::DATA].GetVariantMap ();

        Text @messageType = diplomacyMessagesWindow.GetChild ("messageType");
        if (offerType == DIPLOMACY_OFFER_PEACE)
        {
            messageType.text = GetPeaceOfferTitle (offerDataMap);
        }

        Text @messageText = diplomacyMessagesWindow.GetChild ("messageText");
        String text;
        if (offerType == DIPLOMACY_OFFER_PEACE)
        {
            text = GetPeaceOfferText (offerDataMap);
        }
        text += "\nUntil autodecline: " + Floor (offerData [DiplomacyOffer::UNTIL_AUTODECLINE].GetFloat ()) + " seconds.";
        messageText.text = text;

        diplomacyMessagesWindow.GetChild ("closeInfoButton").visible = false;
        diplomacyMessagesWindow.GetChild ("declineOfferButton").visible = true;
        diplomacyMessagesWindow.GetChild ("acceptOfferButton").visible = true;
    }

    protected String GetWarStartedInfoTitle (VariantMap &infoDataMap)
    {
        return "New war declared!";
    }

    protected String GetPeaceTreatyAcceptedInfoTitle (VariantMap &infoDataMap)
    {
        return "War ended!";
    }

    protected String GetPeaceTreatyDeclinedInfoTitle (VariantMap &infoDataMap)
    {
        return "War continued!";
    }

    protected String GetWarStartedInfoText (VariantMap &infoDataMap)
    {
        PlayerInfo @attacker = GetPlayerInfoByNameHash (scene, infoDataMap [DiplomacyInfoWarStarted_ATTACKER].GetStringHash ());
        PlayerInfo @defender = GetPlayerInfoByNameHash (scene, infoDataMap [DiplomacyInfoWarStarted_DEFENDER].GetStringHash ());
        return ((attacker !is null) ? attacker.name : "---error---") +
            " starts war against " + ((defender !is null) ? defender.name : "---error---") + "!";
    }

    protected String GetPeaceTreatyAcceptedInfoText (VariantMap &infoDataMap)
    {
        PlayerInfo @sender = GetPlayerInfoByNameHash (scene, infoDataMap [DiplomacyInfoPeaceTreatyAccepted_SENDER].GetStringHash ());
        PlayerInfo @anotherPlayer = GetPlayerInfoByNameHash (scene, infoDataMap [DiplomacyInfoPeaceTreatyAccepted_ANOTHER_PLAYER].GetStringHash ());
        return ((anotherPlayer !is null) ? anotherPlayer.name : "---error---") +
            " accepted peace treaty proposed by " +
            ((sender !is null) ? sender.name : "---error---") +
            ".\nThe war is over!";
    }

    protected String GetPeaceTreatyDeclinedInfoText (VariantMap &infoDataMap)
    {
        PlayerInfo @sender = GetPlayerInfoByNameHash (scene, infoDataMap [DiplomacyInfoPeaceTreatyDeclined_SENDER].GetStringHash ());
        PlayerInfo @anotherPlayer = GetPlayerInfoByNameHash (scene, infoDataMap [DiplomacyInfoPeaceTreatyDeclined_ANOTHER_PLAYER].GetStringHash ());
        return ((anotherPlayer !is null) ? anotherPlayer.name : "---error---") +
            " declined peace treaty proposed by " +
            ((sender !is null) ? sender.name : "---error---") +
            ".\nThe war continues!";
    }

    protected String GetPeaceOfferTitle (VariantMap &infoDataMap)
    {
        return "Do you want to make peace?";
    }

    protected String GetPeaceOfferText (VariantMap &infoDataMap)
    {
        PlayerInfo @enemy = GetPlayerInfoByNameHash (scene, infoDataMap [DiplomacyOfferPeace_ENEMY_NAME_HASH].GetStringHash ());
        return ((enemy !is null) ? enemy.name : "---error---") +
            " wants to make peace with you.\nDo you agree?";
    }

    DiplomacyMessagesProcessor ()
    {

    }

    ~DiplomacyMessagesProcessor ()
    {

    }

    void Start ()
    {
        Window @diplomacyMessagesWindow = ui.root.GetChild ("ingame").GetChild ("diplomacyMessagesWindow");
        Button @closeInfoButton = diplomacyMessagesWindow.GetChild ("closeInfoButton");
        Button @declineOfferButton = diplomacyMessagesWindow.GetChild ("declineOfferButton");
        Button @acceptOfferButton = diplomacyMessagesWindow.GetChild ("acceptOfferButton");

        SubscribeToEvent (closeInfoButton, "Released", "HandleCloseInfoClick");
        SubscribeToEvent (declineOfferButton, "Released", "HandleDeclineOfferClick");
        SubscribeToEvent (acceptOfferButton, "Released", "HandleAcceptOfferClick");
        SubscribeToEvent (EVENT_DIPLOMACY_INFO, "HandleDiplomacyInfo");
        SubscribeToEvent (EVENT_DIPLOMACY_OFFER, "HandleDiplomacyOffer");
    }

    void Update (float timeStep)
    {
        Window @diplomacyMessagesWindow = ui.root.GetChild ("ingame").GetChild ("diplomacyMessagesWindow");
        if (diplomacyMessages_.empty)
        {
            diplomacyMessagesWindow.visible = false;
        }
        else
        {
            ProcessOffersAutodeclineTime (timeStep);
            diplomacyMessagesWindow.visible = true;
            Text @title = diplomacyMessagesWindow.GetChild ("title");
            if (diplomacyMessages_.length == 1)
            {
                title.text = "Diplomacy | No new messages.";
            }
            else
            {
                title.text = "Diplomacy | " + (diplomacyMessages_.length - 1) + " new messages!";
            }

            VariantMap firstMessage = diplomacyMessages_ [0];
            if (firstMessage ["messageType"].GetUInt () == DIPLOMACY_MESSAGE_INFO)
            {
                ProcessCurrentDiplomacyInfo (diplomacyMessagesWindow, firstMessage);
            }
            else if (firstMessage ["messageType"].GetUInt () == DIPLOMACY_MESSAGE_OFFER)
            {
                ProcessCurrentDiplomacyOffer (diplomacyMessagesWindow, firstMessage);
            }
        }
    }

    void Stop ()
    {

    }

    void HandleDiplomacyInfo (StringHash eventType, VariantMap &eventData)
    {
        VariantMap data = eventData;
        data ["messageType"] = DIPLOMACY_MESSAGE_INFO;
        diplomacyMessages_.Push (data);
    }

    void HandleDiplomacyOffer (StringHash eventType, VariantMap &eventData)
    {
        VariantMap data = eventData;
        data ["messageType"] = DIPLOMACY_MESSAGE_OFFER;
        diplomacyMessages_.Push (data);
    }

    void HandleCloseInfoClick ()
    {
        diplomacyMessages_.Erase (0);
    }

    void HandleDeclineOfferClick ()
    {
        VariantMap firstMessage = diplomacyMessages_ [0];
        VectorBuffer buffer = VectorBuffer ();
        buffer.WriteInt (PLAYER_ACTION_RESPONCE_TO_DIPLOMACY_OFFER);
        buffer.WriteUInt (firstMessage [DiplomacyOffer::ID].GetUInt ());
        buffer.WriteUInt (DRPSTATUS_DECLINED);

        VariantMap taskData;
        taskData [NewNetworkTask::TASK_TYPE] = Variant (CTS_NETWORK_MESSAGE_SEND_PLAYER_ACTION);
        taskData [NewNetworkTask::MESSAGE_BUFFER] = Variant (buffer);
        SendEvent (EVENT_NEW_NETWORK_TASK, taskData);
        diplomacyMessages_.Erase (0);
    }

    void HandleAcceptOfferClick ()
    {
        VariantMap firstMessage = diplomacyMessages_ [0];
        VectorBuffer buffer = VectorBuffer ();
        buffer.WriteInt (PLAYER_ACTION_RESPONCE_TO_DIPLOMACY_OFFER);
        buffer.WriteUInt (firstMessage [DiplomacyOffer::ID].GetUInt ());
        buffer.WriteUInt (DRPSTATUS_ACCEPTED);

        VariantMap taskData;
        taskData [NewNetworkTask::TASK_TYPE] = Variant (CTS_NETWORK_MESSAGE_SEND_PLAYER_ACTION);
        taskData [NewNetworkTask::MESSAGE_BUFFER] = Variant (buffer);
        SendEvent (EVENT_NEW_NETWORK_TASK, taskData);
        diplomacyMessages_.Erase (0);
    }
}
