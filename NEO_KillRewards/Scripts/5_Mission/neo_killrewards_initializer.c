#ifdef SERVER

modded class MissionServer extends MissionBase
{
    void MissionServer ()
    {
        NEO_KillRewardsData.loadData();
    }
    
}

#endif // SERVER