const string NEO_EGG_CONFIG_FILE = "$profile:NeoEggsConfig.json";

class Neo_Egg_Arena
{
	ref array<EasterEgg> arena_eggs;
	ref Timer m_arena_timer;
	
	static Neo_Egg_Config_Data m_config;
	
	
	void ~Neo_Egg_Arena()
	{
		if (m_arena_timer)
		{
			m_arena_timer.Stop();
		}
	}
	
	static bool IsInArena(vector pos)
	{
		if ((!m_config) || (!m_config.arenaBounds))
		{
			Print("Neo Egg attempted arena check when no arena defined");
			return false; // no arena polygon defined
		}
		
		pos[1] = 0;
		return Math2D.IsPointInPolygonXZ(m_config.arenaBounds, pos);
	}
	
	void StartArena(Neo_Egg_Config_Data necd)
	{
		m_config = necd;
		
		if (!m_config)
		{
			Print("Neo egg arena config was null");
			return;
		}
		
		arena_eggs = new array<EasterEgg>;
		m_arena_timer = new Timer();
		
		if(m_arena_timer)
		{
			m_arena_timer.Run(m_config.arenaRefresherTimerDuration, this, "SpawnEggs", NULL, true);
		}
		else
		{
			Print("Neo eggs arena timer didn't initialize");
			return;
		}
	}
	
	void SpawnEggs()
	{
		EasterEgg egg;
		int arena_index;
		Object o;
		vector pos;
		
		if (!arena_eggs)
		{
			Print("Neo eggs can't spawn arena eggs due to null array");
			return;
		}
		
		if ((!m_config) || (!m_config.monsterList) || (!m_config.arenaEggLocations))
		{
			Print("Neo eggs missing configs for arena");
			return;
		}
		
		
		if (arena_eggs.Count() == 0)
		{
			// create initial array with no eggs 
			for (arena_index = 0; arena_index < m_config.arenaEggLocations.Count(); ++arena_index)
			{
				arena_eggs.Insert(null);
			}
		}
			
		// replace any missing eggs
		int maxcount = m_config.arenaEggLocations.Count();
		if (arena_eggs.Count() < maxcount)
		{
			maxcount = arena_eggs.Count();
		}
		
		for (arena_index = 0; arena_index < maxcount; ++arena_index)
		{
			if (arena_eggs.Get(arena_index) == null)
			{
				pos = m_config.arenaEggLocations.Get(arena_index);
				o = GetGame().CreateObjectEx("EasterEgg", pos, ECE_SETUP | ECE_KEEPHEIGHT);
				if (!o)
				{
					Print("Failed to spawn an arena egg");
					return;
				}
				egg = EasterEgg.Cast(o);
				if (!egg)
				{
					Print("Neo egg somehow didn't get an egg in arena code");
					return;
				}
				egg.Neo_Eggs_SetArenaEgg(true);
				egg.Neo_Eggs_SetArenaManager(this);
				egg.Neo_Eggs_LoadEgg(m_config.monsterList.GetRandomElement());
				arena_eggs.Set(arena_index, egg);
			}
		}
	}
	
	void RemoveEgg(EasterEgg egg)
	{
		if (!arena_eggs)
		{
			Print("Neo eggs arena egg array was null");
			return;
		}
		if (!egg)
		{
			Print("Neo eggs prize can't remove null");
			return;
		}
		
		int index = arena_eggs.Find(egg);
		
		if ((index >= 0) && (index < arena_eggs.Count()))
		{
			arena_eggs.Set(index, null);
		}
	}
}

class Neo_Egg_Prize
{
	ref array<EasterEgg> prize_eggs;
	ref Timer m_prize_timer;
	Neo_Egg_Config_Data m_config;
	
	void ~Neo_Egg_Prize()
	{
		if (m_prize_timer)
		{
			m_prize_timer.Stop();
		}
	}
	
	void StartPrizes(Neo_Egg_Config_Data config)
	{
		m_config = config;
		
		if (!m_config)
		{
			Print("Neo egg prizes config was null");
			return;
		}
		
		prize_eggs = new array<EasterEgg>;
		m_prize_timer = new Timer();
		
		if(m_prize_timer)
		{
			m_prize_timer.Run(m_config.prizeRefresherTimerDuration, this, "SpawnEggs", NULL, true);
		}
		else
		{
			Print("Neo eggs prize timer didn't initialize");
			return;
		}
	}
	
