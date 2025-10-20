const string NEORespawnString = "neorespawn";
const string NEORespawnReportHitString = "neoreporthit";
const string NEORespawnReportTakeString = "neoreporttake";

const string NEORespawnTakeMessage = "Replacing respawning object, do not abuse this feature, it is logged";
const int NEORespawnTime = 5;
const float NEORespawnNotificationTime = 8.0;

#define NEORESPAWNERDEBUG

class NEORespawner
{
	static bool CheckRespawn (ITEM_SpawnerObject item)
	{
		string customStringCopy = item.customString;
		customStringCopy.ToLower();
		if (customStringCopy.Contains(NEORespawnString))
		{
#ifdef NEORESPAWNERDEBUG
			Print("[NEORESPANWER] will respawn: " + item);
#endif 
			return true;
		}
		return false;
	}
	
	static bool CheckReportHit (ITEM_SpawnerObject item)
	{
		string customStringCopy = item.customString;
		customStringCopy.ToLower();
		if (customStringCopy.Contains(NEORespawnReportHitString))
		{
#ifdef NEORESPAWNERDEBUG
			Print("[NEORESPANWER] will report hits: " + item);
#endif 
			return true;
		}
		return false;
	}
	
	static bool CheckReportTake (ITEM_SpawnerObject item)
	{
		string customStringCopy = item.customString;
		customStringCopy.ToLower();
		if (customStringCopy.Contains(NEORespawnReportTakeString))
		{
#ifdef NEORESPAWNERDEBUG
			Print("[NEORESPANWER] will report takes: " + item);
#endif 
			return true;
		}
		return false;
	}
	
	static void SendNotification(PlayerIdentity identity, string message, string icon)
	{
		NotificationSystem.SendNotificationToPlayerIdentityExtended(identity, NEORespawnNotificationTime, "Hit", message, icon);
	}
	
	static void ReportHit(EntityAI hitObject, EntityAI source, TotalDamageResult damageResult, string dmgZone, bool isAlive)
	{
		if (!hitObject)
		{
			return;
		}
		
		if (!source)
		{
			return;
		}
		
		string message;
		string icon;
		PlayerBase pb = PlayerBase.Cast(source.GetHierarchyRootPlayer());
		PlayerIdentity pi;
		if (!pb)
		{
			return; // no player to report to
		}
		
		pi = pb.GetIdentity();
		
		if (!pi)
		{
			return;
		}
		
		message = pi.GetName();
		
		if (isAlive)
		{
			message += " hit ";
			icon = "set:dayz_gui image:scope";			
		}
		else
		{
			message += " killed ";
			icon = "set:dayz_gui image:iconSkull";
		}
		
		message += hitObject.GetDisplayName() + " ";
		if (dmgZone)
		{
			message += "hit in " + dmgZone + " ";
		}
		
		if ( damageResult )	
		{
			float dmg = damageResult.GetHighestDamage("Health");
			message += "for " + dmg.ToString() + " dmg ";
		}
		
		message += "range " + vector.Distance(hitObject.GetPosition(), pb.GetPosition()) + "m";
		
		SendNotification(pi, message, icon);
	}
		
	static void ReportPickup(ITEM_SpawnerObject item, EntityAI new_owner)
	{
		// log pick up in case someone is abusing respawning items
		string message = "[NEORESPANWER] item picked up: " + item.ToString();
		PlayerBase pb = PlayerBase.Cast(new_owner);
		if (pb)
		{
			PlayerIdentity pi = pb.GetIdentity();
			if (pi)
			{
				message += " by " + pi.GetName() + " (" + pi.GetPlainId() + ")";
				SendNotification(pi, NEORespawnTakeMessage, "set:dayz_inventory image:hands");
			}
			Print(message);
		}
	}
	
	static Timer CreateAndStartRespawnTimer(EntityAI respawningObject)
	{
		Timer t = new Timer();
		if (t)
		{
			t.Run(NEORespawnTime, respawningObject, "Delete");
#ifdef NEORESPAWNERDEBUG
			Print("[NEORESPANWER] created respawn timer for: " + respawningObject);
#endif 
			return t;
		}
		
		Print("[NEORESPANWER] failed to create respawn timer for: " + respawningObject);
		
		return null;
	}
	
}


