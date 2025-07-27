

void NEO_AdminBuildPartAfterInterrupt(PlayerBase player, Construction construction, string part_name)
{
	
	if (construction && player)
	{
		construction.BuildPartServer(player, part_name, AT_BUILD_PART);
	}
	else
	{
		Print("[NEO_ADMIN_BUILD] buld part player or construction were null");
		return;
	}
	
	construction.NEO_admin_build_readynext();
}

void NEO_AdminBuildPartInterrupt(PlayerBase player, Construction construction, string part_name)
{
	if(player)
	{
		ActionManagerBase amb = player.GetActionManager();
		if (!amb)
		{
			Print("[NEO_ADMIN_BUID] NEO_AdminBuildPartInterrupt unable to get player action manager");
			return;
		}		
		amb.Interrupt();
	}
	else
	{
		Print("[NEO_ADMIN_BUID] NEO_AdminBuildPartInterrupt player was null");
		return;
	}
	GetGame().GetCallQueue(CALL_CATEGORY_SYSTEM).CallLater(NEO_AdminBuildPartAfterInterrupt, 100, false, player, construction, part_name);
}

modded class ActionBuildPart
{
	override void OnStartServer(ActionData action_data)
	{
		super.OnStartServer(action_data);
		
		PlayerBase player = action_data.m_Player;
		
		if (!player.NEO_admin_build_GetFastBuild())
		{
			return;
		}
		
		BaseBuildingBase base_building = BaseBuildingBase.Cast(action_data.m_Target.GetObject());
				
		Construction construction = base_building.GetConstruction();
		
		string part_name = BuildPartActionData.Cast(action_data).m_PartType;

		GetGame().GetCallQueue(CALL_CATEGORY_SYSTEM).CallLater(NEO_AdminBuildPartInterrupt, 100, false, player, construction, part_name);
	}
};