	void SpawnEggs()
	{
		EasterEgg egg;
		int prize_index;
		
		if (!prize_eggs)
		{
			Print("Neo eggs can't spawn prize eggs due to null array");
			return;
		}
		
		if (prize_eggs.Count() != 0)
		{
			return; // prior set of eggs is still there
		}
		
		if ((!m_config) || (!m_config.prizeEggLocations) || (!m_config.monsterList) || (!m_config.prizeList))
		{
			Print("Neo eggs can't spawn prize eggs with null or invalid config");
			return;
		}
		
		foreach(vector pos: m_config.prizeEggLocations)
		{
			pos[1] = GetGame().SurfaceY(pos[0],pos[2]);
			Object o = GetGame().CreateObjectEx("EasterEgg", pos, ECE_SETUP | ECE_KEEPHEIGHT | ECE_PLACE_ON_SURFACE);
			if (!o)
			{
				Print("Neo egg failed to spawn a prize egg");
				this.ClearEggs();
				return;
			}
			egg = EasterEgg.Cast(o);
			if (!egg)
			{
				Print("Neo egg didn't get an egg on prize spawn");
				o.Delete();
				this.ClearEggs();
				return;
			}
			egg.Neo_Eggs_LoadEgg(m_config.monsterList.GetRandomElement());
			prize_eggs.Insert(egg);
		}
		
		// turn one random egg into a prize
		prize_index = prize_eggs.GetRandomIndex();
		if ((prize_index >= 0) && (prize_index < prize_eggs.Count()))
		{
			egg = prize_eggs.Get(prize_index);
			if (egg)
			{
				egg.Neo_Eggs_LoadEgg(m_config.prizeList.GetRandomElement());
			}
		}
		prize_index = 1;
		foreach (EasterEgg e: prize_eggs)
		{
			e.Neo_Eggs_SetPrizeEgg(true);
			e.Neo_Eggs_SetPrizeIndex(prize_index);
			e.Neo_Eggs_SetPrizeManager(this);
			++prize_index;
		}
	}
	
	void RemoveEgg(EasterEgg egg)
	{
		if (!prize_eggs)
		{
			Print("Neo eggs prize egg array was null");
			return;
		}
		if (!egg)
		{
			Print("Neo eggs prize can't remove null");
			return;
		}
		
		prize_eggs.RemoveItemUnOrdered(egg);
		
		this.ClearEggs();
	}
	
	void ClearEggs()
	{
		foreach(EasterEgg egg: prize_eggs)
		{
			if(egg)
			{
				egg.Delete();
			}
		}
		prize_eggs.Clear();
	}
}

class Neo_Egg_Config_Data
{
	bool enableArena;
	bool enablePrizes;
	
	float arenaRefresherTimerDuration;
	float prizeRefresherTimerDuration;
	
	ref array<vector> arenaBounds;
	ref array<vector> arenaEggLocations;
	ref array<vector> prizeEggLocations;
	
	ref array<string> monsterList;
	ref array<string> prizeList;
}

class Neo_Egg_Config
{
	ref Neo_Egg_Config_Data neo_egg_config_data;

	
	Neo_Egg_Config_Data GetConfigData()
	{
		return neo_egg_config_data;
	}
	
	void LoadConfig()
	{
		neo_egg_config_data = new Neo_Egg_Config_Data();
		string json_load_error;
		
		if (FileExist(NEO_EGG_CONFIG_FILE))
		{
			if (JsonFileLoader<Neo_Egg_Config_Data>.LoadFile(NEO_EGG_CONFIG_FILE, neo_egg_config_data, json_load_error))
			{
				return;
			}
			else
			{
				Print("Neo egg config loading error:");
				Print(json_load_error);
				Print("Will revert to default");
			}
		}
		Print("Neo Egg config setting defaults");
		this.SetDefaults();
		this.SaveConfig();
	}
	
	void SaveConfig()
	{
		if(!neo_egg_config_data)
		{
			Print("Neo egg could not save data, no data object");
			return;
		}
		string json_save_error;
		if (JsonFileLoader<Neo_Egg_Config_Data>.SaveFile(NEO_EGG_CONFIG_FILE, neo_egg_config_data, json_save_error))
		{
			return;
		}
		else
		{
			Print("Neo egg config saving error:");
			Print(json_save_error);
		}
	}
	
