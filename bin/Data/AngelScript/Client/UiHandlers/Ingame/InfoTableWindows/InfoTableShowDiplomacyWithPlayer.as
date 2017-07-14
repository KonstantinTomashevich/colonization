#include "AngelScript/Utils/Constants.as"
#include "AngelScript/Utils/ClientUtils.as"

class InfoTableShowDiplomacyWithPlayer : ScriptObject
{
    protected StringHash showedPlayerNameHash_;
    bool isLastShowedPlayerFound_;

    InfoTableShowDiplomacyWithPlayer ()
    {
        showedPlayerNameHash_ = StringHash ();
        isLastShowedPlayerFound_ = false;
    }

    ~InfoTableShowDiplomacyWithPlayer ()
    {

    }

    void Start ()
    {
        Window @showDiplomacyWithPlayer = ui.root.GetChild ("ingame").GetChild ("infoTableWindow").GetChild ("showDiplomacyWithPlayer");
        Button @declareWarButton = showDiplomacyWithPlayer.GetChild ("actionsButtons").GetChild ("declareWarButton");
        Button @sendPeaceTreatyButton = showDiplomacyWithPlayer.GetChild ("actionsButtons").GetChild ("sendPeaceTreatyButton");

        SubscribeToEvent (declareWarButton, "Released", "HandleDeclareWarClick");
        SubscribeToEvent (sendPeaceTreatyButton, "Released", "HandleSendPeaceTreatyClick");
    }

    void Update (float timeStep)
    {
        Window @showDiplomacyWithPlayer = ui.root.GetChild ("ingame").GetChild ("infoTableWindow").GetChild ("showDiplomacyWithPlayer");
        if (showDiplomacyWithPlayer.visible)
        {
            showedPlayerNameHash_ = showDiplomacyWithPlayer.parent.vars ["elementToShowHash"].GetStringHash ();
            PlayerInfo @playerInfo = GetPlayerInfoByNameHash (scene, showedPlayerNameHash_);
            if (playerInfo !is null)
            {
                Node @scriptMain = GetScriptMain (scene);
                String currentPlayerName = scriptMain.vars [ScriptMainVars::PLAYER_NAME].GetString ();

                Text @title = showDiplomacyWithPlayer.GetChild ("title");
                title.text = "Diplomacy with " + playerInfo.name + ".";

                BorderImage @colorSample = showDiplomacyWithPlayer.GetChild ("colorSample");
                colorSample.color = playerInfo.color;

                Text @infoText = showDiplomacyWithPlayer.GetChild ("infoText");
                String info = "Points: " + playerInfo.points + ".\n";

                Button @declareWarButton = showDiplomacyWithPlayer.GetChild ("actionsButtons").GetChild ("declareWarButton");
                Button @sendPeaceTreatyButton = showDiplomacyWithPlayer.GetChild ("actionsButtons").GetChild ("sendPeaceTreatyButton");

                if (playerInfo.IsAtWarWith (StringHash (currentPlayerName)))
                {
                    info += "At war with you!\n";
                    declareWarButton.visible = false;
                    sendPeaceTreatyButton.visible = true;
                }
                else if (playerInfo.name == currentPlayerName)
                {
                    info += "It's you.\n";
                    declareWarButton.visible = false;
                    sendPeaceTreatyButton.visible = false;
                }
                else
                {
                    info += "At peace with you.\n";
                    declareWarButton.visible = true;
                    sendPeaceTreatyButton.visible = false;
                }
                infoText.text = info;

                isLastShowedPlayerFound_ = true;
            }
            else
            {
                isLastShowedPlayerFound_ = false;
            }
        }
    }

    void Stop ()
    {

    }

    void HandleDeclareWarClick ()
    {
        VectorBuffer buffer = VectorBuffer ();
        buffer.WriteInt (PLAYER_ACTION_DECLARE_WAR);
        buffer.WriteStringHash (showedPlayerNameHash_);

        VariantMap taskData;
        taskData [NewNetworkTask::TASK_TYPE] = Variant (CTS_NETWORK_MESSAGE_SEND_PLAYER_ACTION);
        taskData [NewNetworkTask::MESSAGE_BUFFER] = Variant (buffer);
        SendEvent (EVENT_NEW_NETWORK_TASK, taskData);
    }

    void HandleSendPeaceTreatyClick ()
    {
        Node @scriptMain = GetScriptMain (scene);
        String currentPlayerName = scriptMain.vars [ScriptMainVars::PLAYER_NAME].GetString ();
        Array <DiplomacyWar @> wars = FindWarsWhereThesePlayersFight (scene, StringHash (currentPlayerName), showedPlayerNameHash_);
        if (wars.empty)
        {
            return;
        }

        // TODO: Later reimplement peace treaty request system!
        VectorBuffer buffer = VectorBuffer ();
        buffer.WriteInt (PLAYER_ACTION_SEND_PEACE_TREATY);
        buffer.WriteStringHash (wars [0].hash);
        buffer.WriteStringHash (showedPlayerNameHash_);

        VariantMap taskData;
        taskData [NewNetworkTask::TASK_TYPE] = Variant (CTS_NETWORK_MESSAGE_SEND_PLAYER_ACTION);
        taskData [NewNetworkTask::MESSAGE_BUFFER] = Variant (buffer);
        SendEvent (EVENT_NEW_NETWORK_TASK, taskData);
    }
}
