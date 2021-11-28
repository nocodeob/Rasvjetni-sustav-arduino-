void homeScreen(time_t t)
{
    //vrijeme
    lcd.setCursor(0, 0);
    leadingZero(hour(t));
    lcd.print(":");
    leadingZero(minute(t));
    lcd.print(":");
    leadingZero(second(t));

    lcd.setCursor(9, 0);
    lcd.print(tempF);
    lcd.print((char)223);
    lcd.print("C");


    //datum
    lcd.setCursor(3, 1);
    leadingZero(day(t));
    lcd.print(".");
    leadingZero(month(t));
    lcd.print(".");
    leadingZero(year(t));

}

void screen1 ()
{
    lcd.setCursor(0, 0);
    lcd.print("VRATA: ");
    leadingZero(lightGateInterval / 60000);

    lcd.setCursor(0 , 1);
    lcd.print("TIPKALO: ");
    leadingZero(lightInterval / 60000);
}

void screen2 ()
{
    lcd.setCursor(4, 0);
    lcd.print("RASVJETA ");
    lcd.setCursor(2, 1);
    lcd.print("OD ");
    leadingZero(lightOnTime);
    lcd.print("  ");
    lcd.print("DO ");
    leadingZero(lightOffTime + 1);
}

void screen3 ()
{
    lcd.setCursor(5, 0);
    lcd.print("KAPIJA ");
    lcd.setCursor(2, 1);
    lcd.print("OD ");
    leadingZero(gateOnTime);
    lcd.print("  ");
    lcd.print("DO ");
    leadingZero(gateOffTime);
}

void screen4 ()
{
    String zarez = ',';
    String nesta = a[0]/100 + zarez + a[1]/100 + zarez + a[2]/100 + zarez + a[3]/100 + zarez + a[4]/100;
    lcd.setCursor(4, 0);
    lcd.print("INTERVAL");
    lcd.setCursor(3, 1);
    lcd.print(nesta);
}






