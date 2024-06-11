modded class TerritoryFlag extends BaseBuildingBase {
	
	// Name and SteamID of the latest player to fully raise flag
	string m_playerName = "Unknown"; 
	string m_playerID = "Unknown"; 
	
	// Name and SteamID of the player who placed the flagpole kit 
	// NOTE: any flagpoles built before mod install won't have this.
	string m_builderName = "Unknown";
	string m_builderID = "Unknown";


	// *****************************************************************************
	// Constructor, mostly copied from CFTools example, you can see frmo their 
	// comment, it needs deferred execution. This may cause a race condition with
	// OnStoreLoad, hence the update call in AfterStoreLoad (basicterritories uses 
	// this method as well).
	// *****************************************************************************
	void TerritoryFlag () 
	{
		// MODDED *****************************************
		// CFTools base mod is going to post its own event with this._registeredInstance,
		// we won't conflict with that since our 'update' removes that object, don't want
		// that to happen and have a possible null reference.
	}


	// ***************************************************************************
	// _UpdateFlagManager
	//
	// This should get called any time player or builder is updated to update
	// the flag manager's information
	// ***************************************************************************
	void _UpdateFlagManager()
	{
#ifdef NEOFLAG_DEBUG 
		Print("Enter: _UpdateFlagManager");
#endif
		TerritoryFlagManager tfm;
		NEOFlagInfo nfi;

		nfi = new NEOFlagInfo;
		if (!nfi)
		{
#ifdef NEOFLAG_DEBUG 
			Print("couldn't create new neoflaginfo");
			Print("Leave: _UpdateFlagManager");
#endif
			return;
		}
		
		tfm = GetFlagManager();
		if (!tfm)
		{
#ifdef NEOFLAG_DEBUG 
			Print("couldn't get flag manager");
			Print("Leave: _UpdateFlagManager");
#endif
			return;
		}
		
		nfi.m_playerName = m_playerName;
		nfi.m_playerID = m_playerID;
		nfi.m_builderName = m_builderName;
		nfi.m_builderID = m_builderID;
		nfi.position = this.GetPosition();
		nfi.version = NEOFLAGINFO_VERSION;
		
		tfm.AddFlagInfo(nfi);
		
#ifdef NEOFLAG_DEBUG 
		Print("Leave: _UpdateFlagManager");
#endif
	}
	
	
	// *****************************************************************************
	// SetBuilder
	//
	// This function will set the internal builder name and steam id values and we
	// expect it to be called from the modded TerritoryFlagKit.OnPlacementComplete()
	// function. 
	//
	// This should be getting called on successful placement and builder should be 
	// updated unconditionally. Should only happen once in the objects lifetime.
	//
	// param: Man player - the Man object from the OnPlacementComplete's 'man' argument
	// ** NOTE THIS IS NOT a PlayerBase object, but still has a GetIdentity function
	// *****************************************************************************
	void SetBuilder(Man player)
	{
#ifdef NEOFLAG_DEBUG 
		Print("Enter: SetBuilder");
#endif

		if (!player)
		{
#ifdef NEOFLAG_DEBUG 
			Print("player was null, can't set builder for flag");
			Print("Leave: SetBuilder");
#endif
			return;
		}

		PlayerIdentity id;
		id  = player.GetIdentity();
		if (!id)
		{
#ifdef NEOFLAG_DEBUG 
			Print("id was null, can't set to flag");
			Print("Leave: SetBuilder");
#endif
			return;
		}
		
		// this should return strings and not a potential null pointer
		this.m_builderName = id.GetName();
		this.m_builderID = id.GetPlainId();

#ifdef NEOFLAG_DEBUG
		Print("Settting builder to: " + this.m_builderName + " (" + this.m_builderID + ")");
#endif
		
		this._UpdateFlagManager();
		
		GetGame().GetCallQueue(CALL_CATEGORY_SYSTEM).Call(this._UpdateGameLabs);
#ifdef NEOFLAG_DEBUG
		Print("Leave: SetBuilder");
#endif
	}
	
	// ***************************************************************************
	// SetPlayer
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
	void SetPlayer(PlayerBase player)
	{
#ifdef NEOFLAG_DEBUG 
		Print("Enter: SetPlayer");
#endif

		if (!player)
		{
#ifdef NEOFLAG_DEBUG 
			Print("player was null, can't set to flag");
			Print("Leave: SetPlayer");
#endif
			return;
		}
		
		PlayerIdentity id = player.GetIdentity();
		if (!id)
		{
#ifdef NEOFLAG_DEBUG 
			Print("player.GetIdentity() returned null, can't set to flag");
			Print("Leave: SetPlayer");
#endif
			return;
		}
		// this should return strings and not a potential null pointer
		this.m_playerName = id.GetName();
		this.m_playerID = id.GetPlainId();
		
#ifdef NEOFLAG_DEBUG
		Print("Settting player to: " + this.m_playerName + " (" + this.m_playerID + ")");
#endif 
		
		this._UpdateFlagManager();
		
		GetGame().GetCallQueue(CALL_CATEGORY_SYSTEM).Call(this._UpdateGameLabs);
		
#ifdef NEOFLAG_DEBUG 
		Print("Leave: SetPlayer");
#endif
	}
	
	
	// *************************************************************************
	// _UpdateGameLabs
	//
	// Function to post updated player/builder info to CFTools
	// *************************************************************************
	private void _UpdateGameLabs() 
	{
#ifdef NEOFLAG_DEBUG 
		Print("Enter: _UpdateGameLabs");
		Print("Posting player name/id: " + m_playerName + "/" + m_playerID);
		Print("Posting builder name/id: " + m_builderName + "/" +m_builderID);
#endif

#ifndef GAMELABS
#ifdef NEOFLAG_INFO
		Print("Gamelabs not installed");
#endif
#ifdef NEOFLAG_DEBUG 
		Print("Leave: _UpdateGameLabs");
#endif
		return;
#endif

#ifdef GAMELABS
		// This code is mostly from CFTools _InitGameLabs example for this class 
		// on github, we tweaked it to add the player/builder info
		float remainingLifetime = GetLifetime() / 3600;
		if(GetGameLabs()) 
		{
			if(GetGameLabs().IsServer()) 
			{
				//_registeredInstance class member is actually provided by CFTools
				// for this class
				if(this._registeredInstance) 
				{
					GetGameLabs().RemoveEvent(this._registeredInstance);
				}
				this._registeredInstance = new _Event("<b>Territory Flag</b><br/>PlacedBy: " + m_builderName + " (" + m_builderID + ")<br/>" + "Raised By: " + m_playerName + " (" + m_playerID + ")<br/>" + "Flag Level: " + Math.Round(GetRefresherTime01() * 100) + " %<br/>Remaining Lifetime: ~ " + Math.Round(remainingLifetime) + " hours", "pennant", this);
				GetGameLabs().RegisterEvent(this._registeredInstance);
			}
		}
#endif

#ifdef NEOFLAG_DEBUG 
		Print("Leave: _UpdateGameLabs");
#endif
	}


	// *****************************************************************
	// Destructor
	// This is from CFTools github example for this class and removes the 
	// CFTools marker when the object is destroyed
	// *****************************************************************
	void ~TerritoryFlag () 
	{
#ifdef GAMELABS
		if(GetGameLabs()) 
		{
			if(GetGameLabs().IsServer()) 
			{
				if(this._registeredInstance) 
				{
					GetGameLabs().RemoveEvent(this._registeredInstance);
				}
			}
		}
#endif
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
#ifdef NEOFLAG_DEBUG
		Print("Enter: AfterStoreLoad");
#endif
		super.AfterStoreLoad();
		
		TerritoryFlagManager tfm;
		NEOFlagInfo nfi;
		
		tfm = GetFlagManager();
		if (!tfm)
		{
#ifdef NEOFLAG_INFO
			Print("Unable to get flag manager");
			Print("Leave: AfterStoreLoad");
#endif
			return;
		}
		
		
		nfi = tfm.GetFlagInfo(this.GetPosition());
		
		
		if (nfi)
		{
			this.m_playerName = nfi.m_playerName;
			this.m_playerID = nfi.m_playerID;
			this.m_builderName = nfi.m_builderName;
			this.m_builderID = nfi.m_builderID;
		}
		
		// defer this call a bit extra just so we don't conflict with the CF tools constructor (5 second delay)
		GetGame().GetCallQueue(CALL_CATEGORY_SYSTEM).CallLater(this._UpdateGameLabs, 5000);
#ifdef NEOFLAG_DEBUG
		Print("Leave: AfterStoreLoad");
#endif
	}
	
	
	// ***************************************************************************
	//LogAnimateFlag
	//
	// Hook function here for easier access to when flag is raised than hooking action
	// 'delta' value has already been normalized where we can test for 0 by prior
	// function calls and player param can not be null
	
	override protected void LogAnimateFlag(float newPhase, notnull PlayerBase player)	
	{
#ifdef NEOFLAG_DEBUG
		Print("Enter: LogAnimateFlag");
#endif
		
		super.LogAnimateFlag(newPhase, player);
		
		if (newPhase == 0) // 0 == flag raised all the way
	    {
			this.SetPlayer(player);
		}
		
#ifdef NEOFLAG_DEBUG
		Print("Leave: LogAnimateFlag");
#endif
	}
	

	// ******************************************************************************
	// OnPartBuiltServer
	//
	// This is a better place to catch who is building the flag, it'll end up
	// storing the latest builder for the flag.
	override void OnPartBuiltServer( notnull Man player, string part_name, int action_id )
	{
#ifdef NEOFLAG_DEBUG
		Print("Enter: OnPartBuiltServer");
#endif

		super.OnPartBuiltServer(player, part_name, action_id);
		this.SetBuilder(player);
		
#ifdef NEOFLAG_DEBUG
		Print("Leave: OnPartBuiltServer");
#endif
	}
}; // end modded TerritoryFlag class


