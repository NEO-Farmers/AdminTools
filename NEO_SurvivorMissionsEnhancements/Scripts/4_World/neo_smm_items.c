modded class ItemBase
{

	bool m_neo_smm_isMissionObject = false;
	
	override void OnItemLocationChanged(EntityAI old_owner, EntityAI new_owner)
	{
		super.OnItemLocationChanged(old_owner, new_owner);
		
		if (!m_neo_smm_isMissionObject)
		{
			return;
		}
		
		PlayerBase pb = PlayerBase.Cast(new_owner);
		if (!pb)
		{
			return;
		}
		
		PlayerIdentity pi = pb.GetIdentity();
		if (!pi)
		{
			return;
		}
		
		NotificationSystem.SendNotificationToPlayerIdentityExtended(
			pi, 
			20.0, 
			"Mission Object Reminder", 
			"You have picked up a mission object. Mission objects are deleted at the end of missions. The *contents* of the reward containers are the mission prizes.",
			"set:dayz_inventory image:cat_common_cargo");
	
	}
}