// had to mod player/zombie/animal/items separately, due to unable to mod lower classes
// mostly all originally implemented in playerbase, then copy paste to the others, with addditions
// to animals and zombies add an init ai call and items catch player pickups to respawn.
//----------------------------------------------
modded class PlayerBase
{
	bool m_NEORespawner_respawn = false;
	bool m_NEORespawner_reporthit = false;
	
	ref ITEM_SpawnerObject m_NEORespawner_item; // hold a copy of original object spawner data
	ref Timer m_NEORespawner_timer;
		
	override void OnSpawnByObjectSpawner(ITEM_SpawnerObject item)
	{
		super.OnSpawnByObjectSpawner(item);

		m_NEORespawner_reporthit = NEORespawner.CheckReportHit(item);
		
		if (NEORespawner.CheckRespawn(item))
		{
			m_NEORespawner_respawn = true;
			m_NEORespawner_item = item;
		}
	}

	override void EEHitBy(TotalDamageResult damageResult, int damageType, EntityAI source, int component, string dmgZone, string ammo, vector modelPos, float speedCoef)
	{
		super.EEHitBy(damageResult, damageType, source, component, dmgZone, ammo, modelPos, speedCoef);
		
		if (m_NEORespawner_reporthit)
		{
			NEORespawner.ReportHit(this, source, damageResult, dmgZone, IsAlive());
		}
	}
	
	override void EEKilled(Object killer)
	{
		super.EEKilled(killer);
		
		if (m_NEORespawner_respawn)
		{
			m_NEORespawner_timer = NEORespawner.CreateAndStartRespawnTimer(this);
#ifdef NEORESPAWNERDEBUG
			Print("[NEORESPANWER] killed, started timer: " + m_NEORespawner_item);
#endif 
		}
	}
	
	override void EEDelete(EntityAI parent)
	{
		if (m_NEORespawner_respawn)
		{
			ObjectSpawnerHandler.SpawnObject(m_NEORespawner_item);
#ifdef NEORESPAWNERDEBUG
			Print("[NEORESPANWER] respawning: " + m_NEORespawner_item);
#endif 
		}
		
		super.EEDelete(parent);
	}
}

//----------------------------------------------
modded class ZombieBase
{
	bool m_NEORespawner_respawn = false;
	bool m_NEORespawner_reporthit = false;

	ref ITEM_SpawnerObject m_NEORespawner_item; // hold a copy of original object spawner data
	ref Timer m_NEORespawner_timer;
	AIGroup m_NEORespawner_aigroup;
	
	override void OnSpawnByObjectSpawner(ITEM_SpawnerObject item)
	{
		super.OnSpawnByObjectSpawner(item);
		
		m_NEORespawner_reporthit = NEORespawner.CheckReportHit(item);		
		
		if (NEORespawner.CheckRespawn(item))
		{
			m_NEORespawner_respawn = true;
			m_NEORespawner_item = item;
				
			World w = g_Game.GetWorld();
			if (!w)
			{
				return;
			}
			AIWorld aiw = w.GetAIWorld();
			if (!aiw)
			{
				return;
			}
			m_NEORespawner_aigroup = aiw.CreateDefaultGroup();
			this.InitAIAgent(m_NEORespawner_aigroup);
		}
		
		
	}

	override void EEHitBy(TotalDamageResult damageResult, int damageType, EntityAI source, int component, string dmgZone, string ammo, vector modelPos, float speedCoef)
	{
		super.EEHitBy(damageResult, damageType, source, component, dmgZone, ammo, modelPos, speedCoef);
		
		if (m_NEORespawner_reporthit)
		{
			NEORespawner.ReportHit(this, source, damageResult, dmgZone, IsAlive());
		}
	}
	
	override void EEKilled(Object killer)
	{
		super.EEKilled(killer);
		
		if (m_NEORespawner_respawn)
		{
			m_NEORespawner_timer = NEORespawner.CreateAndStartRespawnTimer(this);
#ifdef NEORESPAWNERDEBUG
			Print("[NEORESPANWER] killed, started timer: " + m_NEORespawner_item);
#endif 
		}
	}
	
	override void EEDelete(EntityAI parent)
	{
		if (m_NEORespawner_respawn)
		{
			ObjectSpawnerHandler.SpawnObject(m_NEORespawner_item);
#ifdef NEORESPAWNERDEBUG
			Print("[NEORESPANWER] respawning: " + m_NEORespawner_item);
#endif 
		}
		
		super.EEDelete(parent);
	}
}

