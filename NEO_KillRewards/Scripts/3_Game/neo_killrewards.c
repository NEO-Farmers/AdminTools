#ifdef SERVER

const bool NEO_KILLREWARDS_DEBUG = true;

class NEO_KillRewardsItem
{
    string className;
    int probability;
    int quantity;
    void NEO_KillRewardsItem(string name, int probable, int howmany)
    {
        className = name;
        probability = probable;
        quantity = howmany;
    }
}

class NEO_KillRewardsTarget
{
    string className;
    ref array<NEO_KillRewardsItem> items;
}
    
typedef array<NEO_KillRewardsTarget> TNEO_KillRewardsTargetArray;

class NEO_KillRewardsData
{
    static ref TNEO_KillRewardsTargetArray targets;

    
    static void spawnRewards(array<NEO_KillRewardsItem> rewards, vector position)
    {
        if (!rewards)
        {
            GetGame().AdminLog("Kill rewards array was empty");
            return;
        }
        int rand = Math.RandomInt(0,100);
        
        if (NEO_KILLREWARDS_DEBUG)
        {
            GetGame().AdminLog(string.Format("Rand int was: %1", rand));
        }

        foreach (NEO_KillRewardsItem item: rewards)
        {
            if (!item)
            {
                if (NEO_KILLREWARDS_DEBUG)
                {
                    GetGame().AdminLog("encountered null reward item");
                }
                continue;
            }
            if (NEO_KILLREWARDS_DEBUG)
            {
                GetGame().AdminLog(string.Format("checking %1 prob %2", item.className, item.probability));
            }
            if (rand < item.probability)
            {
                if (NEO_KILLREWARDS_DEBUG)
                {
                    GetGame().AdminLog(string.Format("spawning %1 %2", item.quantity, item.className));
                }

                for (int j = 0; j < item.quantity; j++)
                {
                    vector targetPos = position;
                    int angle = Math.RandomIntInclusive(1,360);
                    float usedRadius = Math.RandomFloat01() * 0.5;
                    vector posRandom = Vector(targetPos[0] + (Math.Cos(angle) * usedRadius), targetPos[1], targetPos[2] + (Math.Sin(angle) * usedRadius));
                    posRandom[1] = GetGame().SurfaceY(posRandom[0], posRandom[2]);
                    GetGame().CreateObjectEx(item.className, posRandom, ECE_PLACE_ON_SURFACE);
                }
            }
            rand = rand - item.probability;
        }
    }
    
