class CfgPatches
{
	class NEO_PumpkinGrenades
	{
		requiredAddons[] =
		{
			"DZ_Data"
		};
	};
};

class CfgMods
{
	class NEO_PumpkinGrenades
	{
		type = "mod";
		dir= "NEO_PumpkinGrenades";
		name="NEO_PumpkinGrenades";
		version="0.0.0.1";
		
		class defs
		{
			class worldScriptModule
			{
				value = "";
				files[] = { "NEO_PumpkinGrenades/Scripts/4_World" };
			};
			class missionScriptModule
			{
				value = "";
				files[] = { "NEO_PumpkinGrenades/Scripts/5_Mission" };
			};
		};
	};
};