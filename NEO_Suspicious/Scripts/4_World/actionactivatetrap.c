modded class ActionActivateTrap : ActionContinuousBase
{
    // should get this in the admin log now
    override string GetAdminLogMessage(ActionData action_data)
    {
        return " activated  " + action_data.m_Target.GetObject().GetDisplayName();
    }
}