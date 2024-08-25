
modded class TripwireTrap : TrapBase
{
    const float m_neo_minimum_guilt_range_squared = 25.0; // 5 meters squared
    
    Man NeoGetSuspect()
    {
        Man suspect;
        float suspect_distance_squared = m_neo_minimum_guilt_range_squared; // default is out of range
        float distance;
        array<Man> playerlist;
        GetGame().GetPlayers(playerlist);
        if (playerlist.Count() <= 0)
        {
            return null; // maybe server start is attaching them as it loads
        }

        int index;
        for (index = 0; index < playerlist.Count(); index++)
        {
            Man current_suspect = playerlist.Get(index);
            if (current_suspect)
            {
                distance = vector.DistanceSq(GetPosition(), current_suspect.GetPosition());
                if (distance < suspect_distance_squared)
                {
                    suspect = current_suspect;
                    suspect_distance_squared = distance;
                }
            }
        }
        return suspect;
    }
    
    string NeoPlayerNameAndID (Man suspect)
    {
        string name_and_id = "";
        if(suspect)
        {
            PlayerBase player = PlayerBase.Cast(suspect);
            if (player)
            {
                PlayerIdentity pi = player.GetIdentity();
                if (pi)
                {
                    name_and_id = string.Format("%1 (%2)", pi.GetName(), pi.GetPlainId());
                }
            }
        }
        return name_and_id;
    }
    
    override void EEItemAttached(EntityAI item, string slot_name)
    {
        super.EEItemAttached(item,slot_name);
        Man suspect = NeoGetSuspect();
        string playerinfo = NeoPlayerNameAndID(suspect);
        GetGame().AdminLog(string.Format("%1 (guilt-by-closest-player) attached %2 to tripwire at %3", playerinfo, item.GetDisplayName(), GetPosition()));
#ifdef GAMELABS
        PlayerBase player = PlayerBase.Cast(suspect);
        if (player)
        {
            NeoLogToGameLabs(player, item.GetDisplayName(), "tripwire", "armed (guilt-by-closest-player)");
        }
#endif        
    }

    override void EEItemDetached(EntityAI item, string slot_name)
    {
        super.EEItemDetached(item,slot_name);
        Man suspect = NeoGetSuspect();
        string playerinfo = NeoPlayerNameAndID(suspect);
        GetGame().AdminLog(string.Format("%1 (guilt-by-closest-player) detached %2 from tripwire at %3", playerinfo, item.GetDisplayName(), GetPosition()));
#ifdef GAMELABS
        PlayerBase player = PlayerBase.Cast(suspect);
        if (player)
        {
            NeoLogToGameLabs(player, item.GetDisplayName(), "tripwire", "disarmed (guilt-by-closest-player)");
        }
#endif        
    }
}