void editLogic(int picker)
{   lcd.cursor();
    bool walk = false;
    // using unsigned long to be able to send i to existing function
    unsigned long counter = 1;

    //screen 1
    while (picker == 1)
    {
        debouncer2.update();
        debouncer3.update();
        debouncer4.update();

        if (debouncer2.fell() == true)
            walk = !walk;

        walk == false ?  lcd.setCursor(8, 0) : lcd.setCursor(10, 1);
        Serial.println(walk);

        if (debouncer2.rose() == true)
            editButtonState = false;

        if (leave() == true)
        {
            EEPROM.update(10, lightInterval / 60000 );
            EEPROM.update(11, lightGateInterval / 60000);
            break;
        }

        if (walk == false)
        {
            minusPress(walk, lightGateInterval, picker);
            minusHold(walk, lightGateInterval, picker);
            plusPress(walk, lightGateInterval, picker);
            plusHold(walk, lightGateInterval, picker);
        }
        else
        {
            minusPress(walk, lightInterval, picker);
            minusHold(walk, lightInterval, picker);
            plusPress(walk, lightInterval, picker);
            plusHold(walk, lightInterval, picker);
        }
    }

    //screen 2
    while (picker == 2)
    {
        debouncer2.update();
        debouncer3.update();
        debouncer4.update();
        lcd.cursor();



        if (debouncer2.fell() == true)
            walk = !walk;
        walk == false ?  lcd.setCursor(6, 1) : lcd.setCursor(13, 1);

        if (debouncer2.rose() == true) editButtonState = false;

        if (leave() == true)
        {
            EEPROM.update(1, lightOnTime);
            EEPROM.update(2, lightOffTime);
            // coloration between lightOffTime and gateOnTime
            if (lightOffTime == 23)
                {
					gateOnTime = 0;
					EEPROM.update(3, gateOnTime);
				}
            else
			{
                gateOnTime = lightOffTime + 1;
				EEPROM.update(3, gateOnTime);
			}
            break;
        }

        if (walk == false)
        {
            minusPress(walk, lightOnTime, picker);
            minusHold(walk, lightOnTime, picker);
            plusPress(walk, lightOnTime, picker);
            plusHold(walk, lightOnTime, picker);

        }
        else
        {
            minusPress(walk, lightOffTime, picker);
            minusHold(walk, lightOffTime, picker);
            plusPress(walk, lightOffTime, picker);
            plusHold(walk, lightOffTime, picker);
        }
    }

    while (picker == 3)
    {
        debouncer2.update();
        debouncer3.update();
        debouncer4.update();

        if (debouncer2.fell() == true)
            walk = !walk;

        walk == false ?  lcd.setCursor(6, 1) : lcd.setCursor(13, 1);

        if (debouncer2.rose() == true) editButtonState = false;

        if (leave() == true)
        {
            EEPROM.update(3, gateOnTime);
            EEPROM.update(4, gateOffTime);
            //coloration between gateOnTime and lightOffTime
            if (gateOnTime == 0)
                {
					lightOffTime = 23;
					EEPROM.update(2, lightOffTime);
				}
            else
			{
                lightOffTime = gateOnTime - 1;
				EEPROM.update(2, lightOffTime);
			}
            break;
        }

        if (walk == false)
        {
            minusPress(walk, gateOnTime, picker);
            minusHold(walk, gateOnTime, picker);
            plusPress(walk, gateOnTime, picker);
            plusHold(walk, gateOnTime, picker);

        }
        else
        {
            minusPress(walk, gateOffTime, picker);
            minusHold(walk, gateOffTime, picker);
            plusPress(walk, gateOffTime, picker);
            plusHold(walk, gateOffTime, picker);
        }

    }

    while (picker == 4)
    {
        debouncer2.update();
        debouncer3.update();
        debouncer4.update();

        if (debouncer2.fell())
        {
            if (counter >= 5)
                counter = 1;
            else
                counter++;
            //debouncer2.update();
        }

        if (counter == 1)
            lcd.setCursor(3,1);
        else if (counter == 2)
            lcd.setCursor(5,1);
        else if (counter == 3)
            lcd.setCursor(7,1);
        else if (counter == 4)
            lcd.setCursor(9,1);
        else if (counter == 5)
            lcd.setCursor(11,1);

        if (debouncer2.rose() == true) editButtonState = false;

        if (leave() == true)
        {
            EEPROM.update(5, (a[0] / 100));
            EEPROM.update(6, (a[1] / 100));
            EEPROM.update(7, (a[2] / 100));
            EEPROM.update(8, (a[3] / 100));
            EEPROM.update(9, (a[4] / 100));
            break;
        }

        minusPress(false,counter,picker);
        plusPress(false,counter,picker);
    }
}

