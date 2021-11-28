void leadingZero(int val)
{
  if (val < 10) lcd.print('0');
  lcd.print(val, DEC);
  //return;
}

