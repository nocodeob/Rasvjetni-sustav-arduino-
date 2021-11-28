void off ()
{
	digitalWrite(Relay_5, RELAY_OFF);// set the Relay OFF
	delay(a[0]);
	digitalWrite(Relay_4, RELAY_OFF);// set the Relay OFF
	delay(a[1]);
	digitalWrite(Relay_3, RELAY_OFF);// set the Relay OFF
	delay(a[2]);
	digitalWrite(Relay_2, RELAY_OFF);// set the Relay OFF
	delay(a[3]);
	digitalWrite(Relay_1, RELAY_OFF);
	delay(a[4]);

	lightState = false;
}

void offWall()
{
	digitalWrite(Relay_6, RELAY_OFF);
	wallLightState = false;
}