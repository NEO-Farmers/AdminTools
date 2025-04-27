modded class EasterEgg
{
	Man m_neo_eggs_thrower; /* store the last player who's inventory this was in */
	
	Neo_Egg_Arena m_arena_manager;
	Neo_Egg_Prize m_prize_manager;
	
	/* prize egg info */
	bool m_neo_eggs_prize_egg;
	int m_neo_eggs_prize_index;
	
	/* arena egg info */
	bool m_neo_eggs_arena_egg;
	
	void EasterEgg()
	{
		m_neo_eggs_thrower = null;
		m_neo_eggs_prize_egg = false;
		m_neo_eggs_prize_index = 0;
		m_neo_eggs_arena_egg = false;
	}
	
	override void OnInventoryExit( Man player )
	{
		super.OnInventoryExit(player);
		Print("Neo eggs egg left inventory");
		Print("cap state:" + m_CaptureState);
		Print("type: " + m_CreatureType);
		
		m_neo_eggs_thrower = player; /* capture who's inventory it left so we know thrower */
	}
	
	
	override void OnItemLocationChanged(EntityAI old_owner, EntityAI new_owner) 
	{
		Man m;
		PlayerIdentity pi;
		float show_time;
		string title;
		string message;
		string icon;
		
		if ((old_owner == null) && (new_owner != null))
		{
			if (m_neo_eggs_prize_egg)
			{
				m = GetHierarchyRootPlayer();
				if (m)
				{
					pi = m.GetIdentity();
					if (pi)
					{
						show_time = 5.0;
						title = string.Format("What's in Egg number %1 ???", m_neo_eggs_prize_index);
						message = "One of these eggs holds a prize, the other two hold monsters. Only one way to find out. TRY YOUR LUCK!";
						icon = "set:dayz_gui image:tutorials";
						NotificationSystem.SendNotificationToPlayerIdentityExtended(pi, show_time, title, message, icon);
					}
				}
				if (m_prize_manager)
				{
					m_prize_manager.RemoveEgg(this); // egg was put in something, clear prize eggs
				}
			}
			else if (m_neo_eggs_arena_egg)
			{
				m = GetHierarchyRootPlayer();
				if (m)
				{
					pi = m.GetIdentity();
					if (pi)
					{
						show_time = 5.0;
						title = string.Format("Arena Egg", m_neo_eggs_prize_index);
						message = "No idea what's in here. Throw it in the arena and have someone else throw an egg in there too.";
						message = message + "\n... please don't abuse these, use them in the arena only";
						icon = "set:dayz_gui image:tutorials";
						NotificationSystem.SendNotificationToPlayerIdentityExtended(pi, show_time, title, message, icon);
					}
				}
				if (m_arena_manager)
				{
					m_arena_manager.RemoveEgg(this); // egg was grabbed, set egg to be replaced
				}
			}
		}
	}
	
	
	void Neo_Eggs_SetArenaEgg (bool arena_egg)
	{
		m_neo_eggs_arena_egg = arena_egg;
	}
	
	void Neo_Eggs_SetArenaManager (Neo_Egg_Arena nec)
	{
		m_arena_manager = nec;
	}
	
	void Neo_Eggs_SetPrizeEgg (bool prize_egg)
	{
		m_neo_eggs_prize_egg = prize_egg;
		Print("Neo Egg is Prize Egg");
	}
	
	void Neo_Eggs_SetPrizeIndex (int prize_index)
	{
		m_neo_eggs_prize_index = prize_index;
		Print("Neo Egg is prize index" + prize_index);
	}
	
	void Neo_Eggs_SetPrizeManager (Neo_Egg_Prize manager)
	{
		m_prize_manager = manager;
	}
	
	
	void Neo_Eggs_LoadEgg(string loadtype)
	{
		string temptype = loadtype;
		temptype.ToLower();
		Print ("Neo Eggs Loaded egg with: " + loadtype);
		m_CreatureHash = loadtype.Hash();
		m_CreatureType = loadtype;
		m_CaptureState = eCaptureState.STASIS;
		m_DangerSound = true;
		
		
		if (temptype.IndexOf("animal_canislupus") == 0)
		{
			m_CaptureSoundHash = "WolfWhimper_SoundSet".Hash();
			m_ReleaseSoundHash = "WolfBark_SoundSet".Hash();
		}
		else if (temptype == ("animal_ursusarctos"))
		{
			m_CaptureSoundHash = "BearSnarl_SoundSet".Hash();
			m_ReleaseSoundHash = "BearRoarShort_SoundSet".Hash();
		}
		else if (temptype.IndexOf("zmbf") == 0)
		{
			m_CaptureSoundHash = "ZmbF_Normal_HeavyHit_Soundset".Hash();
			m_ReleaseSoundHash = "ZmbF_Normal_CallToArmsShort_Soundset".Hash();
		}
		else if (temptype.IndexOf("zmbm") == 0)
		{
			m_CaptureSoundHash = "ZmbM_Normal_HeavyHit_Soundset".Hash();
			m_ReleaseSoundHash = "ZmbM_Normal_CallToArmsShort_Soundset".Hash();
		}
		else
		{
			m_CaptureSoundHash = "CattleBellow_SoundSet".Hash();
			m_ReleaseSoundHash = "CattleMooA_SoundSet".Hash();
		}
		
		m_DangerSound = true;
		SetQuantity( GetQuantityMax() );
		SetSynchDirty();
	}
	
	// have to replace release to add properties to the released creature/item
	override private void Release( vector pos )
	{
		if ( GetGame().IsServer() )
		{
			Print("Neo Eggs releasing egg: " + m_CreatureType);
			Object o;
			PlayerIdentity pi;
			float show_time;
			string title;
			string message;
			string icon;
			bool add_debug_spawn = false;
			
			
			m_CaptureState = eCaptureState.RELEASEFX;
			m_CreatureHash = 0;
			SetSynchDirty();
			
			if (GetGame().IsKindOf(m_CreatureType, "DZ_LightAI"))
			{
				o = GetGame().CreateObjectEx(m_CreatureType, pos, ECE_INITAI | ECE_CREATEPHYSICS);
			}
			else
			{
				o = GetGame().CreateObjectEx(m_CreatureType, pos, ECE_SETUP | ECE_KEEPHEIGHT | ECE_PLACE_ON_SURFACE);
				add_debug_spawn = true;
			}
			
			if (!o)
			{
				if ((m_neo_eggs_thrower) && (m_neo_eggs_prize_egg || m_neo_eggs_arena_egg))
				{
					pi = m_neo_eggs_thrower.GetIdentity();
					if (pi)
					{
						show_time = 5.0;
						title = "Item failed";
						message = string.Format("Failed to spawn: %1", m_CreatureType);
						icon = "set:dayz_gui image:tutorials";
						NotificationSystem.SendNotificationToPlayerIdentityExtended(pi, show_time, title, message, icon);
					}
				}
			}
			else
			{				
				if ((m_neo_eggs_thrower) && (m_neo_eggs_prize_egg))
				{
					pi = m_neo_eggs_thrower.GetIdentity();
					if (pi)
					{
						show_time = 2.0;
						title = "Congratulations!";
						message = string.Format("You got a(n): %1", o.GetDisplayName());
						icon = "set:dayz_gui image:tutorials";
						NotificationSystem.SendNotificationToPlayerIdentityExtended(pi, show_time, title, message, icon);
					}
				}
			}
			
			m_CreatureType = "";
			if(m_neo_eggs_arena_egg || m_neo_eggs_prize_egg)
			{
				DecreaseHealth("","",GetMaxHealth());
			}
			else
			{
				DecreaseHealth( "", "", GetMaxHealth() * 0.4 );
			}
			SetQuantity( GetQuantityMin(), false );
			SetVelocity( this, CAPTURE_VELOCITY );
			
			if ( !IsAlive() )
			{
				if ( m_ParCapture )
					m_ParCapture.Delete();
				Delete();
			}
			
			EntityAI e = EntityAI.Cast(o);
			if (e)
			{
				if (add_debug_spawn)
				{
					e.OnDebugSpawn();
				}
				
				if (e.IsAnimal())
				{
					AnimalBase ab = AnimalBase.Cast(e);
					if(ab)
					{
						ab.Neo_Eggs_setIsFromEgg(true);
						ab.Neo_Eggs_setIsPrize(m_neo_eggs_prize_egg);
						ab.Neo_Eggs_setThrower(m_neo_eggs_thrower);
						ab.Neo_Eggs_setInArena(Neo_Egg_Arena.IsInArena(ab.GetPosition()));
					}
				}
				if (e.IsZombie())
				{
					ZombieBase zb = ZombieBase.Cast(e);
					if (zb)
					{
						zb.Neo_Eggs_setIsFromEgg(true);
						zb.Neo_Eggs_setIsPrize(m_neo_eggs_prize_egg);
						zb.Neo_Eggs_setThrower(m_neo_eggs_thrower);
						zb.Neo_Eggs_setInArena(Neo_Egg_Arena.IsInArena(zb.GetPosition()));
					}
				}
			}
		}
	}
};