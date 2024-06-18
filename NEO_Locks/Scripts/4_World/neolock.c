// This is a modded CombinationLock class to facilitate 
// dynamic display on cftools and unlocks from cftools

modded class CombinationLock extends ItemBase
{
	// cf tools event
	ref _Event _registeredInstance;
	
	
	// *******************************************************************
	// constructor
	// do nothing for now
	// ******************************************************************
	void CombinationLock()
	{
	}
	
	
	// *******************************************************************
	// destructor
	// calls method to hide on cftools before clearing off (that method should
	// catch a null in _registeredInstance to avoid doubling or other issues
	// ********************************************************************
	void ~CombinationLock()
	{
		Neo_HideOnGameLabs();
	}
	
	// who locked it
	string m_lockerName = "(locked since restart)";
	string m_lockerID = "unknown";
	

	// ****************************************************************************
	// Neo_ShowOnGameLabs
	//
	// This will cause the CombinationLock object to post an event to cftools
	// ***************************************************************************	
	void Neo_ShowOnGameLabs()
	{
#ifdef NEOLOCK_DEBUG
		Print("Enter: CombinationLock.Neo_ShowOnGameLabs()");		
#endif
		
#ifdef GAMELABS		
		
		string combination_text = m_CombinationLocked.ToString();
		
		// stole the zero digit addition from bohemia's base class
		//insert zeros to dials with 0 value
		int length_diff = m_LockDigits - combination_text.Length();
		for ( int i = 0; i < length_diff; ++i )
		{
			combination_text = "0" + combination_text;
		}
		
		this._registeredInstance = new _Event("<b>Combo Lock</b><br/>Combo: " + combination_text + "<br/>Locked by: " + m_lockerName + " (" + m_lockerID + ")", "lock", this);
		if (this._registeredInstance)
		{
			if (GetGameLabs())
			{
				if (GetGameLabs().IsServer())
				{
					GetGameLabs().RegisterEvent(this._registeredInstance);
				}
			}
		}		
#endif // GAMELABS
		
#ifdef NEOLOCK_DEBUG
		Print("Leave: CombinationLock.Neo_ShowOnGameLabs()");		
#endif				
	}
	
	
	// ***********************************************************************
	// Neo_HideOnGameLabs
	//
	// This method will cause the object to remove its event instance from gamelabs
	// if that instance is non-null
	// *****************************************************************************
	void Neo_HideOnGameLabs()
	{
#ifdef NEOLOCK_DEBUG
		Print("Enter: CombinationLock.Neo_HideOnGameLabs()");		
#endif		
		
#ifdef GAMELABS		
		if(!this._registeredInstance)
		{
#ifdef NEOLOCK_DEBUG
			Print("_registeredInstance was null"); // normal if not visible on cftools so debug not info
			Print("Leave: CombinationLock.Neo_HideOnGameLabs()");		
#endif					
			return;
		}
        if(GetGameLabs()) 
		{
            if(GetGameLabs().IsServer()) 
			{
                GetGameLabs().RemoveEvent(this._registeredInstance);
            }
        }
#endif // GAMELABS

#ifdef NEOLOCK_DEBUG
			Print("Leave: CombinationLock.Neo_HideOnGameLabs()");		
#endif							
	}
	
	
	// ***********************************************************************
	// Neo_Unlock
	// 
	// This method should allow for unlocking of a locked lock from a cftools
	// command or an in-game chat command (future work)
	// ***********************************************************************
	void Neo_Unlock()
	{
#ifdef NEOLOCK_DEBUG
		Print("Enter: CombinationLock.Neo_Unlock()");		
#endif							
		
		
		// Setting the combo to the locked combo, executing the check, then mimicing behavior in bohemia's:
		// scripts/4_world/classes/useractionscomponent/actions/continuous/actiondialcombinationlockontarget.c
		// function OnFinishProgressServer
		if (IsLockedOnGate())
		{
			SetCombination(m_CombinationLocked);
			CheckLockedStateServer();
			UnlockServer(null, GetHierarchyParent());
		}
		
#ifdef NEOLOCK_DEBUG
		Print("Leave: CombinationLock.Neo_Unlock()");		
#endif									
	}
		
	
	// ***********************************************************************
	// OnItemLocationChanged
	//
	// This should be the function called when a lock is attached to a gate and locked
	// this should allow us to add a freshly locked lock to our list in the LockManager
	//
	// old_owner is hopefully the player and new_owner appears to be the fence here
	// 
	// **Update: this is called at other times as well and old_owner is sometimes
	// the lock itself, so call super and check for locked on gate before doing
	// anything else
	// **************************************************************************
	override void OnItemLocationChanged( EntityAI old_owner, EntityAI new_owner ) 
	{
#ifdef NEOLOCK_DEBUG	
		Print("Enter: CombinationLock.OnItemLocationChanged()");
		if (old_owner)
		{
			Print("old_owner type: " + old_owner.GetType());
		}
		if (new_owner)
		{
			Print("new_owner type: " + new_owner.GetType());
		}
#endif			

		super.OnItemLocationChanged( old_owner, new_owner );

		// both old and new owners have to be non-null 
        if (!old_owner)
		{
			return;
		}
		
		if (!new_owner)
		{
			return;
		}

		// new owner must be a fence for a lock attach (base class checks for inheritance from building base)
		if (!new_owner.IsInherited( BaseBuildingBase ))
		{
#ifdef NEOLOCK_DEBUG 
			Print("new owner is not a fence, not a lock action");
#endif			
			return;
		}
		
		// old owner must be a player for a lock attach
		Man player;
		Man.CastTo(player, old_owner);
		
		if (!player)
		{
#ifdef NEOLOCK_DEBUG 
			Print("player was null, can't set to lock");
#endif			
			return;
		}
		
		PlayerIdentity id = player.GetIdentity();
		if (!id)
		{
#ifdef NEOLOCK_DEBUG 
			Print("player.GetIdentity() returned null, can't set to lock");
#endif			
			return;
		}
		
		this.m_lockerName = id.GetName();
		this.m_lockerID = id.GetPlainId();
#ifdef NEOLOCK_INFO
		Print("Lock at position: " + GetPosition() + " locked by: " + this.m_lockerName + " ("+ this.m_lockerID + ") combo: " + m_CombinationLocked);
#endif						
		
		
#ifdef NEOLOCK_DEBUG	
		Print("Leave: CombinationLock.OnItemLocationChanged()");
#endif			
		
	}

	
	// ***********************************************************************
	// LockServer
	// catch lock events (should catch AfterStoreLoad as well, since it calls this)
	// ***********************************************************************
	override void LockServer( EntityAI parent, bool ignore_combination = false )
	{	
#ifdef NEOLOCK_DEBUG
		Print("Enter: CombinationLock.LockServer()");		
#endif	
		// call super										
		super.LockServer(parent, ignore_combination);
			
		LockManager lm;
		lm = GetLockManager();
		if(!lm)
		{
#ifdef NEOLOCK_INFO
			Print("Unabled to get LockManager singleton");
 			Print("Leave: CombinationLock.LockServer()");		
#endif										
			
			return;
		}
		
		lm.AddLock(this);
		
#ifdef NEOLOCK_DEBUG
		Print("Leave: CombinationLock.LockServer()");		
#endif																
	}
		
	
	
	// ************************************************************************
	// UnlockServer
	// catch unlock
	// ActionDestroyCombinationLock calls this as well, so it *should* catch lock
	// destruction as well
	// *******************************************************************************
	override void UnlockServer( EntityAI player, EntityAI parent )	
	{
#ifdef NEOLOCK_DEBUG
		Print("Enter: CombinationLock.UnlockServer()");
#endif				
		
		super.UnlockServer(player, parent);
		
		LockManager lm;
		lm = GetLockManager();
		if(!lm)
		{
			return;
		}
		
		lm.RemoveLock(this); // The manager should trigger the clear on gamelabs step
		
#ifdef NEOLOCK_DEBUG
		Print("Leave: CombinationLock.UnlockServer()");
#endif				
	}

		
} // end CombinationLock modded class

