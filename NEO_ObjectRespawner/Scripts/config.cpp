class CfgPatches
{
	class NEO_ObjectRespawner
	{
		requiredAddons[] =
		{
		};
	};
};

class CfgMods
{
	class NEO_ObjectRespawner
	{
		type = "mod";
		dir= "NEO_ObjectRespawner";
		name="NEO_ObjectRespawner";
		version="0.0.0.1";
		
		class defs
		{
			class worldScriptModule
			{
				value = "";
				files[] = { "NEO_ObjectRespawner/Scripts/4_World" };
			};
		};
	};
};