#ifdef SERVER

// mostly using the old easter egg code to determine when thrown and then we explode on contact or touch
// https://github.com/BohemiaInteractive/DayZ-Script-Diff/blob/a22a0553779b157f172283233e91933cc8ee2102/scripts/4_world/entities/itembase/gear/consumables/easteregg.c#L117C1-L135C3


const bool NEO_PumpkingGrenadeDebug = false;
const string NEO_PumpkinGrenadeFile = "$profile:NEO_Pumpkin_Throwers.txt";

class NEO_PumpkinGrenadeInfo
{
    static ref TStringArray userids;
    static bool allthrow = false;
    
    static void loadIDs ()
    {
        if (!NEO_PumpkinGrenadeInfo.userids)
        {
            NEO_PumpkinGrenadeInfo.userids = new TStringArray;
        }
        
        if (!NEO_PumpkinGrenadeInfo.userids)
        {
            GetGame().AdminLog("No pumpkin thrower array created");
            return;
        }
        
        NEO_PumpkinUtil_LoadSteamIDsFromFiles(NEO_PumpkinGrenadeFile, NEO_PumpkinGrenadeInfo.userids);
        if (NEO_PumpkingGrenadeDebug)
        {
            GetGame().AdminLog("pumpkin thrower count: " + NEO_PumpkinGrenadeInfo.userids.Count());
            GetGame().AdminLog("pumpkin throwers: ");
            foreach (string s: NEO_PumpkinGrenadeInfo.userids)
            {
                GetGame().AdminLog(s);
            }
        }
        
        if ((NEO_PumpkinGrenadeInfo.userids.Count() > 0) && (NEO_PumpkinGrenadeInfo.userids.Get(0)) && (NEO_PumpkinGrenadeInfo.userids.Get(0) == "*"))
        {
            if (NEO_PumpkingGrenadeDebug)
            {
                GetGame().AdminLog("ALL CAN THROW");
            }
            allthrow = true;
        }
        else
        {
            if (NEO_PumpkingGrenadeDebug)
            {
                GetGame().AdminLog("ONLY ALLOWED USERS CAN THROW");
            }
            allthrow = false;
        }
       
    }
}

modded class M67Grenade extends Grenade_Base
{
    bool NEO_i_am_a_pumpkin_now = false;
    
    void NEO_set_i_am_a_pumpkin (bool pumpkin_or_not)
    {
        NEO_i_am_a_pumpkin_now = pumpkin_or_not;
    }
    
    override string GetDisplayName()
    {
        if (NEO_i_am_a_pumpkin_now)
        {
            return "PumpkinGrenade";
        }
        return super.GetDisplayName();
    }
}

void NEO_PumpkinGrenade_nadeGoBoom(M67Grenade nade)
{
        nade.NEO_set_i_am_a_pumpkin(true);
        nade.ActivateImmediate();
        if (NEO_PumpkingGrenadeDebug)
        {
            GetGame().AdminLog("pumpkin nade told to go boom");
        }        
}

void NEO_PumpkinGrenade_makeNade(vector position)
{
        Object obnade = GetGame().CreateObject("M67Grenade", position, false, true, true ); //CreateObjectEx("M67Grenade", thepumpkin.GetPosition(), ECE_OBJECT_SWAP, RF_ORIGINAL);
        if (!obnade)
        {
            GetGame().AdminLog("Failed to create grenade from pumpkin");
            return;
        }
        
        M67Grenade nade = M67Grenade.Cast(obnade);
        if (!nade)
        {
            GetGame().AdminLog("Failed to cast new grenade object to grenade type");
            return;
        }
        
        GetGame().GetCallQueue(CALL_CATEGORY_SYSTEM).Call(NEO_PumpkinGrenade_nadeGoBoom,nade);
}