	void SetDefaults()
	{
		if (!neo_egg_config_data)
		{
			neo_egg_config_data = new Neo_Egg_Config_Data();
		}
		
		neo_egg_config_data.enableArena = true;
		neo_egg_config_data.enablePrizes = true;
		
		neo_egg_config_data.arenaRefresherTimerDuration = 60.0; // 1 minute
		neo_egg_config_data.prizeRefresherTimerDuration = 60.0;
		
		neo_egg_config_data.arenaBounds = new array<vector>;
		neo_egg_config_data.arenaBounds.Insert ("8814.42 0 2255.59");
		neo_egg_config_data.arenaBounds.Insert ("8724.68 0 2340.08");
		neo_egg_config_data.arenaBounds.Insert ("8662.95 0 2430.07");
		neo_egg_config_data.arenaBounds.Insert ("8607.51 0 2511.67");
		neo_egg_config_data.arenaBounds.Insert ("8533.38 0 2470.22");
		neo_egg_config_data.arenaBounds.Insert ("8503.38 0 2417.03");
		neo_egg_config_data.arenaBounds.Insert ("8550.16 0 2391.26");
		neo_egg_config_data.arenaBounds.Insert ("8679.10 0 2252.09");
		neo_egg_config_data.arenaBounds.Insert ("8748.83 0 2195.26");
		neo_egg_config_data.arenaBounds.Insert ("8817.20 0 2241.46");
		neo_egg_config_data.arenaBounds.Insert ("8814.42 0 2255.59");
		
		neo_egg_config_data.arenaEggLocations = new array<vector>; // these will be placed on a structure, preserve Y coordinate
		neo_egg_config_data.arenaEggLocations.Insert("8717.47 16.773 2340.01");
		neo_egg_config_data.arenaEggLocations.Insert("8718.0308 16.773 2339.1821");
		neo_egg_config_data.arenaEggLocations.Insert("8718.5916 16.773 2338.3542");
		neo_egg_config_data.arenaEggLocations.Insert("8719.1524 16.773 2337.5263");
		neo_egg_config_data.arenaEggLocations.Insert("8719.7132 16.773 2336.6984");
		
		neo_egg_config_data.prizeEggLocations = new array<vector>; // these will be placed on ground, so Y coord not important
		neo_egg_config_data.prizeEggLocations.Insert("4653.74 0 13327.50");
		neo_egg_config_data.prizeEggLocations.Insert("4654.4076 0 13328.2463");
		neo_egg_config_data.prizeEggLocations.Insert("4655.0752 0 13328.9926");
		
		neo_egg_config_data.monsterList = new array<string>;
		GetDefaultMonsters(neo_egg_config_data.monsterList);
		
		neo_egg_config_data.prizeList = new array<string>;
		GetDefaultPrizes(neo_egg_config_data.prizeList);
	}
	
	void GetDefaultMonsters(array<string> monsters)
	{
		if (!monsters)
		{
			Print("Neo egg could not load monsters into a null array");
		}
		// monsters will be chosen randomly, the more often they appear, the more likey they'll be chosen
		monsters.Clear();
		monsters.Insert("Animal_UrsusArctos");
		monsters.Insert("Animal_CanisLupus_Grey");
		monsters.Insert("Animal_CanisLupus_White");
		monsters.Insert("Zmbm_Mummy");
		monsters.Insert("ZmbM_NBC_Grey");
		monsters.Insert("ZmbM_NBC_White");
		monsters.Insert("ZmbM_NBC_Yellow");
		monsters.Insert("ZmbM_PatrolNormal_Autumn");
		monsters.Insert("ZmbM_PatrolNormal_Flat");
		monsters.Insert("ZmbM_PatrolNormal_PautRev");
		monsters.Insert("ZmbM_PatrolNormal_Summer");
		monsters.Insert("ZmbM_PatrolNormal_Winter");
		monsters.Insert("ZmbM_PolicemanFat");
		monsters.Insert("ZmbM_PolicemanSpecForce");
		monsters.Insert("ZmbM_PolicemanSpecForce_Heavy");
		monsters.Insert("ZmbM_PrisonerSkinny");
		monsters.Insert("ZmbM_Santa");
		monsters.Insert("ZmbM_SoldierNormal");
		monsters.Insert("ZmbM_eastSoldier_Heavy_Navy");
		monsters.Insert("ZmbM_eastSoldier_normal_Navy");
		monsters.Insert("ZmbM_usSoldier_Heavy_Woodland");
		monsters.Insert("ZmbM_usSoldier_Officer_Desert");
		monsters.Insert("ZmbM_usSoldier_normal_Desert");
		monsters.Insert("ZmbM_usSoldier_normal_Woodland");
		monsters.Insert("ZmbF_PoliceWomanNormal");
	}
	
