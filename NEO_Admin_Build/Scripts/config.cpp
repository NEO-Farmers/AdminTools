class CfgPatches
{
	class NEO_Admin_Build
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
	class NEO_Admin_Build
	{
		type = "mod";
		dir= "NEO_Admin_Build";
		name="NEO_Admin_Build";
		version="0.0.0.1";
		
		class defs
		{
			class worldScriptModule
			{
				value = "";
				files[] = { "NEO_Admin_Build/Scripts/4_World" };
			};
			class missionScriptModule
			{
				value = "";
				files[] = { "NEO_Admin_Build/Scripts/5_Mission" };
			};
		};
	};
};