    static void loadData()
    {
        string errorOut;
        
        if (NEO_KILLREWARDS_DEBUG)
        {
            GetGame().AdminLog("Loading data for kill rewards");
        }
        
        targets = new TNEO_KillRewardsTargetArray;
        if (!targets)
        {
            GetGame().AdminLog("Could not allocate target array");
            return;
        }
        
        if (!FileExist(NEO_KILLREWARDS_FILENAME))
        {
            if (NEO_KILLREWARDS_DEBUG)
            {
                GetGame().AdminLog("No existing config found, creating a new one");
            }
            GetGame().AdminLog("No NEO Kill Rewards data file found: " + NEO_KILLREWARDS_FILENAME);
            GetGame().AdminLog("Running with defaults and creating file");

            NEO_KillRewardsTarget zombiemummy = new NEO_KillRewardsTarget;
            if (!zombiemummy)
            {
                GetGame().AdminLog("Could not allocate rewards target");
                return;
            }
            zombiemummy.className = "ZmbM_Mummy";
            zombiemummy.items = new array<NEO_KillRewardsItem>;
            if (!zombiemummy.items)
            {
                GetGame().AdminLog("Could not allocate rewards list");
                return;
            }
            
            NEO_KillRewardsTarget spookygoat = new NEO_KillRewardsTarget;
            if (!spookygoat)
            {
                GetGame().AdminLog("Could not allocate rewards target");
                return;
            }
            spookygoat.className = "Animal_CapraHircus_Spooky";
            spookygoat.items = new array<NEO_KillRewardsItem>;
            if (!spookygoat)
            {
                GetGame().AdminLog("Could not allocate rewards list");
                return;
            }
            
            NEO_KillRewardsItem cauldron = new NEO_KillRewardsItem("Cauldron", 10, 1);
            
            // 10% overall for a hood, red most rare
            NEO_KillRewardsItem hood1 = new NEO_KillRewardsItem("WitchHood_Black", 5, 1);
            NEO_KillRewardsItem hood2 = new NEO_KillRewardsItem("WitchHood_Brown", 3, 1);
            NEO_KillRewardsItem hood3 = new NEO_KillRewardsItem("WitchHood_Red", 2, 1);
            
            NEO_KillRewardsItem nose = new NEO_KillRewardsItem("CrookedNose", 10, 1);
            
            NEO_KillRewardsItem broom = new NEO_KillRewardsItem("Broom_Birch", 10, 1);
            
            NEO_KillRewardsItem pumpkin_nvg = new NEO_KillRewardsItem("PumpkinHelmet", 10, 1);
            
            NEO_KillRewardsItem zagorky_chocolate = new NEO_KillRewardsItem("ZagorkyChocolate", 50, 1);
            
            if ((!cauldron) || (!hood1) || (!hood2) || (!hood3) || (!nose) || (!broom) || (!pumpkin_nvg) || (!zagorky_chocolate))
            {
                GetGame().AdminLog("Unable to allocate reward items");
                return;
            }

            zombiemummy.items.Insert(cauldron);
            zombiemummy.items.Insert(hood1);
            zombiemummy.items.Insert(hood2);
            zombiemummy.items.Insert(hood3);
            zombiemummy.items.Insert(nose);
            zombiemummy.items.Insert(broom);
            zombiemummy.items.Insert(pumpkin_nvg);
            zombiemummy.items.Insert(zagorky_chocolate);
            
            spookygoat.items.Insert(cauldron);
            spookygoat.items.Insert(hood1);
            spookygoat.items.Insert(hood2);
            spookygoat.items.Insert(hood3);
            spookygoat.items.Insert(nose);
            spookygoat.items.Insert(broom);
            spookygoat.items.Insert(pumpkin_nvg);
            spookygoat.items.Insert(zagorky_chocolate);
            
            targets.Insert(zombiemummy);
            targets.Insert(spookygoat);
            

            if (!JsonFileLoader<TNEO_KillRewardsTargetArray>.SaveFile(NEO_KILLREWARDS_FILENAME, targets, errorOut))
            {
                GetGame().AdminLog("Failed to write default file: "+ errorOut);
                return;
            }
            return;
        }
        
        if (NEO_KILLREWARDS_DEBUG)
        {
            GetGame().AdminLog("Loading config");
        }
        
        if (!JsonFileLoader<TNEO_KillRewardsTargetArray>.LoadFile(NEO_KILLREWARDS_FILENAME, targets, errorOut))
        {
            GetGame().AdminLog("Unable to load json file with kill reward targets: " + errorOut);
            return;
        }
        return;
    }
}


modded class EntityAI extends Entity
{
    override void EEKilled(Object killer)
    {
        super.EEKilled(killer);
        
        // avoid things in cargo or attached to things
        GameInventory gi = this.GetInventory();
        if (gi)
        {
            if (gi.IsInCargo() || gi.IsAttachment())
            {
                if (NEO_KILLREWARDS_DEBUG)
                {
                    GetGame().AdminLog("Object was in cargo or attachment, not spawning kill reward");
                }
                return;
            }
        }

            
        if (!NEO_KillRewardsData.targets)
        {
            if (NEO_KILLREWARDS_DEBUG)
            {
                GetGame().AdminLog("No kill reward list specified");
            }
            return;
        }

        for (int i = 0; i < NEO_KillRewardsData.targets.Count(); i++)
        {
            NEO_KillRewardsTarget t = NEO_KillRewardsData.targets.Get(i);
            string type = this.GetType();
            string targetName = t.className;
            
            type.ToUpper();
            targetName.ToUpper();
            
            if (type == targetName)
            {
                if (NEO_KILLREWARDS_DEBUG)
                {
                    GetGame().AdminLog(string.Format("player killed %1", t.className));
                }
                
                spawnRewards(t.items, this.GetPosition());
            }
        }

    }
}

#endif //SERVER