class CfgPatches
{
	class NEO_KillRewards
	{
		requiredAddons[] =
		{
			// This should cause this mod to require CFTools
			// and hopefully load after
			"GameLabs_Scripts"
		};
	};
};

class CfgMods
{
	class NEO_KillRewards
	{
		type = "mod";
		dir= "NEO_KillRewards";
		name="NEO_KillRewards";
		version="0.0.0.1";
		
		class defs
		{
			class worldScriptModule
			{
				value = "";
				files[] = { "NEO_KillRewards/Scripts/4_World" };
			};
			class missionScriptModule
			{
				value = "";
				files[] = { "NEO_KillRewards/Scripts/5_Mission" };
			};
		};
	};
};