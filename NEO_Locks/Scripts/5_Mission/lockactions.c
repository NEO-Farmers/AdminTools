// Basically took:
// https://github.com/CFToolsGameLabs/dayz-examples/blob/master/dynamic_actions/action.c
//
// and updated it for our actions, and used object action example for unlock Execute method

// Place this file anywhere within your 5_Mission scoped script folder

#ifdef GAMELABS

class NEOLock_ShowLocks extends GameLabsContextAction 
{
    // world, player, vehicle
    void NEOLock_ShowLocks() 
	{
		// A unique actionCode as string, identifying your action in the internal GameLabs system
        // Convey to a standard "{YourModPrefix}_{Action}" eg. "TestMod_SomeAction"
        this.actionCode = "NEOLock_ShowLocks";

        // User-friendly name for your action which is being displayed in the CFCloud interface for users
        this.actionName = "Toggle show/hide locked locks";

        // Prefix your action with an icon in the CFCloud interface, leave empty to disable
        // Available icons: https://fontawesome.com/v5/search (type is always dualtone)
        this.actionIcon = "lock";

        // Colour code for your action, only override if the action you implement is destructive
        // Available codes: "success" (green), "danger" (red), "warning" (yellow)
        this.actionColour = "default";

        // Context in which this will be available from the interface (world, player or vehicle)
        this.actionContext = "world";
    }

    // Main function that will be executed
    override bool Execute(GameLabsActionContext context) 
	{
#ifdef NEOLOCK_DEBUG
		Print("Enter: NEOLock_ShowLocks.Execute()");
#endif				
		
		LockManager lm = GetLockManager();
		if (!lm)
		{
#ifdef NEOLOCK_INFO
			Print("Leave: NEOLock_ShowLocks.Execute()");
			Print("Failed to get LockManager singleton");
#endif						
			return false;
		}
	
		lm.ToggleShowLocks();
#ifdef NEOLOCK_DEBUG
		Print("Leave: NEOLock_ShowLocks.Execute()");
#endif			
		return true;
    }
}; // end NEOLock_ShowLocks

class NEOLock_UnlockLock extends GameLabsContextAction 
{
    // world, player, vehicle
    void NEOLock_UnlockLock() 
	{
		// A unique actionCode as string, identifying your action in the internal GameLabs system
        // Convey to a standard "{YourModPrefix}_{Action}" eg. "TestMod_SomeAction"
        this.actionCode = "NEOLock_UnlockLock";

        // User-friendly name for your action which is being displayed in the CFCloud interface for users
        this.actionName = "Unlock lock";

        // Prefix your action with an icon in the CFCloud interface, leave empty to disable
        // Available icons: https://fontawesome.com/v5/search (type is always dualtone)
        this.actionIcon = "lock-open";

        // Colour code for your action, only override if the action you implement is destructive
        // Available codes: "success" (green), "danger" (red), "warning" (yellow)
        this.actionColour = "default";

        // Context in which this will be available from the interface (world, player or vehicle)
        this.actionContext = "object";
    }

    override bool Execute(GameLabsActionContext context) 
	{
#ifdef NEOLOCK_DEBUG
		Print("Enter: NEOLock_UnlockLock.Execute()");
#endif		
        _Event eventInstance;
        _Event.CastTo(eventInstance, context.GetReferencedObject());

		if (!eventInstance)
		{
#ifdef NEOLOCK_INFO
			Print("Unable to get event instance object");
			Print("Leave: NEOLock_UnlockLock.Execute()");						
#endif			
			return false;
		}
		
#ifdef NEOLOCK_DEBUG
		Print("eventInstance.Ref() type: " + eventInstance.Ref().GetType());
#endif
		
		CombinationLock cb;
		CombinationLock.CastTo(cb, eventInstance.Ref());
		
		if (cb)
		{
			cb.Neo_Unlock();
		}
		else
		{
#ifdef NEOLOCK_INFO
			Print("Unable to get combination lock object from event instance object");
			Print("Leave: NEOLock_UnlockLock.Execute()");						
#endif			
			return false;
		}

#ifdef NEOLOCK_DEBUG
			Print("Leave: NEOLock_UnlockLock.Execute()");						
#endif					
        return true;
    }	
}; // end NEOLock_UnlockLock


// *************** TRY TO OVERRIDE GAMELABS DELETE HERE FOR LOCKS ************
//
// in testing, sending object delete to a lock that is locked on a fence 
// left the fence in a locked state with no way to remove wire etc. Try 
// and override it to unlock first then delete the lock. Normally locks don't
// show up on cftools, but now that we expose them to potential deletes, let's
// make it safe
//
// Update: deletes still left gates messed up... lock would look locked but would
// give take to hands prompt instead of turn dial when you walk up to gate
// so now just going to return false
modded class CFCloud_ObjectDelete extends GameLabsContextAction
{
    override bool Execute(GameLabsActionContext context) 
	{
#ifdef NEOLOCK_DEBUG
		Print("Enter: modded CFCloud_ObjectDelete.Execute()");
#endif		
		
        _Event eventInstance;
        _Event.CastTo(eventInstance, context.GetReferencedObject());

		if (!eventInstance)
		{
#ifdef NEOLOCK_INFO
			Print("Unable to get event instance object");
			Print("Leave: modded CFCloud_ObjectDelete.Execute()");						
#endif			
			return false;
		}
				
		CombinationLock cb;
		CombinationLock.CastTo(cb, eventInstance.Ref());		
		
		if (cb)
		{
			if (cb.IsLockedOnGate())
			{
#ifdef NEOLOCK_DEBUG
				Print("Caught delete of locked lock, returning error");
#endif			
				GetGameLabs().GetLogger().Warn(string.Format("[ObjectDelete] Can't safely delete locked lock, unlock instead: %1", eventInstance.Ref()));	
				return false;
			}		
		}
	
#ifdef NEOLOCK_DEBUG
		Print("Leave: modded CFCloud_ObjectDelete.Execute()");
#endif	
		
		return super.Execute(context);
	}		
}



#endif // GAMELABS