
#ifdef GAMELABS

#ifndef NEO_LOG_TO_GAMELABS
#define NEO_LOG_TO_GAMELABS
static void NeoLogToGameLabs(PlayerBase player, string useditem="", string targetitem="", string actionperformed="")
{
	
#ifdef CFTOOLSEXTRALOGGING
	return SendToCFTools(player, useditem, targetitem, actionperformed);
#endif // CFTOOLSEXTRALOGGING

#ifndef CFTOOLSEXTRALOGGING
    if (!player)
    {
        return;
    }
    
	_LogPlayerEx logplayer = new _LogPlayerEx(player);
	if (logplayer)
	{
        _Payload_ItemInteract payloadinteract = new _Payload_ItemInteract(logplayer, useditem, targetitem, actionperformed);
        GetGameLabs().GetApi().ItemInteract(new _Callback(), payloadinteract);
    }
#endif // undefined CFTOOLSEXTRALOGGING

}
#endif // NEO_LOG_TO_GAMELABS

#endif // GAMELABS