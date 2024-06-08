# NEO Flag Tracker
## Description
The purpose of this mod is to provide a server-side only mod that will display flag-owner information on 
your server's CFTools (see https://cftools.com/) map. Other mods that were considered often added further 
functionality that required a client-side mod. (i.e. https://github.com/DaemonForge/DayZ-BasicTerritories). 
NEO Farmers also avoids server wipes as much as possible, so we avoided altering OnStoreSave/OnStoreLoad on 
the built-in object and store the flag-owner information in a JSON file that also provides offline 
human-readability (stored in profiles directory as neoflags.json).
## Example(s)
<img width="278" alt="image" src="https://github.com/NEO-Farmers/AdminTools/assets/18753203/c97d716e-1c74-455a-8754-e682c43bd9ee">

## Requirements
- DayZServer
- CFTools (with account)
## Installation
### FIRST!!
Get CFTools running properly with your server and verify that your map on CFTools works properly. (see 
CFTools Steam page https://steamcommunity.com/workshop/filedetails/?id=2464526692 or 
their website https://cftools.com/ for more information)
### Download from GitHub or Build in DayZTools
#### Builds from GitHub
Build artifacts coming soon...
#### Build in DayZTools
**TODO**: Show how to build in CFTools
### Set server to use mod
#### Commandline
Add -serverMod=\<cftools directory here\>;@NEO_FlagTracker
(note: These should be directories in the top-level dayzserver directory and the names on your system may 
vary depending on how you set it up)
Be sure to use the **-serverMod** argument not -mod
#### OmegaManager (a CFTools server manager)
If you use OmegaManager you can specify a non-workshop mod and run it server-side only by adding the following:
**TODO**:add example
