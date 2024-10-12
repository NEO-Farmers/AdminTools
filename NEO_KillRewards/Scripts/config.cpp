class CfgPatches
{
	class NEO_KillRewards
	{
		requiredAddons[] =
		{
			"DZ_Data"
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
			class gameScriptModule
			{
				value="";
				files[]={"NEO_KillRewards/Scripts/3_Game"};
			};
/*
			class worldScriptModule
			{
				value = "";
				files[] = { "NEO_KillRewards/Scripts/4_World" };
			};
*/
			class missionScriptModule
			{
				value = "";
				files[] = { "NEO_KillRewards/Scripts/5_Mission" };
			};
		};
	};
};