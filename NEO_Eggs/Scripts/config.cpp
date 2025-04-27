class CfgPatches
{
	class NEO_Eggs
	{
		requiredAddons[] =
		{
			"DZ_data_aiconfigs",
			"DZ_Data",
			"DZ_Scripts"
		};
	};
};

class CfgMods
{
	class NEO_Eggs
	{
		type = "mod";
		dir= "NEO_Eggs";
		name="NEO_Eggs";
		version="0.0.0.1";
		
		class defs
		{
			class worldScriptModule
			{
				value = "";
				files[] = { "NEO_Eggs/Scripts/4_World" };
			};
			class missionScriptModule
			{
				value = "";
				files[] = { "NEO_Eggs/Scripts/5_Mission" };
			};
		};
	};
};

class AIParams
{
	class AgentTeams
	{
		class Predator
		{
			friends[] -= {
				"Predator",
				"Zombies"
			};
		};
		class Zombies
		{
			friends[] -=
			{
				"Zombies",
				"Predator"
			};
		};
	};
};