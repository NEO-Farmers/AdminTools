#ifdef SERVER

// mostly using the old easter egg code to determine when thrown and then we kill on contact or touch of living player
// https://github.com/BohemiaInteractive/DayZ-Script-Diff/blob/a22a0553779b157f172283233e91933cc8ee2102/scripts/4_world/entities/itembase/gear/consumables/easteregg.c#L117C1-L135C3

bool NEO_DodgeBallDebug = true;

modded class Pumpkin : Edible_Base
{
    bool NEO_i_am_a_dodgeball_now = false;

    void Pumpkin()
    {
        SetEventMask(GetEventMask() | EntityEvent.TOUCH);
    }
    
    void NEO_DodgeBallContact(IEntity other)
    {
        if (NEO_DodgeBallDebug)
        {
            GetGame().AdminLog("In NEO_DodgeBallContact");
        }
        if (NEO_i_am_a_dodgeball_now)
        {
            if (NEO_DodgeBallDebug)
            {
                GetGame().AdminLog("Pumpkin is dodgeball");
            }
            // if not moving (velocity less than .2 for now, then no longer deadly), 
            // otherwise I think ppl will die picking up a stopped 'ball'
            float velocity = GetVelocity(this).Length();
            if (velocity < 0.2)
            {
                if (NEO_DodgeBallDebug)
                {
                    GetGame().AdminLog(string.Format("NEO_Dodgeball Velocity too low = %1", velocity));
                }
                NEO_i_am_a_dodgeball_now = false;
                return;
            }
            
            if (other)
            {
                Man m = Man.Cast(other);
                if (m)
                {
                    if (m.IsAlive()) // can bounce past dead bodies
                    {
                        if (NEO_DodgeBallDebug)
                        {
                            GetGame().AdminLog("Killing player");
                        }
                        m.SetHealth("","",0.0);
                        NEO_i_am_a_dodgeball_now = false;
                    }
                    else
                    {
                        if (NEO_DodgeBallDebug)
                        {
                            GetGame().AdminLog("contacted player was already dead");
                        }
                    }
                }
                else
                {
                    if (NEO_DodgeBallDebug)
                    {
                        GetGame().AdminLog("other in contact was not a player");
                    }
                }
            }
            else
            {
                if (NEO_DodgeBallDebug)
                {
                    GetGame().AdminLog("other was null in touch/contact");
                }
            }
        }
    }
    
    // hit other entities
    override void EOnTouch( IEntity other, int extra )
    {
        if (NEO_DodgeBallDebug)
        {
            GetGame().AdminLog("In EOnTouch in dodgeball mod");
        }
        NEO_DodgeBallContact(other);
    }
    
    // hit things like the ground (gonna implement just in case)
    override void EOnContact( IEntity other, Contact extra )
    {
        if (NEO_DodgeBallDebug)
        {
            GetGame().AdminLog("In EOnContact in dodgeball mod");
        }
        NEO_DodgeBallContact(other);
    }
    
    override void OnInventoryExit( Man player )
    {
        if (NEO_DodgeBallDebug)
        {
            GetGame().AdminLog("In OnInventoryExit in dodgeball mod");
        }
        super.OnInventoryExit(player);
        NEO_i_am_a_dodgeball_now = false;
        
        PlayerBase p = PlayerBase.Cast( player );
        if (p)
        {
            DayZPlayerImplementThrowing player_throwing = p.GetThrowing();
            if (player_throwing)
            {
                if (player_throwing.IsThrowingAnimationPlaying())
                {
                    if (NEO_DodgeBallDebug)
                    {
                        GetGame().AdminLog("Is deadly dodge ball now");
                    }
                    NEO_i_am_a_dodgeball_now = true;
                }
                else
                {
                    if (NEO_DodgeBallDebug)
                    {
                        GetGame().AdminLog("was not in throwing animation");
                    }
                }
            }
            else
            {
                if (NEO_DodgeBallDebug)
                {
                    GetGame().AdminLog("wasn't being thrown");
                }
            }
        }
        else
        {
            if (NEO_DodgeBallDebug)
            {
                GetGame().AdminLog("didn't exit player inventory");
            }
        }
    }
}


#endif // SERVER