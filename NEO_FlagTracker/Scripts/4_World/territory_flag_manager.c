
// **********************************************************
// This will maintain a list of territory flags and owners/builders
// and jsonserialize them to a file. It /should/ load that data at 
// launch and the territoryflag mods should be able to read that and
// apply it to the cftools updates. Storing to a file avoids causing 
// issues with the built-in OnStoreLoad/OnStoreSave we would encounter
// without a server wipe.


// ******************************************************************************
// Class for data storaage..
// making a pure data class for json serialization (not sure what happens to functions)
class NEOFlagInfo
{
	int version; // set to NEOFLAGINFO_VERSION
	string m_playerName;
	string m_playerID;
	string m_builderName;
	string m_builderID;
	vector position;  // we need this to match them up, don't know a unique item id we can use (yet)
	int lastupdate_year; // last time updated (in game-world time)
	int lastupdate_month; 
	int lastupdate_day;
	int lastupdate_hour;
	int lastupdate_minute;	
}


// ******************************************************************************
// static ref to singleton for territory flag manager
// Public interface access
private static ref TerritoryFlagManager g_territoryFlagManager;

static ref TerritoryFlagManager GetFlagManager() {
	if(!g_territoryFlagManager) {
		g_territoryFlagManager = new TerritoryFlagManager;
	}
#ifdef NEOFLAG_INFO
		if (!g_territoryFlagManager)
		{
			Print("Unable to create TerritoryFlagManager object");
		}
#endif	
	return g_territoryFlagManager;
};	


// *******************************************************************************
// TerritoryFlagManagerClass
// ***************************************************************************
class TerritoryFlagManager
{
	ref array<ref NEOFlagInfo> m_infoArray;
	
	// ***********************************************************************
	// constructor
	// *********************************************************************
	void TerritoryFlagManager()
	{
#ifdef NEOFLAG_DEBUG
		Print("Enter: TerritoryFlagManager constructor, should only happen once as a singleton");
#endif 		

		m_infoArray = new array<ref NEOFlagInfo>;
		if (!m_infoArray)
		{
#ifdef NEOFLAG_INFO
			Print("Failed to create flag data array");
#endif
			return;
		}
		this.LoadFlagInfo();
		
#ifdef NEOFLAG_DEBUG
		Print ("Leave: TerritoryFlagManager constructor");
#endif 		
	}
	
	
    // *******************************************************************************
	// destructor (probably never called since there's no release of strong ref above
	// ****************************************************************************
	void ~TerritoryFlagManager()
	{
	}

	
	// ************************************************************************
	// LoadFlagInfo
	// 
	// Loads flag data from json file in profiles (see NEOFLAG_FILENAME in defines.c)
	// ***************************************************************************
	bool LoadFlagInfo ()
	{
		
#ifdef NEOFLAG_DEBUG
		Print("Enter: LoadFlagInfo");
#endif

		if (!FileExist(NEOFLAG_FILENAME))
		{
#ifdef NEOFLAG_INFO
			Print("No NEOFlag data file found: " + NEOFLAG_FILENAME);
			Print("If you see this message on every start, something is wrong with storing flag info");
			Print("However, this is normal on first run or if data was cleared, continuing");		
			Print("Leave: LoadFlagInfo");
#endif			
			return true;
		}
		
		FileHandle file = OpenFile(NEOFLAG_FILENAME, FileMode.READ);
		
		if (file == 0)
		{
#ifdef NEOFLAG_INFO
			Print("Unable to open neoflag file for reading: " + NEOFLAG_FILENAME);
			Print("Leave: LoadFlagInfo");			
#endif
			return false;
		}
		
		string jsonstring = "";
		string tempstring = "";
		while (FGets(file, tempstring) > 0)
		{
			jsonstring = jsonstring + tempstring;
		}
		
		CloseFile(file);
		
#ifdef NEOFLAG_DEBUG
		Print("neo flag data json:");
		Print(jsonstring);
#endif 
		
		JsonSerializer js = new JsonSerializer;
		if (!js)
		{
#ifdef NEOFLAG_INFO
			Print("Unable to create jsonserializer");
			Print("Leave: LoadFlagInfo");			
#endif
			return false;
		}
		
		array<NEOFlagInfo> temparray = new array<NEOFlagInfo>;
		if (!temparray)
		{
#ifdef NEOFLAG_INFO
			Print("Unable to create temparray");
			Print("Leave: LoadFlagInfo");
#endif
			return false;
		}
		
		string errorstring;
		
		if (!js.ReadFromString(temparray, jsonstring, errorstring))
		{
#ifdef NEOFLAG_INFO
			Print("Error converting neoflag json to array:");
			Print(errorstring);
			Print("Leave: LoadFlagInfo");
#endif			
			return false;
		}
		
		if (!m_infoArray)
		{
#ifdef NEOFLAG_INFO
			Print("internal info array was null");
			Print("Leave: LoadFlagInfo");
#endif			
			return false;
		}
		
		m_infoArray.Clear();
		m_infoArray.InsertAll(temparray);
		
#ifdef NEOFLAG_DEBUG
		Print("Leave: LoadFlagInfo");
#endif
		return true;
	}

	
	// ************************************************************************
	// StoreFlagInfo
	// 
	// Stores flag data in json file in profiles (see NEOFLAG_FILENAME in defines.c)
	// ***********************************************************************
	bool StoreFlagInfo ()
	{		
#ifdef NEOFLAG_DEBUG
		Print("Enter: StoreFlagInfo");
#endif
		
		FileHandle file = OpenFile(NEOFLAG_FILENAME, FileMode.WRITE);
		
		if (file == 0)
		{
#ifdef NEOFLAG_INFO
			Print("Unable to open neoflag file for writing: " + NEOFLAG_FILENAME);
			Print("Leave: StoreFlagInfo");			
#endif	
			return false;
		}
		
		string jsonstring;
		
		JsonSerializer js = new JsonSerializer;
		if (!js)
		{
#ifdef NEOFLAG_INFO
			Print("Unable to create jsonserializer");
			Print("Leave: StoreFlagInfo");			
#endif
			return false;
		}		
		
		if (!m_infoArray)
		{
#ifdef NEOFLAG_INFO
			Print("internal info array was null");
			Print("Leave: StoreFlagInfo");
#endif			
			return false;
		}
		
		if (!js.WriteToString(m_infoArray, true, jsonstring))
		{
#ifdef NEOFLAG_INFO
			Print("Unable to json serialize flag info array");
			Print("Leave: StoreFlagInfo");
#endif	
			return false;
		}
		
		FPrint(file, jsonstring); // returns void, so gotta hope it worked
		CloseFile(file);
		
#ifdef NEOFLAG_DEBUG
		Print("Leave: StoreFlagInfo");
#endif

		return true;
	}


