class CfgPatches
{
	class NEO_FlagTracker
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
	class NEO_FlagTracker
	{
		type = "mod";
		dir= "NEO_FlagTracker";
		name="NEO_FlagTracker";
		version="0.0.0.1";
		
		class defs
		{
			class worldScriptModule
			{
				value = "";
				files[] = { "NEO_FlagTracker/Scripts/4_World" };
			};
		};
	};
};