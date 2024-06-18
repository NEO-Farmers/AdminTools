// pretty much straight from:
// https://github.com/CFToolsGameLabs/dayz-examples/blob/master/dynamic_actions/mission_server.c
// just updated to our actions and a little formatting

modded class MissionServer 
{
#ifdef GAMELABS
    override void GLActionRegisterHook () 
	{
        super.GLActionRegisterHook();

        NEOLock_ShowLocks().Register();
		NEOLock_UnlockLock().Register();
    }
#endif
};