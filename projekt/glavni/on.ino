void on ()
{
  digitalWrite(Relay_1, RELAY_ON);// set the Relay ON
  delay(a[0]);
  digitalWrite(Relay_2, RELAY_ON);// set the Relay ON
  delay(a[1]);
  digitalWrite(Relay_3, RELAY_ON);// set the Relay ON
  delay(a[2]);
  digitalWrite(Relay_4, RELAY_ON);// set the Relay ON
  delay(a[3]);
  digitalWrite(Relay_5, RELAY_ON);// set the Relay ON
  delay(a[4]);
  lightState = true;
}

void onWall()
{
    digitalWrite(Relay_6, RELAY_ON);// set the Relay ON
    wallLightState = true;
}



