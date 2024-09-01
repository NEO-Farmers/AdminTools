modded class ActionRestrainTarget: ActionContinuousBase
{
    override string GetAdminLogMessage(ActionData action_data)
    {
        PlayerBase targetPlayer = PlayerBase.Cast(action_data.m_Target.GetObject());
        string targetinfo = "unknown";
        if (targetPlayer)
        {
            PlayerIdentity pi = targetPlayer.GetIdentity();
            if (pi)
            {
                targetinfo = string.Format("%1 (%2)", pi.GetName(), pi.GetPlainId());
            }
        }
        
        
        
        return " restrained " +targetinfo + " with " + action_data.m_MainItem.GetDisplayName();
    }
}