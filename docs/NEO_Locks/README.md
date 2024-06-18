# NEO Locks
## NOTE: EXPERIMENTAL
This mod is still under testing for server stability. We've already had one glitch that broke freshly 
spawned locks (which is now fixed). Hopefully, we'll have time to post a post-mortem on that one under issues in the future.
## Description
This mod is designed to allow admins to use icons on their CFTools map to get lock combinations or even unlock
them from the map on the CFTools website.  Running a PVE server leads to a LOT of player construction. In order 
to prevent CFTools map icons from overloading the map, there is a command to toggle show/hide locks and will 
only display or allowed to interact with locks that have been locked.

## Example(s)
World action to Show/Hide locks (on PVE servers with lots of building they would otherwise crowd the map):

![image](https://github.com/NEO-Farmers/AdminTools/assets/18753203/19836f85-d5a3-4b7a-a631-c4c76d2ba252)

Locks are now visible with their combination, if they were locked since last restart it will show who locked it:

![image](https://github.com/NEO-Farmers/AdminTools/assets/18753203/dcaa0226-b70c-428a-b3cd-fcb2c2d3460f)

Right-click on a lock to see the Unlock lock object-based action (note, deleting lock objects seems to break fences, 
this mod will quietly reject attempts to do so save for a warning in the GameLabs log):

![image](https://github.com/NEO-Farmers/AdminTools/assets/18753203/216b7d1a-dc07-4e2d-8d61-4a5cb173f59a)

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
to create an output directory (in my case I had to make the @NEO_Locks and then addons for the following example before Addon builder could see them). Set your source directory and your destination directory and options like so:

![image](https://github.com/NEO-Farmers/AdminTools/assets/18753203/a9436665-0a4b-4b1e-9570-9af06ec41f06)
![image](https://github.com/NEO-Farmers/AdminTools/assets/18753203/0d7a9cba-5085-483c-9719-22c5153cce1e)

Then click "PACK" and you should get a built mod in your @NEO_Locks directory. Signatures are only required to verify mods between clients and servers and this one is only installed on the server, so signing is unnecessary.

### Set server to use mod
#### Commandline
Add -serverMod=\<gamelabs cftools directory here\>;@NEO_Locks
(note: These should be directories in the top-level dayzserver directory and the names on your system may 
vary depending on how you set it up)
Be sure to use the **-serverMod** argument not -mod and be sure the NEO_Locks mod is AFTER your Gamelabs CFTools mod.
#### OmegaManager (a CFTools server manager)
If you use OmegaManager you can specify a non-workshop mod and run it server-side only by adding it to your mod list with
file_id of 0. Make sure the NEO_Locks mod is AFTER the GameLabs CFTools mod. Also, Make sure "server_side" is true:
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
               "directory": "@NEO_Locks", 
               "disable_updates": false, 
               "disabled": false, 
               "file_id": 0, 
               "server_side": true
          }
<...>
```
