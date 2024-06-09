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
### First, make sure you have CFTools working properly
Get CFTools running properly with your server and verify that your map on CFTools works properly. (see 
CFTools Steam page https://steamcommunity.com/workshop/filedetails/?id=2464526692 or 
their website https://cftools.com/ for more information)
### Download from GitHub or Build in DayZTools
#### Builds from GitHub
Build artifacts coming soon...
#### Build in DayZTools
In Steam under tools, install DayZ Tools then, download the source and build with Add-On builder. You will probably need 
to create an output directory (in my case I had to make the @NEO_FlagTracker and then addons for the following example before Addon builder could see them). Set your source directory and your destination directory and options like so:


![image](https://github.com/NEO-Farmers/AdminTools/assets/18753203/a9436665-0a4b-4b1e-9570-9af06ec41f06)
![image](https://github.com/NEO-Farmers/AdminTools/assets/18753203/d59eacbb-5d57-4c37-8421-2bcc49966141)


Then click "PACK" and you should get a built mod in your @NEO_FlagTracker directory. Signatures are only required to verify mods between clients and servers and this one is only installed on the server, so signing is unnecessary.

### Set server to use mod
#### Commandline
Add -serverMod=\<gamelabs cftools directory here\>;@NEO_FlagTracker
(note: These should be directories in the top-level dayzserver directory and the names on your system may 
vary depending on how you set it up)
Be sure to use the **-serverMod** argument not -mod and be sure the NEO_FlagTracker mod is AFTER your Gamelabs CFTools mod.
#### OmegaManager (a CFTools server manager)
If you use OmegaManager you can specify a non-workshop mod and run it server-side only by adding it to your mod list with
file_id of 0. Make sure the NEO_FlagTracker mod is AFTER the GameLabs CFTools mod. Also, Make sure "server_side" is true:
```
<...>
     "mods": [
          {
               "directory": "@GameLabs", 
               "disable_updates": false, 
               "disabled": false, 
               "file_id": 2464526692, 
               "server_side": true
          }, 
          {
               "directory": "@NEO_FlagTracker", 
               "disable_updates": false, 
               "disabled": false, 
               "file_id": 0, 
               "server_side": true
          }
<...>
```
