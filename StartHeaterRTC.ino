#include <Servo.h>
#include <virtuabotixRTC.h>                                                                              //|
#include <stdlib.h>
#include <time.h>
#include <stdio.h>
#include <RTClib.h>
//#include <LiquidCrystal_I2C.h>


/*
KYGM Services LLC, All Rights Reserved
Kevin Gutierrez
4/18/2022
*/
//Variable Declaration
//LiquidCrystal_I2C lcd(0x3f,20,4);  // set the LCD address to 0x27 for a 16 chars and 2 line display
Servo servo;
const uint8_t servoPin = 5; //pwm signal - pwm pin
const uint8_t CLK = 7;
const uint8_t DAT = 8;
const uint8_t RST = 9;
virtuabotixRTC myRTC(CLK, DAT, RST);
boolean manualCutoff = false;
boolean willRunToday = true;


void setup() 
{
	//Servo initialization
  servo.attach(servoPin);
  servo.write(0);
	pinMode(LED_BUILTIN, OUTPUT);
  digitalWrite(LED_BUILTIN, HIGH);
  delay(500);
  digitalWrite(LED_BUILTIN, LOW);
  delay(500);
 
  //to set rtc time, uncommont the following line:
  //myRTC.setDS1302Time(00, 31, 9 , 1, 25, 4, 2022);
	Serial.begin(9600);
	//etDS1302Time(seconds, minutes, hour, dayofweek, day, month, year).
}

bool ranToday = false;
uint8_t hourToStart = 6;
void loop() 
{

  //The following function must be called each time that the loop runs if we want
  //updated time.
  myRTC.updateTime();
  blinkTime(myRTC);
  delay(2000);
  if(myRTC.dayofweek != 0 && myRTC.dayofweek != 6)
	{
		if(myRTC.hours == hourToStart && !ranToday)
		{
			turnServo(135);
      ranToday = true;
		}
    else
    {
      if(ranToday && myRTC.hours != hourToStart)
      {
        ranToday = false;
      }
    }
	}
}

void turnServo(uint16_t tAngle)
{
	int angle = 0;
	// put your main code here, to run repeatedly:
	// scan from 0 to tAngle degrees
	for(angle = 10; angle < tAngle; angle++)
	{
		servo.write(angle);
		delay(15);
	}
	// now scan back from tAngle to 0 degrees
	for(angle = tAngle; angle > 10; angle--)
	{
		servo.write(angle);
		delay(15);
	}
}

void printTime(virtuabotixRTC myRTC)
{
	Serial.print("Current Date / Time: ");                                                                 //|
	Serial.print(myRTC.month);                                                                        //|
	Serial.print("/");                                                                                     //|
	Serial.print(myRTC.dayofmonth);                                                                             //|
	Serial.print("/");                                                                                     //|
	Serial.print(myRTC.year);                                                                              //|
	Serial.print("  ");                                                                                    //|
	Serial.print(myRTC.hours);                                                                             //|
	Serial.print(":");                                                                                     //|
	Serial.print(myRTC.minutes);                                                                           //|
	Serial.print(":");                                                                                     //|
	Serial.println(myRTC.seconds);
}

void blinkTime(virtuabotixRTC myRTC)
{
	uint8_t hours = myRTC.hours;
	uint8_t minutes = myRTC.minutes;
		
	delay(2000);

  for(int x = 0; x < myRTC.dayofweek; x++)
  {
    digitalWrite(LED_BUILTIN, HIGH);
    delay(2000);
    digitalWrite(LED_BUILTIN, LOW);
    delay(500);
  }

  delay(2000);
	for(int x = 0; x < hours; x++)
	{
		digitalWrite(LED_BUILTIN, HIGH);
		delay(500);
		digitalWrite(LED_BUILTIN, LOW);
    delay(500);
	}
	delay(2000);
	for(int x = 0; x < minutes; x++)
	{
		digitalWrite(LED_BUILTIN, HIGH);
		delay(100);
		digitalWrite(LED_BUILTIN, LOW);
    delay(100);
	}
 delay(2000);
}
