class CfgPatches
{
	class NEO_DeadlyDodgeball
	{
		requiredAddons[] =
		{
			"DZ_Data"
		};
	};
};

class CfgMods
{
	class NEO_DeadlyDodgeball
	{
		type = "mod";
		dir= "NEO_DeadlyDodgeball";
		name="NEO_DeadlyDodgeball";
		version="0.0.0.1";
		
		class defs
		{
			class worldScriptModule
			{
				value = "";
				files[] = { "NEO_DeadlyDodgeball/Scripts/4_World" };
			};
		};
	};
};