	void GetDefaultPrizes(array<string> prizes)
	{
		if (!prizes)
		{
			Print("Neo egg could not load prizes into a null array");
		}
		// prizes will be chosen randomly, the more often they appear, the more likey they'll be chosen
		prizes.Clear();
		
		prizes.Insert("WeaponCleaningKit");
		prizes.Insert("WeaponCleaningKit");
		prizes.Insert("WeaponCleaningKit");
		prizes.Insert("WeaponCleaningKit");
		prizes.Insert("WeaponCleaningKit");
		
		prizes.Insert("Whetstone");
		prizes.Insert("Whetstone");
		prizes.Insert("Whetstone");
		prizes.Insert("Whetstone");
		prizes.Insert("Whetstone");
		
		prizes.Insert("NailBox");
		prizes.Insert("NailBox");
		prizes.Insert("NailBox");
		
		prizes.Insert("AmmoBox_00buck_10rnd");
		prizes.Insert("AmmoBox_12gaRubberSlug_10Rnd");
		prizes.Insert("AmmoBox_12gaSlug_10Rnd");
		prizes.Insert("AmmoBox_22_50Rnd");
		prizes.Insert("AmmoBox_308WinTracer_20Rnd");
		prizes.Insert("AmmoBox_308Win_20Rnd");
		prizes.Insert("AmmoBox_357_20Rnd");
		prizes.Insert("AmmoBox_380_35rnd");
		prizes.Insert("AmmoBox_45ACP_25rnd");
		prizes.Insert("AmmoBox_545x39Tracer_20Rnd");
		prizes.Insert("AmmoBox_545x39_20Rnd");
		prizes.Insert("AmmoBox_556x45Tracer_20Rnd");
		prizes.Insert("AmmoBox_556x45_20Rnd");
		prizes.Insert("AmmoBox_762x39Tracer_20Rnd");
		prizes.Insert("AmmoBox_762x39_20Rnd");
		prizes.Insert("AmmoBox_762x54Tracer_20Rnd");
		prizes.Insert("AmmoBox_762x54_20Rnd");
		prizes.Insert("AmmoBox_9x19_25rnd");
		prizes.Insert("AmmoBox_9x39AP_20Rnd");
		prizes.Insert("AmmoBox_9x39_20Rnd");
		
		prizes.Insert("Barrel_Blue");
		prizes.Insert("Barrel_Green");
		prizes.Insert("Barrel_Red");
		prizes.Insert("Barrel_Yellow");
		
		prizes.Insert("Barrel_Blue");
		prizes.Insert("Barrel_Green");
		prizes.Insert("Barrel_Red");
		prizes.Insert("Barrel_Yellow");
		
		prizes.Insert("Barrel_Blue");
		prizes.Insert("Barrel_Green");
		prizes.Insert("Barrel_Red");
		prizes.Insert("Barrel_Yellow");
		
		prizes.Insert("Barrel_Blue");
		prizes.Insert("Barrel_Green");
		prizes.Insert("Barrel_Red");
		prizes.Insert("Barrel_Yellow");
		
		prizes.Insert("Barrel_Blue");
		prizes.Insert("Barrel_Green");
		prizes.Insert("Barrel_Red");
		prizes.Insert("Barrel_Yellow");
		
		prizes.Insert("SledgeHammer");
		prizes.Insert("SledgeHammer");
		
		prizes.Insert("Pickaxe");
		prizes.Insert("Pickaxe");
		
		prizes.Insert("FirefighterAxe_Green");
		prizes.Insert("FirefighterAxe_Black");
		
		prizes.Insert("M4A1_Black");
		
		prizes.Insert("M14");
		
		prizes.Insert("M16A2");
		
		prizes.Insert("Mosin9130");
		prizes.Insert("Mosin9130_Black");
		prizes.Insert("Mosin9130_Camo");
		
		prizes.Insert("SKS");
		
		prizes.Insert("PlateCarrierVest");
		prizes.Insert("PlateCarrierVest_Black");
		prizes.Insert("PlateCarrierVest_Camo");
		
		prizes.Insert("NVGoggles");
		
		prizes.Insert("HuntingBag_Hannah");
	}
}