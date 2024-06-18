
// **********************************************************
// This will maintain a list of locked locks and allow their
// display to be turned on/off in cftools, it will also add 
// an action to cftools to toggle display and unlock.
// *************************************************************



// static ref to singleton for lock manager
private static ref LockManager g_lockManager;


// *************************************************************
// GetLockManager
//
// Public interface access, returns a singleton of the LockManager class
// or null if it fails
// ****************************************************************
static ref LockManager GetLockManager() { // ** does 'static' do anything here, it's not a class member
	if(!g_lockManager) {
		g_lockManager = new LockManager;
	}
#ifdef NEOLOCK_INFO
		if (!g_lockManager)
		{
			Print("Unable to create LockManager object");
		}
#endif	
	return g_lockManager;
};	


// *******************************************************************************
// LockManager Class
// ***************************************************************************
class LockManager
{
	// this time an array of weak references to lock objects so 
	// native engine can manage them.
	ref array<CombinationLock> m_lockArray;
	
	// flag to indicate if locks should so on the map or not
	bool m_showLocks = false;
	
	// ***********************************************************************
	// constructor
	// *********************************************************************
	void LockManager()
	{
#ifdef NEOLOCK_DEBUG
		Print("Enter: LockManager constructor, should only happen once as a singleton");
#endif 		

		m_lockArray = new array<CombinationLock>;
		
		if (!m_lockArray)
		{
#ifdef NEOLOCK_INFO
			Print("Failed to create combination lock array");
#endif
			return;
		}

#ifdef NEOLOCK_DEBUG
		Print ("Leave: LockManager constructor");
#endif 		
	}
	
	
    // *******************************************************************************
	// destructor (probably never called since there's no release of strong ref above
	// but let's clear the list just in case
	// ****************************************************************************
	void ~LockManager()
	{
#ifdef NEOLOCK_DEBUG
		Print("Enter: LockManager destructor, should only happen once as a singleton");
#endif 		
		if (m_lockArray)
		{
			m_lockArray.Clear();
		}
#ifdef NEOLOCK_DEBUG
		Print("Leave: LockManager destructor, should only happen once as a singleton");
#endif 				
	}
	

	// *******************************************************************************
	// GetShowLocks
	//
	// returns whether locks should be currently shown
	// ******************************************************************************
	bool GetShowLocks()
	{
#ifdef NEOLOCK_DEBUG
		Print("Enter: LockManager.GetShowLocks()");
		Print("Leave: LockManager.GetShowLocks()");
#endif 				
		return m_showLocks;
	}
	
	
	// ************************************************************************************
	// SetLockDisplay
	//
	// This function will iterate over its list of locked locks and call their functions that
	// display them on cftools if m_showLocks is true, otherwise it calls their functions that
	// removes their cftools event
	// ***********************************************************************************
	void SetLockDisplay()
	{
#ifdef NEOLOCK_DEBUG
		Print("Enter: LockManager.SetLockDisplay()");
#endif 						
		
		int i;
		CombinationLock lock;
		
		if (!m_lockArray)
		{
#ifdef NEOLOCK_INFO
			Print("Internal array was null");			
			Print("Leave: LockManager.SetLockDisplay()");
#endif			
			return;
		}
		
		for (i = 0; i < m_lockArray.Count(); i++)
		{
			lock = m_lockArray.Get(i);
			if (lock)
			{
				if (m_showLocks)
				{
					lock.Neo_ShowOnGameLabs();
				}
				else
				{
					lock.Neo_HideOnGameLabs();
				}
			}
		}
#ifdef NEOLOCK_DEBUG
		Print("Leave: LockManager.SetLockDisplay()");
#endif 		
	}
	
	
	// *******************************************************************************
	// ToggleShowLocks
	//
	// This function uses its internal status variable to either tell all locks to 
	// display or clear themselves on cftools
	// ******************************************************************************
	void ToggleShowLocks()
	{
#ifdef NEOLOCK_DEBUG
		Print("Enter: LockManager.ToggleShowLocks()");
#endif		
		if (m_showLocks)
		{
			m_showLocks = false;
		}
		else
		{
			m_showLocks = true;
		}
		SetLockDisplay();
#ifdef NEOLOCK_DEBUG
		Print("Leave: LockManager.ToggleShowLocks()");
#endif				
	}
	
	
	// ******************************************************************************
	// FindLock
	// 
	// This will look for the specified lock argument in the internal array and return
	// its index if found, otherwise it'll return -1
	// *****************************************************************************
	int FindLock(CombinationLock lock)
	{
#ifdef NEOLOCK_DEBUG
		Print("Enter: LockManager.FindLock()");
#endif				
		int i;
		
		if (!m_lockArray)
		{
#ifdef NEOLOCK_INFO
			Print("Internal array was null");			
			Print("Leave: LockManager.FindLock()");
#endif			
			return -1;
		}
		
		for (i = 0; i < m_lockArray.Count(); i++)
		{
			if (m_lockArray.Get(i) == lock)
			{
				return i;
			}
		}
		
		// not found
#ifdef NEOLOCK_DEBUG
		Print("Leave: LockManager.FindLock()");
#endif				
						
		return -1;
	}
	
	
	// ************************************************************************************
	// AddLock
	//
	// This function adds the specified lock to the inteernal array (it will avoid making 
	// duplicates). This should be called when a lock is locked
	// ************************************************************************************
	void AddLock(CombinationLock lock)
	{
#ifdef NEOLOCK_DEBUG
		Print("Enter: LockManager.AddLock()");
#endif				
		
		if (FindLock(lock) >= 0)
		{
			// already there
			return;
		}
		
		if (!m_lockArray)
		{
#ifdef NEOLOCK_INFO
			Print("Internal array was null");			
			Print("Leave: LockManager.AddLock()");
#endif						
			return;
		}
		
		m_lockArray.Insert(lock);
		
		if (m_showLocks) // if locks are supposed to show up add this new locked lock
		{
			lock.Neo_ShowOnGameLabs();
		}
		
#ifdef NEOLOCK_DEBUG
		Print("Leave: LockManager.AddLock()");
#endif						
	}
	
	
	// ***********************************************************************************
	// RemoveLock
	// 
	// This removes a lock from the internal array, if not found it will fail quietly (besides
	// maybe a log message). This should be called when a lock is unlocked or destroyed.
	// ************************************************************************************
	void RemoveLock(CombinationLock lock)
	{
#ifdef NEOLOCK_DEBUG
		Print("Enter: LockManager.RemoveLock()");
#endif		
		
		int index;
		
		if (!m_lockArray)
		{
#ifdef NEOLOCK_INFO
			Print("Internal array was null");			
			Print("Leave: LockManager.RemoveLock()");
#endif									
			return;
		}
		
		index = FindLock(lock);
		
		if (index < 0)
		{
#ifdef NEOLOCK_INFO
			Print("lock not found in remove lock, this should not happen");			
#endif												
		}
		
		// If it's being shownm, clear it since it should have been unlocked before 
		// calling this, call before removal in case we're the last reference to 
		// the object (doubt this could happen here, but better safe than sorry)
		if (m_showLocks)
		{
			lock.Neo_HideOnGameLabs(); // we only want to show locked locks
		}
		
		if (index >= 0)
		{
			m_lockArray.Remove(index);
		}
		
		
#ifdef NEOLOCK_DEBUG
		Print("Leave: LockManager.RemoveLock()");
#endif	
					
	}
} // end LockManagerClass
	


