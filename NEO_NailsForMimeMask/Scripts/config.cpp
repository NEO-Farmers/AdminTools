class CfgPatches
{
	class NEO_NailsForMimeMask
	{
		requiredAddons[] =
		{
			"DZ_Data"
		};
	};
};

class CfgMods
{
	class NEO_NailsForMimeMask
	{
		type = "mod";
		dir= "NEO_NailsForMimeMask";
		name="NEO_NailsForMimeMask";
		version="0.0.0.1";
		
		class defs
		{
			class worldScriptModule
			{
				value = "";
				files[] = { "NEO_NailsForMimeMask/Scripts/4_World" };
			};
		};
	};
};