

void NEO_AdminBuildAfterPlaceInterrupt(PlayerBase player, KitBase kit, vector position, vector orientation)
{
	BaseBuildingBase newobject;
	if(player && kit)
	{
		kit.OnPlacementComplete(player, position, orientation);
		/* normal deploy action deletes the kit, so do so */
		kit.Delete(); // won't delete until next frame
		
		newobject = BaseBuildingBase.NEO_AdminBuildGetLatestStructure();
		
		if (newobject)
		{
			/* sanity check it's in the right place */
			if (vector.DistanceSq(position, newobject.GetPosition()) > 1)
			{
				Print("[NEO_ADMIN_BUILD] NEO_AdminBuildAfterPlaceInterrupt new item too far to be right one");
				return;
			}
			
			if (player.NEO_admin_build_GetFullBuild())
			{
				newobject.FullyBuild(player);
			}
			else if (player.NEO_admin_build_GetFastBuild())
			{
				newobject.NEO_admin_build_readynext();
			}
			else
			{
				Print("[NEO_ADMIN_BUILD] NEO_AdminBuildAfterPlaceInterrupt neither full nor fast build enabled, how did we get here");
			}
		}
		else
		{
			Print("[NEO_ADMIN_BUILD] NEO_AdminBuildAfterPlaceInterrupt failed to find new baes item");
		}
	}
	else
	{
		Print("[NEO_ADMIN_BUID] NEO_AdminBuildAfterPlaceInterrupt kit or player were null");
	}
}

void NEO_AdminBuildPlaceInterrupt(PlayerBase player, KitBase kit, vector position, vector orientation)
{
	if(player)
	{
		ActionManagerBase amb = player.GetActionManager();
		if (!amb)
		{
			Print("[NEO_ADMIN_BUID] NEO_AdminBuildPlaceInterrupt unable to get player action manager");
			return;
		}		
		amb.Interrupt();
	}
	else
	{
		Print("[NEO_ADMIN_BUID] NEO_AdminBuildPlaceInterrupt player was null");
		return;
	}
	GetGame().GetCallQueue(CALL_CATEGORY_SYSTEM).CallLater(NEO_AdminBuildAfterPlaceInterrupt, 100, false, player, kit, position, orientation);
}

modded class ActionDeployObject
{
	override void OnStartServer(ActionData action_data)
	{
		super.OnStartServer(action_data);
		
		PlaceObjectActionData poActionData = PlaceObjectActionData.Cast(action_data);
		if (!poActionData)
			return;
		
		PlayerBase deployplayer = poActionData.m_Player;
		if (!deployplayer)
		{
			return;
		}
		
		if (poActionData.m_MainItem.IsBasebuildingKit() && deployplayer.NEO_admin_build_GetEnabled())
		{
			KitBase kit = KitBase.Cast(poActionData.m_MainItem);
			if (!kit)
			{
				Print("[NEO_ADMIN_BUILD] ActionDeployObject failed to cast kit");
				return;
			}
			GetGame().GetCallQueue(CALL_CATEGORY_SYSTEM).CallLater(NEO_AdminBuildPlaceInterrupt, 100, false, deployplayer, kit, poActionData.m_Position, poActionData.m_Orientation);
		}
	}
};