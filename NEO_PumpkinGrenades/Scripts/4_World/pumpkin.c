// mostly using the old easter egg code to determine when thrown and then we explode on contact or touch
// https://github.com/BohemiaInteractive/DayZ-Script-Diff/blob/a22a0553779b157f172283233e91933cc8ee2102/scripts/4_world/entities/itembase/gear/consumables/easteregg.c#L117C1-L135C3

modded class Pumpkin : Edible_Base
{
    bool NEO_i_am_a_grenade_now = false;

    //Used for capture
    override void EOnTouch( IEntity other, int extra )
    {
        if (NEO_i_am_a_grenade_now)
        {
            Explode(DT_EXPLOSION, "Explosion_NonLethal");
            NEO_i_am_a_grenade_now = false;
        }
    }
    
    //Used for release
    override void EOnContact( IEntity other, Contact extra )
    {
        if (NEO_i_am_a_grenade_now)
        {
            Explode(DT_EXPLOSION, "Explosion_NonLethal");
            NEO_i_am_a_grenade_now = false;
        }
    }
    
    override void OnInventoryExit( Man player )
    {
        // Easter egg code doesn't call super here... so let's hope for the best
        
        PlayerBase p = PlayerBase.Cast( player );
        if (p)
        {
            DayZPlayerImplementThrowing player_throwing = p.GetThrowing();
            if (player_throwing)
            {
                if (player_throwing.IsThrowingAnimationPlaying())
                {
                    NEO_i_am_a_grenade_now = true;
                }
            }
        }
        super.OnInventoryExit(player);
    }
}