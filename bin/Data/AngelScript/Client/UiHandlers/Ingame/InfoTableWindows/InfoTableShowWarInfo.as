#include "AngelScript/Utils/ClientUtils.as"

class InfoTableShowWarInfo : ScriptObject
{
    protected StringHash warHash_;

    InfoTableShowWarInfo ()
    {
        warHash_ = StringHash ();
    }

    ~InfoTableShowWarInfo ()
    {

    }

    void Start ()
    {

    }

    void Update (float timeStep)
    {
        Window @showWarInfoWindow = ui.root.GetChild ("ingame").GetChild ("infoTableWindow").GetChild ("showWarInfo");
        if (showWarInfoWindow.visible)
        {
            warHash_ = showWarInfoWindow.parent.vars ["elementToShowHash"].GetStringHash ();
            DiplomacyWar @war = GetWarByHash (scene, warHash_);
            if (war !is null)
            {
                Text @title = showWarInfoWindow.GetChild ("title");
                title.text = "War: " + ConstructWarShortname (scene, war) + ".";

                Text @infoText = showWarInfoWindow.GetChild ("infoText");
                String info = "Conflict escalation: " + Floor (war.conflictEscalation) + ".\n";

                info += "\nAttackers (" + war.attackersCount + "):\n";
                for (uint index = 0; index < war.attackersCount; index++)
                {
                    PlayerInfo @playerInfo = GetPlayerInfoByNameHash (scene, war.GetAttackerNameHashByIndex (index));
                    if (playerInfo !is null)
                    {
                        info += "    " + playerInfo.name;
                    }
                    else
                    {
                        info += "    Error: " + war.GetAttackerNameHashByIndex (index).ToString () + " not found!";
                    }
                    info += "\n";
                }

                info += "\nDefenders (" + war.defendersCount + "):\n";
                for (uint index = 0; index < war.defendersCount; index++)
                {
                    PlayerInfo @playerInfo = GetPlayerInfoByNameHash (scene, war.GetDefenderNameHashByIndex (index));
                    if (playerInfo !is null)
                    {
                        info += "    " + playerInfo.name;
                    }
                    else
                    {
                        info += "    Error: " + war.GetDefenderNameHashByIndex (index).ToString () + " not found!";
                    }
                    info += "\n";
                }
                infoText.text = info;
            }
        }
    }

    void Stop ()
    {

    }
}
