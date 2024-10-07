#ifdef SERVER


const bool NEO_PUMPKIN_GRENADE_MISSION_DEBUG = false;
/* admin stuff */
/* IDs in this file will be able to run any command */
const string NEO_PUMPKIN_THROWER_ADMINS_FILE = "$profile:admin_powers.txt";

modded class MissionServer extends MissionBase
{
    ref TStringArray m_pumpkinThrowerAdmins;
    
    void MissionServer()
    {
         
        NEO_PumpkinGrenadeInfo.loadIDs();
        m_pumpkinThrowerAdmins = new TStringArray;
        if (m_pumpkinThrowerAdmins)
        {
            NEO_PumpkinUtil_LoadSteamIDsFromFiles(NEO_PUMPKIN_THROWER_ADMINS_FILE, m_pumpkinThrowerAdmins);
        }
        if (NEO_PUMPKIN_GRENADE_MISSION_DEBUG)
        {
            if (m_pumpkinThrowerAdmins)
            {
                foreach (string s: m_pumpkinThrowerAdmins)
                {
                    GetGame().AdminLog("admin: " + s);
                }
            }
            else
            {
                GetGame().AdminLog("admin list doesn't exist");
            }
        }
    }
    
    override void OnEvent(EventType eventTypeId, Param params)
    {
        super.OnEvent(eventTypeId, params);
        switch(eventTypeId)
        {
            // Handle user command
            case ChatMessageEventTypeID:

                ChatMessageEventParams chatParams;
                Class.CastTo(chatParams, params);
                if ((chatParams.param3 == "pumpkin_grenade_reload") || (chatParams.param3 == "/pumpkin_grenade_reload"))
                {
                    foreach (Man m: m_Players) // use built-in array
                    {
                        if (!m)
                        {
                            break;
                        }
                        PlayerIdentity pi = m.GetIdentity();
                        if (!pi)
                        {
                            break;
                        }
                        if (!m_pumpkinThrowerAdmins)
                        {
                            break;
                        }
                        if ((chatParams.param2 == pi.GetName()) && (m_pumpkinThrowerAdmins.Find(pi.GetPlainId()) != -1))
                        {
                            NEO_PumpkinGrenadeInfo.loadIDs();
                            
                            string loadmessage = "";
                            if (NEO_PumpkinGrenadeInfo.allthrow)
                            {
                                loadmessage = loadmessage + "All Throw: Enabled\n";
                            }
                            else
                            {
                                loadmessage = loadmessage + "All Throw: Disabled\n";
                            }
                            
                            if (NEO_PumpkinGrenadeInfo.userids)
                            {
                                loadmessage = loadmessage + "Thrower IDs:";
                                
                                foreach (string idstring: NEO_PumpkinGrenadeInfo.userids)
                                {
                                    loadmessage = loadmessage + " " + idstring + ",";
                                }
                                
                            }
                            NotificationSystem.SendNotificationToPlayerIdentityExtended(pi, 7.0, "Pumpkin Thrower Update", loadmessage, "set:dayz_inventory image:explosive");
                            
                            if (NEO_PUMPKIN_GRENADE_MISSION_DEBUG)
                            {
                                GetGame().AdminLog("reloaded pumpkin throwers");
                            }
                        }
                    }
                }
                break;
        }
    }
}


#endif //SERVER