modded class Pumpkin : Edible_Base
{
    bool NEO_i_am_a_grenade_now = false;

    void NEO_goBoom()
    {
        //Explode(DT_EXPLOSION, "Explosion_NonLethal");
        if (NEO_PumpkingGrenadeDebug)
        {
            GetGame().AdminLog("Pumpkin should go boom now");
        }
        
        vector position = this.GetPosition();
        
        Delete();
        
        GetGame().GetCallQueue(CALL_CATEGORY_SYSTEM).Call(NEO_PumpkinGrenade_makeNade,position);
    }
    
    // hit other entities
    override void EOnTouch( IEntity other, int extra )
    {
        //if (NEO_PumpkingGrenadeDebug)
        //{
        //    GetGame().AdminLog("In EOnTouch, NEO_i_am_a_grenade_now is: " + NEO_i_am_a_grenade_now);
        //}
        if (NEO_i_am_a_grenade_now)
        {
            NEO_i_am_a_grenade_now = false;
            NEO_goBoom();
        }
    }
    
    // hit things like the ground
    override void EOnContact( IEntity other, Contact extra )
    {
        //if (NEO_PumpkingGrenadeDebug)
        //{
        //    GetGame().AdminLog("In EOnContact, NEO_i_am_a_grenade_now is: " + NEO_i_am_a_grenade_now);
        //}
        
        if (NEO_i_am_a_grenade_now)
        {
            NEO_i_am_a_grenade_now = false;
            NEO_goBoom();
        }
    }
    
    override void OnInventoryExit( Man player )
    {
        super.OnInventoryExit(player);
        NEO_i_am_a_grenade_now = false;
        
        if (NEO_PumpkingGrenadeDebug)
        {
            GetGame().AdminLog("In OnInventoryExit, NEO_i_am_a_grenade_now is: " + NEO_i_am_a_grenade_now);
        }
        
        // Easter egg code doesn't call super here... so let's hope for the best
        
        PlayerBase p = PlayerBase.Cast( player );
        if (p)
        {
            DayZPlayerImplementThrowing player_throwing = p.GetThrowing();
            if (player_throwing)
            {
                if (player_throwing.IsThrowingAnimationPlaying())
                {
                    if (NEO_PumpkingGrenadeDebug)
                    {
                        GetGame().AdminLog("Pumpkin was thrown by " + p.GetIdentity().GetPlainId());
                    }
                    
                    if (NEO_PumpkinGrenadeInfo.allthrow)
                    {
                        if (NEO_PumpkingGrenadeDebug)
                        {
                            GetGame().AdminLog("Pumpkin allthrow was set");
                        }
                        NEO_i_am_a_grenade_now = true;
                    }
                    else
                    {
                        if (NEO_PumpkingGrenadeDebug)
                        {
                            GetGame().AdminLog("Pumpkin allthrow was not set");
                        }
                        
                        if (NEO_PumpkinGrenadeInfo.userids)
                        {
                            PlayerIdentity pi = p.GetIdentity();
                            if (pi)
                            {
                                if (NEO_PumpkinGrenadeInfo.userids.Find(pi.GetPlainId()) != -1)
                                {
                                    if (NEO_PumpkingGrenadeDebug)
                                    {
                                        GetGame().AdminLog("Pumpkin was thrown by allowed player " + p.GetIdentity().GetPlainId());
                                    }
                                    NEO_i_am_a_grenade_now = true;
                                }
                                else // did not find player ID on list
                                {
                                    if (NEO_PumpkingGrenadeDebug)
                                    {
                                        GetGame().AdminLog("Pumpkin was thrown by non-allowed player " + p.GetIdentity().GetPlainId());
                                    }
                                    NEO_i_am_a_grenade_now = false;
                                }
                            }
                            else // no player id from player object
                            {
                                GetGame().AdminLog("Pumpkin grenades found no player id object");
                                NEO_i_am_a_grenade_now = false;
                            }
                        }
                        else  // no global throw and no userids
                        {
                            if (NEO_PumpkingGrenadeDebug)
                            {
                                GetGame().AdminLog("No all throw and no players in list to throw pumpkins");
                            }
                            NEO_i_am_a_grenade_now = false;
                        }
                    }
 
                    if (NEO_PumpkingGrenadeDebug)
                    {
                        GetGame().AdminLog("Pumpkin grenade is active");
                    }
                }
            }
        }
        
        if (NEO_PumpkingGrenadeDebug)
        {
            GetGame().AdminLog("Leaving OnInventoryExit, NEO_i_am_a_grenade_now is: " + NEO_i_am_a_grenade_now);
        }
    }
}

void NEO_PumpkinUtil_LoadSteamIDsFromFiles(string filePath, TStringArray arrayToLoad)
{
    if (arrayToLoad.Count() > 0)
    {
        arrayToLoad.Clear();
    }
    
    FileHandle file = OpenFile(filePath, FileMode.READ);
    
    // If file doesnt exist, create it
    if ( file == 0 ) {
        file = OpenFile(filePath, FileMode.WRITE);
        
        FPrintln(file, "// This file contains SteamID64 of all NEO Pumpkin Grenade ");
        FPrintln(file, "// Throwers. If the first line is a * then all players can.");
        FPrintln(file, "// Line starting with // means a comment line.");
        
        CloseFile(file);
        return;
    }
    
    string line;
    
    while ( FGets( file, line ) > 0 )
    {
        if (line.Length() < 1) continue;
        if (line.Get(0) + line.Get(1) == "//") continue;
        
        arrayToLoad.Insert(line);
    }

    CloseFile(file);
}

#endif // SERVER