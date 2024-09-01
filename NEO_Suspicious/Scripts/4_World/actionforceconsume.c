modded class ActionForceConsume : ActionContinuousBase
{
    // for this to log to the admin log it needs to return a non-empty string
    // in GetAdminLogMessage. This override should catch:
    // ActionForceDrink, ActionForceFeed, ActionForceFeedSmall, ActionForceFeedCan
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
        
        
        
        return " forced " +targetinfo + " to consume " + action_data.m_MainItem.GetDisplayName();
    }
}