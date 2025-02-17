#ifdef SERVER
#ifdef GAMELABS

modded class TerritoryFlag
{
	// Newer version of this mod just overrides the base CFTools string
	// generation and stores multiple flag raisers using native serializer
	// and persistent object ID based file names
	// see changes to CFTools flags here:
	// https://github.com/CFToolsGameLabs/dayz-examples/commit/e0daff605e366c6e42b7443c7e1f68c81746b3e6
	//
	// Going to attempt to save all flag raisers using a method taken from OnStoreSave/OnStoreLoad in:
	// scripts/4_world/classes/playerstomach.c
	
	
	ref TStringIntMap	m_NEOF_flag_raisers; // map of string to int string is "name (steamid)", int is number of times flag is raised
	bool				m_NEOF_flag_raisers_changed; // flag to indicate if list of raisers needs to be saved
	
	
	void TerritoryFlag () 
	{
		m_NEOF_flag_raisers = new TStringIntMap;
		if (!FileExist(NEOFLAG_DATA_DIRECTORY))
		{
			MakeDirectory(NEOFLAG_DATA_DIRECTORY);
		}
		m_NEOF_flag_raisers_changed = false;
	}

	void ~TerritoryFlag ()
	{
		if (m_NEOF_flag_raisers)
		{
			delete m_NEOF_flag_raisers;
		}
	}
	
	// **************************************************************************
	// NEOF_Flag_GetFilename
	// Get a unique file name for this object
	// **************************************************************************
	string NEOF_Flag_GetFilename()
	{
		int b1;
		int b2;
		int b3;
		int b4;
		string filename;
		GetPersistentID(b1, b2, b3, b4);
		
		filename = string.FORMAT("%1flag_%2_%3_%4_%5", NEOFLAG_DATA_DIRECTORY, b1, b2, b3, b4);
		return filename;
	}
	
	// ***************************************************************************
	// NEOF_Flag_SetPlayer
	// This function will set the internal player name and steam id values and we
	// expect it to be called from the modded ActionRaiseFlag.OnFinishProgressServer()
	// function. 
	//
	// We used to check flag state here, but it was not actually updated by the
	// call to totem.AnimateFlagEx in the action function. So now this function will
	// set the player when called and it will post an CFTools update with the new info.
	//
	// param: PlayerBase player - the PlayerBase object from the action's 'action_data' argument
	// ***********************************************************************
	void NEOF_Flag_SetPlayer(PlayerBase player)
	{
		NEOF_flag_debug("Enter: NEOF_Flag_SetPlayer");
		
		if (!player)
		{
			NEOF_flag_debug("player was null, can't set to flag");
			NEOF_flag_debug("Leave: NEOF_Flag_SetPlayer");
			return;
		}
		
		PlayerIdentity id = player.GetIdentity();
		if (!id)
		{
			NEOF_flag_debug("player.GetIdentity() returned null, can't set to flag");
			NEOF_flag_debug("Leave: NEOF_Flag_SetPlayer");
			return;
		}
		
		// this should return strings and not a potential null pointer
		string playerName = id.GetName();
		string playerID = id.GetPlainId();
		
		string raiserString = string.Format("%1 (%2)", playerName, playerID);
		NEOF_flag_debug("Adding flag raiser: " + raiserString);
		
		int raise_count = 0;
		if (m_NEOF_flag_raisers)
		{
			if(m_NEOF_flag_raisers.Find(raiserString, raise_count))
			{
				++raise_count;
				m_NEOF_flag_raisers.Set(raiserString, raise_count);
			}
			else
			{
				raise_count = 1; // first time this player raised flag
				m_NEOF_flag_raisers.Insert(raiserString, raise_count);
			}
			m_NEOF_flag_raisers_changed = true;
			_updateEvent(); // internal function from cftools example on github
		}
		else
		{
			NEOF_flag_debug("Raiser mapping, not allocated");
		}
		
		NEOF_flag_debug("Leave: NEOF_Flag_SetPlayer");
	}
	
	
	// ***************************************************************************
	// GL_GetDisplayName
	// This is a modified version of the CFTools examples function from github
	// that allows us to override the html that populates the pop-up on the map
	// in cftools
	// ***************************************************************************
	override private string GL_GetDisplayName() 
	{
		float remainingLifetime = GetLifetime() / 3600;
		string steam64 = GL_GetSteam64();
		string displayName;
		
		TStringArray top_raisers = {};
		if (m_NEOF_flag_raisers)
		{
			TStringArray key_array = m_NEOF_flag_raisers.GetKeyArray();
			TIntArray val_array = m_NEOF_flag_raisers.GetValueArray();
			val_array.Sort(true); // sort larger numbers to lower
			
			count = val_array.Count();
			if (count > NEOFLAG_RAISER_COUNT)
			{
				count = NEOFLAG_RAISER_COUNT;
			}
			int i;
			for (i = 0; i < count; ++i)
			{
				int val = val_array.Get(i);
				foreach(string name: key_array) 
				{
					if (m_NEOF_flag_raisers.Get(name) == val)
					{
						top_raisers.Insert(name);
						key_array.RemoveItem(name);
						break;
					}
				}
			}
			delete key_array;
			delete val_array;
		}
		if (top_raisers.Count() > 0)
		{
			displayName = string.Format("<b>Territory Flag</b><br/>Flag Level: %1 %%<br/>Remaining Lifetime: ~ %2 hours<br/>", Math.Round(GetRefresherTime01() * 100), Math.Round(remainingLifetime));
			raisers_string = "";
			foreach(string name: top_raisers)
			{
				// cull any < > & " ' characters from name that can break html 
				string cleaned_name;
				cleaned_name = name;
				cleaned_name.Replace("&","&amp;");
				cleaned_name.Replace("<","&lt;");
				cleaned_name.Replace(">","&gt;");
				cleaned_name.Replace("\"","&quot;");
				cleaned_name.Replace("'","&apos;");
				raisers_string = raisers_string + string.Format("<li>%1</li>", cleaned_name);
			}
			
			displayName = displayName + string.Format("Primary Flag Raisers:<ul>%1</ul>", raisers_string);
		}
		else // default gamelabs behavior
		{
			if(steam64) {
				displayName = string.Format("<b>Territory Flag</b><br/>Flag Level: %1 %%<br/>Remaining Lifetime: ~ %2 hours<br/>Owner: %3", Math.Round(GetRefresherTime01() * 100), Math.Round(remainingLifetime), steam64);
			} else {
				displayName = string.Format("<b>Territory Flag</b><br/>Flag Level: %1 %%<br/>Remaining Lifetime: ~ %2 hours", Math.Round(GetRefresherTime01() * 100), Math.Round(remainingLifetime));
			}
		}
		delete top_raisers;
		return displayName;
	}
	

	// *************************************************************************
	// OnStoreSave
	// If the flag raiser mapping is updated, then update our saved information
	// *************************************************************************
	override void OnStoreSave(ParamsWriteContext ctx)
	{
		NEOF_flag_debug("Enter: OnStoreSave");
		
		super.OnStoreSave(ctx);
		if (!m_NEOF_flag_raisers_changed)
		{
			NEOF_flag_debug("Flag raisers unchanged");
			NEOF_flag_debug("Leave: OnStoreSave");
			return;
		}
		
		if (!m_NEOF_flag_raisers)
		{
			NEOF_flag_debug("Flag raisers mapping uninitialized");
			NEOF_flag_debug("Leave: OnStoreSave");
			return;
		}
		
		string filename = NEOF_Flag_GetFilename();
		
		NEOF_flag_debug("Saving raiser data in " + filename);
		autoptr TStringIntMap temp_map = new TStringIntMap;
		temp_map.Copy(m_NEOF_flag_raisers); // get a non-changing mapping just in caes
		FileSerializer file = new FileSerializer();
		if (file.Open(filename, FileMode.WRITE))
		{
			autoptr TStringArray key_array = temp_map.GetKeyArray();
			int count = key_array.Count();
			if (file.Write(count))
			{
				foreach(string name: key_array)
				{
					file.Write(name);
					file.Write(temp_map.Get(name));
				}
			}
			
			file.Close();
		}
		else
		{
			NEOF_flag_debug("Unable to open file to save");
		}
		m_NEOF_flag_raisers_changed = false;
		NEOF_flag_debug("Leave: OnStoreSave");
	}

	// *************************************************************************
	// AfterStoreLoad
	// This should get called on the after loading data from storage
	//
	// This will grab flag info from the territoryflagmanager and if it is 
	// available, update internal variables and post an update to gamelabs.
	// **************************************************************************
	override void AfterStoreLoad()
	{
		NEOF_flag_debug("Enter: AfterStoreLoad");

		super.AfterStoreLoad();

		string filename = NEOF_Flag_GetFilename();
		
		if (!FileExist(filename))
		{
			NEOF_flag_debug("no flag raiser data file");
		}
		
		autoptr TStringIntMap temp_map = new TStringIntMap;
		bool successful_read = true;

		FileSerializer file = new FileSerializer();
		if (file.Open(filename, FileMode.READ))
		{
			int count;
			if (file.Read(count))
			{
				int i;
				for (i = 0; i < count; ++i)
				{
					string name;
					int raised;
					successful_read = file.Read(name);
					if (!successful_read)
					{
						break;
					}
					successful_read = file.Read(raised);
					if (!successful_read)
					{
						break;
					}
					temp_map.Insert(name, raised);
				}
			}
			else
			{
				successful_read = false;
			}
			
			file.Close();
		}
		else 
		{
			NEOF_flag_debug("Failed to open flag data flie");
			successful_read = false;
		}
		
		if (successful_read)
		{
			m_NEOF_flag_raisers.Clear();
			m_NEOF_flag_raisers.Copy(temp_map);
		}
		else
		{
			NEOF_flag_debug("Failed to successfully read flag raiser data");
		}
		NEOF_flag_debug("Leave: AfterStoreLoad");
	}
	
	// **************************************************************************
	// Delete
	// Catch object delete here to delete data file that is no longer needed
	// **************************************************************************
	override void Delete()
	{
		NEOF_flag_debug("Enter: Delete");
		string filename = NEOF_Flag_GetFilename();
		
		if (FileExist(filename))
		{
			NEOF_flag_debug("Deleting " + filename);
			DeleteFile(filename);
		}
		
		super.Delete(); // do this at the end so persistent ID exists for filename
		
		NEOF_flag_debug("Leave: Delete");
	}
	// ***************************************************************************
	//LogAnimateFlag
	//
	// Hook function here for easier access to when flag is raised than hooking action
	// 'delta' value has already been normalized where we can test for 0 by prior
	// function calls and player param can not be null
	
	override protected void LogAnimateFlag(float newPhase, notnull PlayerBase player)	
	{
		NEOF_flag_debug("Enter: LogAnimateFlag");
		
		super.LogAnimateFlag(newPhase, player);
		
		if (newPhase == 0) // 0 == flag raised all the way
	    {
			this.NEOF_Flag_SetPlayer(player);
		}
		
		NEOF_flag_debug("Leave: LogAnimateFlag");
	}
	

	// *******************************************************************
	// NEOF_flag_debug
	// Debug print function, does nothing if NEOFLAG_DEBUG isn't set,
	// results end up in the script log in the profile directory
	// *******************************************************************
	void NEOF_flag_debug(string message)
	{
#ifdef NEOFLAG_DEBUG 
		int hour;
		int minute;
		int second;
		int year;
		int month;
		int day;
		
		GetYearMonthDay(year, month, day);
		GetHourMinuteSecond(hour, minute, second);
		formatted_message = string.Format("%1:%2:%3 %4:%5:%6 (NEOFLAGDEBUG): %7", year, month, day, hour, minute, second, message);
		Print(formatted_message);
#endif
	}
	
}; // end modded TerritoryFlag class

#endif // GAMELABS
#endif //SERVER

