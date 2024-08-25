
modded class DeployableContainer_Base : Container_Base
{
    override void EEKilled(Object killer)
    {
        
        super.EEKilled(killer);
        
        string culprit = "unknown";
        // steal player from killer used in admin log
        Object source = killer;
        PlayerBase playerSource = PlayerBase.Cast( EntityAI.Cast( source ).GetHierarchyParent() );
        if (!playerSource)
        {
            playerSource = PlayerBase.Cast( source );
        }
        
        if (playerSource)
        {
            PlayerIdentity pi = playerSource.GetIdentity();
            if (pi)
            {
                culprit = string.Format("%1 (%2)");
            }
#ifdef GAMELABS            
            NeoLogToGameLabs(playerSource, "weapon", "deployable container", "destroyed");
#endif
        }
        string message = string.Format("%1 destroyed deployable container at %2", culprit, GetPosition());
        GetGame().AdminLog(message);
    }
}