// leaving edit mode
bool leave()
{
    debouncer4.update();
    if (debouncer2.read() == LOW && debouncer2.duration() > 2000 && editButtonState == false)
    {
        editMode = 0;
        editEntrence = true;
        lcd.noCursor();
        editButtonState = false;
        return true;
    }
    return false;
}

void minusPress(bool walker, unsigned long interval, int i)
{
    //minusButton press for gate
    if (debouncer3.fell() == true && debouncer3.duration() < 500)
    {

        switch (i)
        {
            case 1:
            {
                if (interval == 0)
                    interval = 1 * 60000;
                else
                    interval = interval - 1 * 60000;
                if(walker == false)
                    lightGateInterval = interval;
                else
                    lightInterval = interval;
                screen1();

            }
            break;
            case 2:
            {
                if (interval == 0)
                    interval = 23;
                else
                    interval = interval - 1;
                if (walker == false)
                    lightOnTime = interval;
                else
                    lightOffTime = interval;
                screen2();
            }
            break;
            case 3:
            {
                if (interval == 0)
                    interval = 23;
                else
                    interval = interval - 1 ;
                if (walker == false)
                    gateOnTime = interval;
                else
                    gateOffTime = interval;
                screen3();
            }
            break;
            case 4:
            {
                // interval in here is walker (i)
                if (a[interval - 1] / 100 < 1)
                    a[interval - 1] = 0;
                else
                    a[interval - 1] = a[interval - 1] - 1 * 100;
                screen4();
            }
            break;
        }
    }
}

void minusHold (bool walker, unsigned long interval, int i)
{
    while (debouncer3.read() == LOW && debouncer3.duration() > 1000)
    {
        switch (i)
        {
            case 1:
            {
                if (interval == 0)
                    interval = 1 * 60000;
                else
                    interval = interval - 1 * 60000;
                if(walker == false)
                    lightGateInterval = interval;
                else
                    lightInterval = interval;
                debouncer3.update();
                screen1();
                delay (150);
            }
            break;
            case 2:
            {
                if (interval == 0)
                    interval = 23;
                else
                    interval = interval - 1;
                if(walker == false)
                    lightOnTime = interval;
                else
                    lightOffTime = interval;
                debouncer3.update();
                screen2();
                delay (150);
            }
            break;
            case 3:
            {
                if (interval == 0)
                    interval = 23;
                else
                    interval = interval - 1 ;
                if(walker == false)
                    gateOnTime = interval;
                else
                    gateOffTime = interval;
                debouncer3.update();
                screen3();
                delay (150);
            }
            break;
        }
    }
}

void plusPress (bool walker, unsigned long interval, int i)
{
    if (debouncer4.fell() == true && debouncer4.duration() < 500)
    {
        switch (i)
        {
            case 1:
            {
                interval = interval + 1 * 60000;
                if(walker == false)
                    lightGateInterval = interval;
                else
                    lightInterval = interval;
                screen1();
            }
            break;
            case 2:
            {
                if (interval > 23)
                    interval = 0;
                else
                    interval = interval + 1;
                if(walker == false)
                    lightOnTime = interval;
                else
                    lightOffTime = interval;
                screen2();
            }
            break;
            case 3:
            {
                if (interval == 23)
                    interval = 0;
                else
                    interval = interval + 1 ;
                if(walker == false)
                    gateOnTime = interval;
                else
                    gateOffTime = interval;
                screen3();
            }
            break;
            case 4:
            {
                a[interval - 1] = a[interval - 1] + 1 * 100;
                screen4();
            }
            break;
        }
    }
}

void plusHold (bool walker, unsigned long interval, int i)
{
    while (debouncer4.read() == LOW && debouncer4.duration() > 1000)
    {
        switch (i)
        {
            case 1:
            {
                interval = interval + 1 * 60000;
                if(walker == false)
                    lightGateInterval = interval;
                else
                    lightInterval = interval;
                debouncer4.update();
                screen1();
                delay (150);
            }
            break;
            case 2:
            {
                if (interval > 23)
                    interval = 0;
                else
                    interval = interval + 1;
                if (walker == false)
                    lightOnTime = interval;
                else
                    lightOffTime = interval;
                debouncer4.update();
                screen2();
                delay (150);
            }
            break;
            case 3:
            {
                if (interval == 23)
                    interval = 0;
                else
                    interval = interval + 1;
                if (walker == false)
                    gateOnTime = interval;
                else
                    gateOffTime = interval;
                debouncer4.update();
                screen3();
                delay (150);
            }
            break;
        }
    }
}
