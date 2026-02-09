/*
  Name: DS1307 I2C RTC
  Board: esp32 -> ESP32S3 Dev Module / DevKit
  Hardware: ayatec sensoraya v2.1

  Description:
  Connects to the on-board DS1307 Real-Time Clock via I2C.
  Reads and returns the RTC time each second.
  If the RTC time is greater than 2010-01-01, then reset the RTC time to 2010-01-01 00:00. 

  Pin usage:
  - I2C: IO39, IO40 (SCL/SDA, see sensoraya v2.1 schematic)

  Dependencies:
  - N/A

  Created: 2026-02-09
  Author: ayatec (Michal Liptak)
  License: MIT
*/

#include <Wire.h>
#include <RTClib.h>

#define PIN_SCL           39
#define PIN_SDA           40

RTC_DS1307 rtc;

void setup()
 { 
  Serial.begin(115200);           // setting communication with PC
  Wire.begin(PIN_SDA, PIN_SCL);
  delay(500);

  Serial.println("Initializing Sensoraya DS1307 RTC Demo");
  if(rtc.begin()){
    Serial.println("RTC ready!");
  } else {
    Serial.println("Hardware RTC not found!");
    while(true);   // Halt if RTC is unavailable
  }
}

void loop()
{
  DateTime now = rtc.now();
  Serial.printf("%04d-%02d-%02d %02d:%02d:%02d\n",now.year(),now.month(),now.day(),now.hour(),now.minute(),now.second());
  if(now.year() != 2010 || now.month() != 1 || now.day() != 1){   // keep date = 2010-01-01
    rtc.adjust(DateTime(2010, 1, 1, 0, 0, 0));
    Serial.println("Resetting time to 2010-01-01 00:00");
    }
  delay(1000);
}
