
void conditions(time_t t)
{
    // turn on lights in interval form lightOnTime to lightOffTime
    if (hour(t) >= lightOnTime && hour(t) <= lightOffTime && lightState == false && button1State == false) // added lightState
        on();

    //turn off lights in interval if button1 wasn't pressed, gate wasn't open and light was on
    else if (hour(t) >= gateOnTime && hour(t) <= gateOffTime && lightState == true && gateState1 == false && button1State == false)
        off();

    //digitalRead(gate) == HIGH ako su vrata otvorena
    if (hour(t) >= gateOnTime && hour(t) <= gateOffTime && digitalRead(gatePin) == HIGH ) // removed && button1State == false
    {
        if (lightState == false)
        {
            on();
            gateState1 = true;
        }
        previousGateTime = currentTime;

    }

    //-------------------wallLight----------------
    // turn on wall lights in interval form lightOnTime to lightOffTime
    if (hour(t) >= lightOnTime && hour(t) <= lightOffTime && wallLightState == false && button2State == false) // added lightState
        onWall();

    //turn off wall lights in interval from 0 to 6 and check for gate
    else if (hour(t) >= gateOnTime && hour(t) <= gateOffTime && wallLightState == true && gateState2 == false && button2State == false)
        offWall();

    //digitalRead(gate) == HIGH ako su vrata otvorena
    if (hour(t) >= gateOnTime && hour(t) <= gateOffTime && digitalRead(gatePin) == HIGH ) // removed && button2State == false
    {
        if (wallLightState == false)
		{
            onWall();
			gateState2 = true;
		}
        previousGateTime = currentTime;
        
    }



    //turn on or off  5 lights depending on previous light state
    if (debouncer1.fell() == true)
    {
		// if gate was opened and btn 1 was pressed turn off light and continue with normal program
        if (gateState1 == true)
        {
            gateState1 = false;
            off();
			debouncer1.update();
        }
		
        else if (button1State ==  true)
        {
            lightState == true ? off() : on();
            button1State = false;
            gateState1 = false;
        }
        else
        {
            button1State =  true;
            previousTime = currentTime;
            lightState == true ? off() : on();
            gateState1 = false;
            gateState1Btn = true;
        }
    }


    if (debouncer5.fell() == true)
    {
		// if gate was opened and btn 2 was pressed turn off light and continue with normal program
        if (gateState2 == true)
        {
            gateState2 = false;
            offWall();
			
        }
        else if (button2State ==  true)
        {
            wallLightState == true ? offWall() : onWall();
            button2State = false;
            gateState2 = false;
        }
        else
        {
            button2State =  true;
            previousWallTime = currentTime;
            wallLightState == true ? offWall() : onWall();
            gateState2 = false;
            gateState2Btn = true;
        }
    }
}


