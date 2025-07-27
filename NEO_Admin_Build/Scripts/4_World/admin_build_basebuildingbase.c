modded class BaseBuildingBase
{
	static BaseBuildingBase m_neo_admin_build_latest_structure;
	
	void BaseBuildingBase()
	{
		m_neo_admin_build_latest_structure = this;
	}
	
	static BaseBuildingBase NEO_AdminBuildGetLatestStructure()
	{
		return m_neo_admin_build_latest_structure;
	}
	
	void NEO_admin_build_readynext()
	{
		if (!m_Construction)
		{
			Print("[NEO_ADMIN_BUILD] BaseBuildingBase::NEO_admin_build_readynext m_Construction was null");
			return;
		}
		
		m_Construction.NEO_admin_build_readynext();
	}
	
	void FullyBuild(PlayerBase player)
	{
		array<string> excludes = OnDebugSpawnBuildExcludes();
		array<ConstructionPart> parts = GetConstruction().GetConstructionParts().GetValueArray();
		
		Man p = player;
				
		foreach (ConstructionPart part : parts)
		{
			bool excluded = false;
			string partName = part.GetPartName();
			if (excludes)
			{
				foreach (string exclude : excludes)
				{
					if (partName.Contains(exclude))
					{
						excluded = true;
						break;
					}
				}
			}
			
			if (!excluded)
			{
				OnPartBuiltServer(p, partName, AT_BUILD_PART);
			}
		}
		
		GetConstruction().UpdateVisuals();
	}

}