/*
  Name: DFPlayer
  Board: esp32 -> ESP32S3 Dev Module / DevKit
  Hardware: ayatec sensoraya v2.1

  Description:
  Connects to the on-board DFPlayer via secondary HW Serial (Serial1).
  Plays 3-second MP3 track samples from SD card.

  Pin usage:
  - DFPlayer: IO41, IO42 (DFPlayer TX/RX, see sensoraya v2.1 schematic)

  Dependencies:
  - N/A

  Created: 2026-02-09
  Author: ayatec (Michal Liptak)
  License: MIT
*/

#include "DFRobotDFPlayerMini.h"

DFRobotDFPlayerMini myDFPlayer;
void printDetail(uint8_t type, int value);

#define PIN_DFPlayer_RX   41
#define PIN_DFPlayer_TX   42
#define DFPlayerTimeout   500
#define DFPlayerVolume    10   //Set volume value. From 0 to 30 (logarithmic scale)

void setup()
 { 
  Serial.begin(115200);           // setting communication with PC
  Serial1.begin(9600, SERIAL_8N1, PIN_DFPlayer_RX, PIN_DFPlayer_TX);
  
  Serial.println();
  Serial.println(F("Initializing Sensoraya DFRobot DFPlayer Mini Demo"));

  if (!myDFPlayer.begin(Serial1)) {
    Serial.println(F("Unable to begin. Please recheck the connection and SD card!"));
    while(true){
      delay(1); // halt here if DFPlayer init fails
    }
  }
  myDFPlayer.setTimeOut(DFPlayerTimeout);
  myDFPlayer.outputDevice(DFPLAYER_DEVICE_SD);
  myDFPlayer.volume(DFPlayerVolume);
  myDFPlayer.play(1);  //Play the first mp3
}

void loop()
{
  static unsigned long timer = 0;
  
  if (millis() - timer > 5000) {
    timer = millis();
    myDFPlayer.next();  //Play next mp3 every few seconds.
    Serial.println(F("Next track"));
  }
  
  if (myDFPlayer.available()) {
    printDetail(myDFPlayer.readType(), myDFPlayer.read()); //Print the detail message from DFPlayer to handle different errors and states.
  }
  delay(1); // keep watch dog happy
}

void printDetail(uint8_t type, int value){
  switch (type) {
    case TimeOut:
      Serial.println(F("Time Out!"));
      break;
    case WrongStack:
      Serial.println(F("Stack Wrong!"));
      break;
    case DFPlayerCardInserted:
      Serial.println(F("Card Inserted!"));
      break;
    case DFPlayerCardRemoved:
      Serial.println(F("Card Removed!"));
      break;
    case DFPlayerCardOnline:
      Serial.println(F("Card Online!"));
      break;
    case DFPlayerUSBInserted:
      Serial.println("USB Inserted!");
      break;
    case DFPlayerUSBRemoved:
      Serial.println("USB Removed!");
      break;
    case DFPlayerPlayFinished:
      Serial.print(F("Number:"));
      Serial.print(value);
      Serial.println(F(" Play Finished!"));
      break;
    case DFPlayerError:
      Serial.print(F("DFPlayerError:"));
      switch (value) {
        case Busy:
          Serial.println(F("Card not found"));
          break;
        case Sleeping:
          Serial.println(F("Sleeping"));
          break;
        case SerialWrongStack:
          Serial.println(F("Get Wrong Stack"));
          break;
        case CheckSumNotMatch:
          Serial.println(F("Check Sum Not Match"));
          break;
        case FileIndexOut:
          Serial.println(F("File Index Out of Bound"));
          break;
        case FileMismatch:
          Serial.println(F("Cannot Find File"));
          break;
        case Advertise:
          Serial.println(F("In Advertise"));
          break;
        default:
          break;
      }
      break;
    default:
      break;
  }
  
}
