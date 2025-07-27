

void NEO_AdminDismantlePartAfterInterrupt(PlayerBase player, Construction construction, string part_name, BaseBuildingBase bbb)
{
	
	if (construction && player)
	{
		if (player.NEO_admin_build_GetFastBuild())
		{
			construction.DismantlePartServer(player, part_name, AT_DISMANTLE_PART );
		}
		else if (player.NEO_admin_build_GetFullBuild())
		{
			if (bbb)
			{
				bbb.Delete();
			}
			else
			{
				Print("[NEO ADMIN BUILD] dismantle construction parent was null");
				return;
			}
		}
		else
		{
			Print("[NEO ADMIN BUILD] dismantle player wasn't fast or full builder, should not get here");
			return;
		}
	}
	else
	{
		Print("[NEO_ADMIN_BUILD] dismantle player or construction were null");
		return;
	}
	
	construction.NEO_admin_build_readynext();
}

void NEO_AdminDismantlePartInterrupt(PlayerBase player, Construction construction, string part_name, BaseBuildingBase bbb)
{
	if(player)
	{
		ActionManagerBase amb = player.GetActionManager();
		if (!amb)
		{
			Print("[NEO_ADMIN_BUID] NEO_AdminDismantlePartInterrupt unable to get player action manager");
			return;
		}		
		amb.Interrupt();
	}
	else
	{
		Print("[NEO_ADMIN_BUID] NEO_AdminDismantlePartInterrupt player was null");
		return;
	}
	GetGame().GetCallQueue(CALL_CATEGORY_SYSTEM).CallLater(NEO_AdminBuildPartAfterInterrupt, 100, false, player, construction, part_name, bbb);
}



modded class ActionDismantlePart
{
	
	override void OnStartServer(ActionData action_data)
	{
		super.OnStartServer(action_data);
		
		PlayerBase player = action_data.m_Player;
		
		if (!player.NEO_admin_build_GetEnabled())
		{
			return;
		}
		
		BaseBuildingBase base_building = BaseBuildingBase.Cast(action_data.m_Target.GetObject());
				
		Construction construction = base_building.GetConstruction();
		
		ConstructionActionData construction_action_data = action_data.m_Player.GetConstructionActionData();
		ConstructionPart construction_part = construction_action_data.GetTargetPart();
		
		string part_name = construction_part.GetName();
		

		GetGame().GetCallQueue(CALL_CATEGORY_SYSTEM).CallLater(NEO_AdminDismantlePartInterrupt, 100, false, player, construction, part_name, base_building);
	}
}