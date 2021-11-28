#include <DS1307RTC.h>   //http://www.arduino.cc/playground/Code/Time
#include <TimeLib.h>        //http://www.arduino.cc/playground/Code/Time
#include <Timezone.h>    //https://github.com/JChristensen/Timezone
#include <Wire.h>        //http://arduino.cc/en/Reference/Wire (supplied with the Arduino IDE)
#include <LiquidCrystal.h>
#include <Bounce2.h>
#include <ds3231.h>
#include <EEPROM.h>

// define button pins
#define editButtonPin 23
#define minusButtonPin 21
#define plusButtonPin 22
#define wallLightPin 7
#define button1Pin 6
#define button2Pin 5
#define gatePin 4

#define RELAY_ON 0
#define RELAY_OFF 1
#define Relay_1  12  // Arduino Digital I/O pin number
#define Relay_2  11
#define Relay_3  10
#define Relay_4  9
#define Relay_5  8
#define Relay_6  7

////pin out for 5 lights
//int light[] = {12, 11, 10, 9, 8};

///CROATIA TIMEZONE
TimeChangeRule myDST = {"EDT", Last, Sun, Mar, 2, 120};    //Daylight time = UTC 1 hour
TimeChangeRule mySTD = {"CEST", Last, Sun, Oct, 3, 120};     //Standard time = UTC 2 hours
Timezone myTZ(myDST, mySTD);

//If TimeChangeRules are already stored in EEPROM, comment out the three
//lines above and uncomment the line below.
//Timezone myTZ(100);       //assumes rules stored at EEPROM address 100

TimeChangeRule *tcr;        //pointer to the time change rule, use to get TZ abbrev
time_t utc, local;

bool editButtonState = false;
bool minusButtonState = false;
bool plusButtonstate = false;
bool gateState1 = false;
bool gateState2 = false;
bool gateState1Btn = false;
bool gateState2Btn = false;
bool button1State = false;
bool button2State = false;
bool editEntrence = false;

bool screenPickerTracker = false;
bool lightState = false;
bool wallLightState = false;

int lightOnTime = EEPROM.read(1); //eeprom 1
int lightOffTime = EEPROM.read(2); // eeprom 2
int gateOnTime = EEPROM.read(3); // eeprom 3
int gateOffTime = EEPROM.read(4); // eeprom 4
int screenPicker = 0;
int editMode = 0;
int a[5] = {EEPROM.read(5) * 100, EEPROM.read(6) * 100, EEPROM.read(7) * 100,
            EEPROM.read(8) * 100, EEPROM.read(9) * 100
           }; // eeprom 5,6,7,8,9

unsigned long previousTime = 0;
unsigned long previousWallTime = 0;
unsigned long previousGateTime = 0;
unsigned long previousEditTime = 0;
unsigned long currentTime = 0;
unsigned long lightInterval = EEPROM.read(10) * 60000; // eeprom 10
unsigned long lightGateInterval = EEPROM.read(11) * 60000; // eeprom 11


char tempF[6];
float temperature;

//initialize lcd and debouncers
//LiquidCrystal lcd(RS, Enable, D4, D5, D6, D7)
LiquidCrystal lcd(20, 17, 16, 15, 14, 13);
Bounce debouncer1 = Bounce(); // button1
Bounce debouncer2 = Bounce(); // editButton
Bounce debouncer3 = Bounce(); // minusButton
Bounce debouncer4 = Bounce(); // plusButton
Bounce debouncer5 = Bounce(); // button2

void setup(void)
{
    Serial.begin(115200);
    lcd.begin(16, 2);
    setSyncProvider(RTC.get);   // the function to get the time from the RTC
    if (timeStatus() != timeSet)
    {
      Serial.println("Unable to sync with the RTC");
      lcd.print("ds3231");
    }
    else
        Serial.println("RTC has set the system time");

    //---( THEN set pins as outputs )----
    pinMode(Relay_1, OUTPUT);
    digitalWrite(Relay_1, RELAY_OFF);
    pinMode(Relay_2, OUTPUT);
    digitalWrite(Relay_2, RELAY_OFF);
    pinMode(Relay_3, OUTPUT);
    digitalWrite(Relay_3, RELAY_OFF);
    pinMode(Relay_4, OUTPUT);
    digitalWrite(Relay_4, RELAY_OFF);
    pinMode(Relay_5, OUTPUT);
    digitalWrite(Relay_5, RELAY_OFF);
    pinMode(Relay_6, OUTPUT);
    digitalWrite(Relay_6, RELAY_OFF);


    pinMode (gatePin, INPUT_PULLUP);

    //debouncer for button1 (tipkalo)
    pinMode (button1Pin, INPUT_PULLUP);
    debouncer1.attach(button1Pin);
    debouncer1.interval(5);
    //debounce for editButton
    pinMode (editButtonPin, INPUT_PULLUP);
    debouncer2.attach(editButtonPin);
    debouncer2.interval(5);
    //debounce for minusButton
    pinMode (minusButtonPin, INPUT_PULLUP);
    debouncer3.attach(minusButtonPin);
    debouncer3.interval(5);
    //debounce for plusButton
    pinMode (plusButtonPin, INPUT_PULLUP);
    debouncer4.attach(plusButtonPin);
    debouncer4.interval(5);
    //debounce for button2
    pinMode (button2Pin, INPUT_PULLUP);
    debouncer5.attach(button2Pin);
    debouncer5.interval(5);

   

}

