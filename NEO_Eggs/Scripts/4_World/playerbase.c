modded class PlayerBase
{
	
	void Neo_Eggs_Arena_Msg(Neo_Egg_Arena nea)
	{
	}
		
	// need to make 'prize' monsters only target thrower and arena monsters attack no players 
	override bool CanBeTargetedByAI(EntityAI ai)
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
		if (ai_in_arena)
		{
			return false;
		}
		if ((ai_is_prize) && (ai_thrower != this))
		{
			return false;
		}
		
		return super.CanBeTargetedByAI(ai); // not a prize or in arena, vanilla behavior
	}
}