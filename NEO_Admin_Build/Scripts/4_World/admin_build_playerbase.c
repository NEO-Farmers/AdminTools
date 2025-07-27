modded class PlayerBase
{
	bool m_neo_admin_build_fast = false;
	bool m_neo_admin_build_full = false;
	bool m_neo_admin_build_enabled = false;

	void NEO_admin_build_clear()
	{
		m_neo_admin_build_fast = false;
		m_neo_admin_build_full = false;
		m_neo_admin_build_enabled = false;
	}
	
	void _NEO_admin_build_set_main_flag()
	{
		if (m_neo_admin_build_full || m_neo_admin_build_fast)
		{
			m_neo_admin_build_enabled = true;
		}
		else
		{
			m_neo_admin_build_enabled = false;
		}
	}
	
	void NEO_admin_build_SetFastBuild (bool fastbuildsetting)
	{
		NEO_admin_build_clear();
		m_neo_admin_build_fast = fastbuildsetting;
		_NEO_admin_build_set_main_flag();
	}
	
	void NEO_admin_build_SetFullBuild (bool fullbuildsetting)
	{
		NEO_admin_build_clear();
		m_neo_admin_build_full = fullbuildsetting;
		_NEO_admin_build_set_main_flag();
	}
	
	bool NEO_admin_build_GetFastBuild()
	{
		return m_neo_admin_build_fast;
	}
	
	bool NEO_admin_build_GetFullBuild()
	{
		return m_neo_admin_build_full;
	}
	
	bool NEO_admin_build_GetEnabled()
	{
		return m_neo_admin_build_enabled;
	}
}