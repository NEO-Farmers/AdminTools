class CfgPatches
{
	class NEO_SurvivorMissionsEnhancements
	{
		requiredAddons[] =
		{
			"SurvivorMissions"
		};
	};
};

class CfgMods
{
	class NEO_SurvivorMissionsEnhancements
	{
		type = "mod";
		dir= "NEO_SurvivorMissionsEnhancements";
		name="NEO_SurvivorMissionsEnhancements";
		version="0.0.0.1";
		
		class defs
		{
			class worldScriptModule
			{
				value = "";
				files[] = { "NEO_SurvivorMissionsEnhancements/Scripts/4_World" };
			};
			class missionScriptModule
			{
				value = "";
				files[] = { "NEO_SurvivorMissionsEnhancements/Scripts/5_Mission" };
			};
		};
	};
};