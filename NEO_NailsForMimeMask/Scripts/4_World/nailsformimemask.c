#ifdef SERVER

#ifdef GAMELABS

#ifndef NEO_LOG_TO_GAMELABS
#define NEO_LOG_TO_GAMELABS
static void NeoLogToGameLabs(PlayerBase player, string useditem="", string targetitem="", string actionperformed="")
{
	
#ifdef CFTOOLSEXTRALOGGING
	return SendToCFTools(player, useditem, targetitem, actionperformed);
#endif // CFTOOLSEXTRALOGGING

#ifndef CFTOOLSEXTRALOGGING
	_LogPlayerEx logplayer = new _LogPlayerEx(player);
	if (logplayer)
	{
        _Payload_ItemInteract payloadinteract = new _Payload_ItemInteract(logplayer, useditem, targetitem, actionperformed);
        GetGameLabs().GetApi().ItemInteract(new _Callback(), payloadinteract);
    }
#endif // undefined CFTOOLSEXTRALOGGING

}
#endif // NEO_LOG_TO_GAMELABS

#endif // GAMELABS


modded class MimeMask_Male : Mask_Base
{
	override bool ReplaceOnDeath()
	{
		GameInventory inv = GetInventory();
		if (inv)
		{
			if (!inv.IsInCargo() && !inv.IsAttachment())
			{
				return true;
			}
		}
		return super.ReplaceOnDeath();
	}

	override string GetDeadItemName()
	{
		return "NailBox";
	}
	
	override void EEKilled(Object killer)
	{
		if (ReplaceOnDeath())
		{
			// this should only get called during replacement so log here, the playersource stuff is from PluginAdminLog.c
			PlayerBase playerSource = PlayerBase.Cast( EntityAI.Cast( killer ).GetHierarchyParent() );
			if (!playerSource)
			{
				playerSource = PlayerBase.Cast( killer );
			}
			if (playerSource)
			{
				if (playerSource.GetIdentity())
				{
					if (playerSource.GetIdentity().GetName())
					{
						string message = string.Format("%1 turned MimeMask into Nails at %2", playerSource.GetIdentity().GetName(), GetPosition());
						GetGame().AdminLog(message);
#ifdef GAMELABS
						NeoLogToGameLabs(playerSource, "Mimemask", "Nails", "made ");
#endif //GAMELABS
					}
				}
			}
		}

		super.EEKilled(killer);
	}
	
}

#endif //SERVER