void loop(void)
{
    Serial.println();
    utc = now();
    local = myTZ.toLocal(utc, &tcr);
    currentTime = millis();

    //buttons update
    debouncer1.update();
    debouncer2.update();
    debouncer3.update();
    debouncer4.update();
    debouncer5.update(); // dodao button2

    conditions(local);

    Serial.print("button 1 state: ");
    Serial.println(button1State);
    Serial.print("button 2 state: ");
    Serial.println(button2State);
    Serial.print("light state: ");
    Serial.println(lightState);
    Serial.print("wall light state: ");
    Serial.println(wallLightState);
    Serial.print("hour utc: ");
    Serial.println(hour(utc));
    Serial.print("hourlocal: ");
    Serial.println(hour(local));
    Serial.print("gateState1: ");
    Serial.println(gateState1);
    Serial.print("gateState2: ");
    Serial.println(gateState2);
    Serial.print("ScreenPicker: ");
    Serial.println(screenPicker);
    Serial.print("lightOnTime: ");
    Serial.println(lightOnTime);
    Serial.print("lightOfTime: ");
    Serial.println(lightOffTime);
    Serial.print("gateOnTime: ");
    Serial.println(gateOnTime);
    Serial.print("gateOffTime: ");
    Serial.println(gateOffTime);
    Serial.print("VRJEME: ");
    Serial.println(local);



    temperature = DS3231_get_treg(); //Get temperature
    dtostrf(temperature, 5, 1, tempF);

    if (currentTime - previousTime >= lightInterval && button1State == true)
    {
        if (gateState1Btn == true)
        {
            off();
            button1State = false;
            gateState1Btn = false;
        }
        else
            lightState == true ? off() : on();
        button1State = false;
    }

    if (currentTime - previousGateTime >= lightGateInterval && gateState1 == true) // dali ovdje fali button?
    {
        off();
        gateState1 = false;
    }


    if (currentTime - previousWallTime >= lightInterval && button2State == true)
    {
        if (gateState2Btn == true)
        {
            offWall();
            button2State = true;
            gateState2Btn = false;
        }
        else
            wallLightState == true ? offWall() : onWall();
        button2State = false;
    }

    // ovaj dio prebaci kod gate time prvog PROVJERI
    if (currentTime - previousGateTime >= lightGateInterval && gateState2 == true) // dali ovdje fali button?
    {
        offWall();
        gateState2 = false;
    }

    switch (screenPicker)
    {
        case 0:
        {
            homeScreen(local);
            if (screenPickerTracker == false)
            {
                lcd.clear();
                screenPickerTracker = true;
            }
            btnLogic();
        }
        break;

        case 1:
        {
            if (screenPickerTracker == false)
            {
                lcd.clear();
                screen1();
                screenPickerTracker = true;
            }
            //entry in edit mode
            if (debouncer2.read() == LOW && debouncer2.duration() > 2000 && editEntrence == false)
            {
                editMode = 1;
                editButtonState = true;
            }

            editMode > 0 ? editLogic(editMode) : btnLogic();
            if (debouncer2.rose() == true)
                editEntrence = false;
        }
        break;

        case 2:
        {
            if (screenPickerTracker == false)
            {
                lcd.clear();
                screen2();
                screenPickerTracker = true;
            }
            //entry in edit mode
            if (debouncer2.read() == LOW && debouncer2.duration() > 2000 && editEntrence == false)
            {
                editMode = 2;
                editButtonState = true;
            }

            editMode > 0 ? editLogic(editMode) : btnLogic();
            if (debouncer2.rose() == true)
                editEntrence = false;
        }
        break;

        case 3:
        {
            if (screenPickerTracker == false)
            {
                lcd.clear();
                screen3();
                screenPickerTracker = true;
            }
            //entry in edit mode
            if (debouncer2.read() == LOW && debouncer2.duration() > 2000 && editEntrence == false)
            {
                editMode = 3;
                editButtonState = true;
            }
            editMode > 0 ? editLogic(editMode) : btnLogic();
            if (debouncer2.rose() == true)
                editEntrence = false;
        }
        break;

        case 4:
        {
            if (screenPickerTracker == false)
            {
                lcd.clear();
                screen4();
                screenPickerTracker = true;
            }
            //entry in edit mode
            if (debouncer2.read() == LOW && debouncer2.duration() > 2000 && editEntrence == false)
            {
                editMode = 4;
                editButtonState = true;
            }
            editMode > 0 ? editLogic(editMode) : btnLogic();
            if (debouncer2.rose() == true)
                editEntrence = false;
        }
        break;
    }
}
