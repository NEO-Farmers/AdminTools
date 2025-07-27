modded class MissionServer
{
	#ifdef GAMELABS
	override void GLActionRegisterHook()
	{
		super.GLActionRegisterHook();
		
		NEO_CFCloud_AdminBuild().Register();	
	}
	#endif
}

class NEO_CFCloud_AdminBuild extends GameLabsContextAction 
{
	void NEO_CFCloud_AdminBuild() 
	{
		this.actionCode = "NEO_CFCloud_AdminBuild";
		this.actionName = "Set admin build abilities";
		this.actionIcon = "hammer";
		this.actionColour = "default";
		this.actionContext = "player";
		
		GameLabsActionParameter adminBuildSetting = new GameLabsActionParameter("Admin Build Settings", "", "options");
		
		GameLabsActionParameter disable = new GameLabsActionParameter("Disable", "disable admin build", "string");
		disable.valueString = "Disable";
		adminBuildSetting.options.Insert("Disable", disable);
		
		GameLabsActionParameter full = new GameLabsActionParameter("Full", "perform instant full build", "string");
		full.valueString = "Full";
		adminBuildSetting.options.Insert("Full", full);
		
		GameLabsActionParameter fast = new GameLabsActionParameter("Fast", "build parts fast", "string");
		fast.valueString = "Fast";
		adminBuildSetting.options.Insert("Fast", fast);
		
		this.parameters.Insert("adminbuildsetting", adminBuildSetting);
	}
	
	override bool Execute(GameLabsActionContext context)
	{
		PlayerBase player;
		PlayerBase.CastTo(player, context.GetReferencedObject());
		
		if (!player)
		{
			GetGameLabs().GetLogger().Warn("Admin build player was null");
			return false;
		}
		string setting = context.parameters.Get("adminbuildsetting").GetString();
		
		if (setting == "Disable")
		{
			player.NEO_admin_build_clear();
		}
		else if (setting == "Full")
		{
			player.NEO_admin_build_SetFullBuild(true);
		}
		else if (setting == "Fast")
		{
			player.NEO_admin_build_SetFastBuild(true);
		}
		else
		{
			GetGameLabs().GetLogger().Warn("adminbuild invalid setting");
			return false;
		}
		
		GetGameLabs().GetLogger().Warn(string.Format("[AdminBuild] %1 %2", setting, player));
		return true;
	}
};