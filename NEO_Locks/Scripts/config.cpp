class CfgPatches
{
	class NEO_Locks
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
	class NEO_Locks
	{
		type = "mod";
		dir= "NEO_Locks";
		name="NEO_Locks";
		version="0.0.0.1";
		
		class defs
		{
			class worldScriptModule
			{
				value = "";
				files[] = { "NEO_Locks/Scripts/4_World" };
			};
			class missionScriptModule
			{
				value = "";
				files[] = { "NEO_Locks/Scripts/5_Mission" };
			};
		};
	};
};