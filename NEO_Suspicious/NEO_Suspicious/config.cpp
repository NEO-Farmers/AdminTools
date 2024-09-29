class CfgPatches
{
	class NEO_Suspicious
	{
		requiredAddons[] =
		{
			"DZ_Data",
			"DZ_Scripts",
			// This should cause this mod to require CFTools
			// and hopefully load after
			"GameLabs_Scripts"
		};
	};
};

class CfgMods
{
	class NEO_Suspicious
	{
		type = "mod";
		dir= "NEO_Suspicious";
		name="NEO_Suspicious";
		version="0.0.0.1";
		
		class defs
		{
			class worldScriptModule
			{
				value = "";
				files[] = { "NEO_Suspicious/Scripts/4_World" };
			};
			class missionScriptModule
			{
				value = "";
				files[] = { "NEO_Suspicious/Scripts/5_Mission" };
			};
		};
	};
};