modded class MissionServer
{
	ref Neo_Egg_Config nec;
	Neo_Egg_Config_Data necd;
	
	ref Neo_Egg_Prize nep;
	ref Neo_Egg_Arena nea;
	
	void MissionServer()
	{
		nec = new Neo_Egg_Config();
		if(!nec)
		{
			Print("Neo eggs failed to create base class");
			return;
		}
		
		nec.LoadConfig();
		necd = nec.GetConfigData();
		
		if (necd.enablePrizes)
		{
			nep = new Neo_Egg_Prize();
		}
		if (nep)
		{
			nep.StartPrizes(necd);
		}
		
		if (necd.enableArena)
		{
			nea = new Neo_Egg_Arena();
		}
		if (nea)
		{
			nea.StartArena(necd);
		}
		
	}
	
	void ~MissionServer()
	{
		nep = null;
		nec = null;
	}
}