

class NEO_KillRewardsItem
{
    string className;
    int probability;
    int quantity;
    void NEO_KillRewardsItem(string className, int probability, int quantity)
    {
        className = className;
        probability = probability;
        quantity = quantity;
    }
}
class NEO_KillRewardsTarget
{
    string className;
    ref array<NEO_KillRewardsItems> items;
}
    
class NEO_KillRewardsData
{
    static ref array<NEO_KillRewardsTarget> targets;

    string errorOut;
    
    static void spawnRewards(array<NEO_KillRewardsItem> rewards, vector position)
    {
        if (!rewards)
        {
            Print("Kill rewards array was empty");
            return;
        }
        int rand = Math.RandomInt(0,100);
#ifdef NEO_KILLREWARDS_DEBUG
        Print(string.Format("Rand int was: %1", rand));
#endif

        foreach (NEO_KillRewardsItem i: rewards)
        {
            if (!i)
            {
#ifdef NEO_KILLREWARDS_DEBUG
                Print("encountered null reward item");
#endif
                continue;
            }
#ifdef NEO_KILLREWARDS_DEBUG
            Print(string.Format("checking %1 prob %2", i.className, i.probability));
#endif
            if (rand < i.probability)
            {
#ifdef NEO_KILLREWARDS_DEBUG
            Print(string.Format("spawning %1 %2", i.quantity, i.className));
#endif

                for (int i = 0; i < i.quantity; i++)
                {
                    vector posRandom = MiscGameplayFunctions.GetRandomizedPosition(position, 0.5);
                    posRandom[1] = GetGame().SurfaceY([posRandom[0], posRandom[2]);
                    GetGame().CreateObjectEx(i.className, posRandom, ECE_PLACE_ON_SURFACE);
                }
            }
            rand = rand - i.probability;
        }
    }
    
    static void loadData()
    {
        targets = new array<NEO_KillRewardsTarget>;
        if (!targets)
        {
            Print("Could not allocate target array");
            return;
        }
        
        if (!FileExist(NEO_KILLREWARDS_FILENAME))
        {
            Print("No NEO Kill Rewards data file found: " + NEO_KILLREWARDS_FILENAME);
            Print("Running with defaults and creating file");

            NEO_KillRewardsTarget zombiemummy = new NEO_KillRewardsTarget;
            if (!zombiemummy)
            {
                Print("Could not allocate rewards target");
                return;
            }
            zombiemummy.className = "ZmbM_Mummy";
            zombiemummy.rewards = new array<NEO_KillRewardsItem>;
            if (!zombiemummy.rewards)
            {
                Print("Could not allocate rewards list");
                return;
            }
            
            NEO_KillRewardsTarget spookygoat = new NEO_KillRewardsTarget;
            if (!spookygoat)
            {
                Print("Could not allocate rewards target");
                return;
            }
            spookygoat.className = "Animal_CapraHircus_Spooky";
            spookygoat.rewards = new array<NEO_KillRewardsItem>;
            if (!spookygoat)
            {
                Print("Could not allocate rewards list");
                return;
            }
            
            NEO_KillRewardsItem cauldron = new NEO_KillRewardsItem("Cauldron", 10, 1);
            
            // 10% overall for a hood, red most rare
            NEO_KillRewardsItem hood1 = new NEO_KillRewardsItem("WitchHood_Black", 5, 1);
            NEO_KillRewardsItem hood2 = new NEO_KillRewardsItem("WitchHood_Brown", 3, 1);
            NEO_KillRewardsItem hood3 = new NEO_KillRewardsItem("WitchHood_Red", 2, 1);
            
            NEO KillRewardsItem nose = new NEO_KillRewardsItem("CrookedNose", 10, 1);
            
            NEO_KillRewardsItem broom = new NEO_KillRewardsItem("Broom_Birch", 10, 1);
            
            NEO_KillRewardsItem pumpkin_nvg = new NEO_KillRewardsItem("PumpkinHelmet", 10, 1);
            
            NEO_KillRewardsItem zagorky_chocolate = new NEO_KillRewardsItem("ZagorkyChocolate", 50, 1);
            
            if ((!cauldron) || (!hood1) || (!hood2) || (!hood3) || (!nose) || (!broom) || (!pumpkin_nvg) || (!zagorky_chocolate))
            {
                Print("Unable to allocate reward items");
                return;
            }

            zombiemummy.rewards.Insert(cauldron);
            zombiemummy.rewards.Insert(hood1);
            zombiemummy.rewards.Insert(hood2);
            zombiemummy.rewards.Insert(hood3);
            zombiemummy.rewards.Insert(nose);
            zombiemummy.rewards.Insert(broom);
            zombiemummy.rewards.Insert(pumpkin_nvg);
            zombiemummy.rewards.Insert(zagorky_chocolate);
            
            spookygoat.rewards.Insert(cauldron);
            spookygoat.rewards.Insert(hood1);
            spookygoat.rewards.Insert(hood2);
            spookygoat.rewards.Insert(hood3);
            spookygoat.rewards.Insert(nose);
            spookygoat.rewards.Insert(broom);
            spookygoat.rewards.Insert(pumpkin_nvg);
            spookygoat.rewards.Insert(zagorky_chocolate);
            
            targets.Insert(zombiemummy);
            targets.Insert(spookygoat);
            

            if (!JsonFileLoader<array<NEO_KillRewardsTarget>>.SaveFile(NEO_KILLREWARDS_FILENAME, targets, errorOut))
            {
                Print("Failed to write default file: "+ errorOut);
                return;
            }
        }
        
        if (!JsonFileLoader<array<NEO_KillRewardsTarget>>.LoadFile(NEO_KILLREWARDS_FILENAME, targets, errorOut))
        {
            Print("Unable to load json file with kill reward targets: " + errorOut);
            return;
        }
    }
}




modded class EntityAI extends Entity
{
    override void EEKilled(Object killer)
    {
        super.EEKilled(killer);
        if (!NEO_KillRewardsData.targets)
        {
            return;
        }
        
        foreach(NEO_KillRewardsTarget t: NEO_KillRewardsData.targets)
        {
            string type = this.GetType();
            string targetName = t.className;
            
            type.ToUpper();
            targetName.ToUpper();
            
            if (type == targetName)
            {
#ifdef NEO_KILLREWARDS_DEBUG
                Print(string.Format("player killed %1", t.className));
#endif
                
                spawnRewards(t.rewards, this.GetPosition());
            }
        }
    }
}