	// ****************************************************************************
	// AddFlagInfo
	//
	// Adds flag information, replacing anything that prior existed for that flag
	// ***************************************************************************
	void AddFlagInfo (NEOFlagInfo info)
	{
#ifdef NEOFLAG_DEBUG
		Print("Enter: AddFlagInfo");
#endif	
	
		int i;
		NEOFlagInfo temp;
		
		if (!info)
		{
#ifdef NEOFLAG_INFO
			Print("function was passed a null value");
			Print("Leave: AddFlagInfo");
#endif			
			return;
		}		
		
		if (!m_infoArray)
		{
#ifdef NEOFLAG_INFO
			Print("internal info array was null");
			Print("Leave: AddFlagInfo");
#endif			
			return;
		}
		
		for (i = 0; i < m_infoArray.Count(); i++)
		{
			temp = m_infoArray.Get(i);
			if (!temp)
			{
#ifdef NEOFLAG_INFO
				Print("internal info array held null value, skipping");
#endif				
				continue;
			}
			
			if (temp.position == info.position)
			{
#ifdef NEOFLAG_DEBUG
				Print("Removing old info for flag found at position: " + temp.position);
#endif			
				m_infoArray.Remove(i);
				continue; // would put a break here, but we want to kill dupes too
			}
		}
#ifdef NEOFLAG_DEBUG
		Print("Adding info for flag found at position: " + info.position);
#endif	
		// store updated time, we can use this to remove old flags at some point in the future
        GetGame().GetWorld().GetDate(info.lastupdate_year, info.lastupdate_month, info.lastupdate_day, info.lastupdate_hour, info.lastupdate_minute);	
		m_infoArray.Insert(info);
		
		this.StoreFlagInfo();

#ifdef NEOFLAG_DEBUG
		Print("Leave: AddFlagInfo");
#endif		

	}
	
	// *********************************************************************
	//GetFlagInfo
	//
	// Returns flag info for flag at position
	// If no flag found at that position it returns null
	// *********************************************************************
	NEOFlagInfo GetFlagInfo (vector position)
	{
		
#ifdef NEOFLAG_DEBUG
		Print("Enter: GetFlagInfo, position: " + position);
#endif	
	
		int i;
		NEOFlagInfo temp;
		
		if (!m_infoArray)
		{
#ifdef NEOFLAG_INFO
			Print("internal info array was null");
			Print("Leave: GetFlagInfo");
#endif			
			return null;
		}
		
		for (i = 0; i < m_infoArray.Count(); i++)
		{
			temp = m_infoArray.Get(i);
			if (temp.position == position)
			{
#ifdef NEOFLAG_DEBUG
				Print("Returning info for flag found at position: " + position);
				Print("Leave: GetFlagInfo");
#endif				
				return temp;
			}
		}
		
		// if not found, return null
#ifdef NEOFLAG_DEBUG
		Print("No flag found at position: " + position);
		Print("Leave: GetFlagInfo");
#endif		
		return null;			
	}
	
} // end TerritoryFlagManagerClass
	