//----------------------------------------------
modded class AnimalBase
{
	bool m_NEORespawner_respawn = false;
	bool m_NEORespawner_reporthit = false;
	
	ref ITEM_SpawnerObject m_NEORespawner_item; // hold a copy of original object spawner data
	ref Timer m_NEORespawner_timer;
	AIGroup m_NEORespawner_aigroup;
		
	override void OnSpawnByObjectSpawner(ITEM_SpawnerObject item)
	{
		super.OnSpawnByObjectSpawner(item);

		m_NEORespawner_reporthit = NEORespawner.CheckReportHit(item);
		
		if (NEORespawner.CheckRespawn(item))
		{
			m_NEORespawner_respawn = true;
			m_NEORespawner_item = item;
			
			m_NEORespawner_reporthit = NEORespawner.CheckReportHit(item);
			
			World w = g_Game.GetWorld();
			if (!w)
			{
				return;
			}
			AIWorld aiw = w.GetAIWorld();
			if (!aiw)
			{
				return;
			}
			m_NEORespawner_aigroup = aiw.CreateDefaultGroup();
			this.InitAIAgent(m_NEORespawner_aigroup);
		}
	}

	override void EEHitBy(TotalDamageResult damageResult, int damageType, EntityAI source, int component, string dmgZone, string ammo, vector modelPos, float speedCoef)
	{
		super.EEHitBy(damageResult, damageType, source, component, dmgZone, ammo, modelPos, speedCoef);
		
		if (m_NEORespawner_reporthit)
		{
			NEORespawner.ReportHit(this, source, damageResult, dmgZone, IsAlive());
		}
	}
	
	override void EEKilled(Object killer)
	{
		super.EEKilled(killer);
		
		if (m_NEORespawner_respawn)
		{
			m_NEORespawner_timer = NEORespawner.CreateAndStartRespawnTimer(this);
#ifdef NEORESPAWNERDEBUG
			Print("[NEORESPANWER] killed, started timer: " + m_NEORespawner_item);
#endif 
		}
	}
	
	override void EEDelete(EntityAI parent)
	{
		if (m_NEORespawner_respawn)
		{
			ObjectSpawnerHandler.SpawnObject(m_NEORespawner_item);
#ifdef NEORESPAWNERDEBUG
			Print("[NEORESPANWER] respawning: " + m_NEORespawner_item);
#endif 
		}
		
		super.EEDelete(parent);
	}
}

//----------------------------------------------
modded class ItemBase
{
	bool m_NEORespawner_respawn = false;
	bool m_NEORespawner_reporthit = false;
	bool m_NEORespawner_reporttake = false;
	
	ref ITEM_SpawnerObject m_NEORespawner_item; // hold a copy of original object spawner data
	ref Timer m_NEORespawner_timer;
		
	override void OnSpawnByObjectSpawner(ITEM_SpawnerObject item)
	{
		super.OnSpawnByObjectSpawner(item);
		
		m_NEORespawner_reporthit = NEORespawner.CheckReportHit(item);
		m_NEORespawner_reporttake = NEORespawner.CheckReportTake(item);
		
		if (NEORespawner.CheckRespawn(item))
		{
			m_NEORespawner_respawn = true;
			m_NEORespawner_item = item;
		}
	}

	override void EEHitBy(TotalDamageResult damageResult, int damageType, EntityAI source, int component, string dmgZone, string ammo, vector modelPos, float speedCoef)
	{
		super.EEHitBy(damageResult, damageType, source, component, dmgZone, ammo, modelPos, speedCoef);
		
		if (m_NEORespawner_reporthit)
		{
			NEORespawner.ReportHit(this, source, damageResult, dmgZone, IsAlive());
		}
	}
	
	override void EEKilled(Object killer)
	{
		super.EEKilled(killer);
		
		if (m_NEORespawner_respawn)
		{
			m_NEORespawner_timer = NEORespawner.CreateAndStartRespawnTimer(this);
#ifdef NEORESPAWNERDEBUG
			Print("[NEORESPANWER] killed, started timer: " + m_NEORespawner_item);
#endif 
		}
	}
	
	override void EEDelete(EntityAI parent)
	{
		if (m_NEORespawner_respawn)
		{
			ObjectSpawnerHandler.SpawnObject(m_NEORespawner_item);
#ifdef NEORESPAWNERDEBUG
			Print("[NEORESPANWER] respawning: " + m_NEORespawner_item);
#endif 
		}
		
		super.EEDelete(parent);
	}
	
	// handle player pickup of spawned items	
	override void OnItemLocationChanged(EntityAI old_owner, EntityAI new_owner)
	{
		super.OnItemLocationChanged(old_owner, new_owner);
		if (m_NEORespawner_respawn)
		{
			if (!old_owner && new_owner)
			{
				// if it had no parent before and has one now, spawn a new one, detach this one from
				// respwawn
				if (m_NEORespawner_reporttake)
				{
					NEORespawner.ReportPickup(m_NEORespawner_item, new_owner);
				}
				
				// create replacement object
				ObjectSpawnerHandler.SpawnObject(m_NEORespawner_item);
				
				// keep a picked up item from being tracked anymore
				m_NEORespawner_respawn = false;
				
				m_NEORespawner_item = null;				
			}
		}
	}
}






