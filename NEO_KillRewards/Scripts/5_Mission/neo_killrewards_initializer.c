modded class MissionServer extends MissionBase
{
    override void OnInit()
    {
        super.OnInit();
        NEO_KillRewardsData.loadData();
    }
    
}
