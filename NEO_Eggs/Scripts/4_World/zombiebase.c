modded class ZombieBase
{
	bool m_neo_eggs_is_from_egg;
	bool m_neo_eggs_is_prize;
	bool m_neo_eggs_in_arena;
	Neo_Egg_Arena m_neo_eggs_arena;
	Man m_neo_eggs_thrower;

	bool Neo_Eggs_isAmbient()
	{
		return false;
	}
	
	bool Neo_Eggs_isBigGame()
	{
		return false;
	}
	
	bool Neo_Eggs_isPredator()
	{
		return false;
	}
	
	bool Neo_Eggs_getIsFromEgg()
	{
		return m_neo_eggs_is_from_egg;
	}
	
	void Neo_Eggs_setIsFromEgg(bool isFromEgg)
	{
		m_neo_eggs_is_from_egg = isFromEgg;
	}
	
	bool Neo_Eggs_getIsPrize()
	{
		return m_neo_eggs_is_prize;
	}
	
	void Neo_Eggs_setIsPrize(bool isPrize)
	{
		m_neo_eggs_is_prize = isPrize;
	}
	
	bool Neo_Eggs_getInArena()
	{
		return m_neo_eggs_in_arena;
	}
	
	void Neo_Eggs_setInArena(bool inArena)
	{
		m_neo_eggs_in_arena = inArena;
	}
	
	void Neo_Eggs_setArena(Neo_Egg_Arena nec)
	{
		m_neo_eggs_arena = nec;
	}
	
	Man Neo_Eggs_getThrower()
	{
		return m_neo_eggs_thrower;
	}
	
	void Neo_Eggs_setThrower(Man player)
	{
		m_neo_eggs_thrower = player;
	}
	
	void ZombieBase()
	{
		m_neo_eggs_is_from_egg = false;
		m_neo_eggs_is_prize = false;
		m_neo_eggs_in_arena = false;
		m_neo_eggs_thrower = null;
	}
	
	override bool CanBeTargetedByAI(EntityAI ai) // zombie and predator AIs no longer consider each other 'friends' by default
	{
		bool ai_from_egg;
		bool ai_is_prize;
		bool ai_in_arena;
		Man ai_thrower;
		
		/* sadly, have to split these since I can't mod DayZCreature */
		if (ai.IsZombie())
		{
			ZombieBase zb = ZombieBase.Cast(ai);
			if(zb)
			{
				ai_from_egg = zb.Neo_Eggs_getIsFromEgg();
				ai_is_prize = zb.Neo_Eggs_getIsPrize();
				ai_in_arena = zb.Neo_Eggs_getInArena();
				ai_thrower = zb.Neo_Eggs_getThrower();
			}
		}
		else if (ai.IsAnimal())
		{
			AnimalBase ab = AnimalBase.Cast(ai);
			if (ab)
			{
				if (!ab.Neo_Eggs_isPredator())
				{
					return super.CanBeTargetedByAI(ai); /* default behavior for non-predators */
				}
				ai_from_egg = ab.Neo_Eggs_getIsFromEgg();
				ai_is_prize = ab.Neo_Eggs_getIsPrize();
				ai_in_arena = ab.Neo_Eggs_getInArena();
				ai_thrower = ab.Neo_Eggs_getThrower();
			}
		}
		else
		{
			return super.CanBeTargetedByAI(ai); /* not a zombie and not a predator at this point, vanilla behavior */
		}
		
		/* zombies and predators both in the arena and not thrown by the same player should attack each other */
		if ((ai_in_arena && m_neo_eggs_in_arena) && (ai_thrower != m_neo_eggs_thrower))
		{
			return super.CanBeTargetedByAI(ai); /* since they no longer have each other listed as friends they should attack */
		}
		else
		{
			return false;
		}
	}
}
