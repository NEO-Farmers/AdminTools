modded class SurvivorMissions
{
	ref array<Object> m_neo_smm_backup_mission_objects;

	
#ifdef GAMELABS
	Object m_neo_smm_gamelabs_ref_obj;
	vector m_neo_smm_prior_position;
	ref _Event m_neo_smm_registeredInstance;
#endif
	
	void SurvivorMissions()
	{
		GetGame().GetCallQueue(CALL_CATEGORY_SYSTEM).Call(this.NEOSMMUpdates);
	}
	
    void ~SurvivorMissions()
	{
		NEOSMMCleanup();
	}		

	
	void NEOSMMGameLabsUpdate()
	{
#ifdef GAMELABS
		if (m_MissionPosition == m_neo_smm_prior_position)
		{
			return;
		}
		
		if (m_neo_smm_registeredInstance)
		{
			GetGameLabs().RemoveEvent(m_neo_smm_registeredInstance);
			m_neo_smm_registeredInstance = null;
		}
		if (m_neo_smm_gamelabs_ref_obj)
		{
			m_neo_smm_gamelabs_ref_obj.Delete();
			m_neo_smm_gamelabs_ref_obj = null;
		}
		
		float y = GetGame().SurfaceY(m_MissionPosition[0], m_MissionPosition[2]);

		// stolen from object spawner
		int flags = ECE_SETUP | ECE_UPDATEPATHGRAPH | ECE_CREATEPHYSICS | ECE_NOLIFETIME | ECE_DYNAMIC_PERSISTENCY;
				
		vector position = m_MissionPosition;
		position[1] = y + 500; // put an object 500m in the sky so players don't see it
		
		m_neo_smm_gamelabs_ref_obj = GetGame().CreateObjectEx("SparkPlug", position, flags, RF_IGNORE);
		if (!m_neo_smm_gamelabs_ref_obj)
		{
			// uh, not much we can do
			return;
		}
			
		m_neo_smm_registeredInstance = new _Event("Mission", "map-marked", m_neo_smm_gamelabs_ref_obj, m_MissionType);
		if(!m_neo_smm_registeredInstance)
		{
			return;
		}
		GetGameLabs().RegisterEvent(m_neo_smm_registeredInstance);		

		m_neo_smm_prior_position = m_MissionPosition;
		
#endif // GAMELABS
	}

		
	void NEOSMMCleanup()
	{
		if (m_neo_smm_backup_mission_objects)
		{
			m_neo_smm_backup_mission_objects.Clear();
		}
		
#ifdef GAMELABS
		if (m_neo_smm_registeredInstance)
		{
			GetGameLabs().RemoveEvent(m_neo_smm_registeredInstance);
			m_neo_smm_registeredInstance = null;
		}
		if (m_neo_smm_gamelabs_ref_obj)
		{
			m_neo_smm_gamelabs_ref_obj.Delete();
			m_neo_smm_gamelabs_ref_obj = null;
		}
#endif // GAMELABS
	}
	
	override void MissionFinal()
	{
		super.MissionFinal();
		NEOSMMCleanup();
	}
	

	
	void NEOSMMUpdates()
	{
		NEOSMMGameLabsUpdate(); // will just return if no gamelabs
		
		if (!m_neo_smm_backup_mission_objects)
		{
			m_neo_smm_backup_mission_objects = new array<Object>;
		}
		
		bool arrays_different = false;
		
		if ((m_neo_smm_backup_mission_objects.Count()) != (m_MissionObjects.Count()))
		{
			arrays_different = true;
		}
		else
		{
			for (int i = 0; i < m_neo_smm_backup_mission_objects.Count(); ++i)
			{
				if (m_neo_smm_backup_mission_objects.Get(i) != m_MissionObjects.Get(i))
				{
					arrays_different = true;
				}
			}
		}
		if (!arrays_different)
		{
			return;
		}

		m_neo_smm_backup_mission_objects.Clear();
		m_neo_smm_backup_mission_objects.Copy(m_MissionObjects);
		
		ItemBase ib;
		foreach (Object o: m_neo_smm_backup_mission_objects)
		{
			ib = ItemBase.Cast(o);
			if(ib)
			{
				ib.m_neo_smm_isMissionObject = true;
			}
		}
	}
	
	override void ExtendMission()
	{
		super.ExtendMission();
		NEOSMMUpdates();		
	}
	
	override bool DeployMission()
	{
		bool result = super.DeployMission();
		if (result)
		{
			NEOSMMUpdates();
		}
		return result;
	}
	
}
