// pretty much straight from:
// https://github.com/CFToolsGameLabs/dayz-examples/blob/master/loggable_actions/mission_server.c
// just updated to our actions and a little formatting

modded class MissionServer {
#ifdef GAMELABS
    override void GLActionRegisterHook() {
        super.GLActionRegisterHook();

        // force feed actions found in scripts/4_world/classes/useractionscomponent/actionconstructor.c
        // ActionForceConsumeSingle may not get logged as it is not a 'continous' action, but appears to cover
        // medical items so should not be a malicous act
        GetGameLabs().BLAHABLAH();
        GetGameLabs().AddMonitoredAction("ActionForceConsumeSingle");
        GetGameLabs().AddMonitoredAction("ActionForceDrink");
        GetGameLabs().AddMonitoredAction("ActionForceFeed");
        GetGameLabs().AddMonitoredAction("ActionForceFeedSmall");
        GetGameLabs().AddMonitoredAction("ActionForceFeedCan");

        // trap actions from actionconstructor.c
        GetGameLabs().AddMonitoredAction("ActionActivateTrap");

        // agressive player actions
        GetGameLabs().AddMonitoredAction("ActionRestrainTarget");
        GetGameLabs().AddMonitoredAction("ActionCoverHeadTarget");
    }
#endif
};