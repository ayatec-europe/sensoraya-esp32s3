/*
  Name: Relays
  Board: esp32 -> ESP32S3 Dev Module / DevKit
  Hardware: ayatec sensoraya v2.1

  Description:
  Loops between individual relays (1-10) using 74HC595 Shift register.

  Pin usage:
  - IO21, IO45, IO47, IO48: Shift register control pins (SRCLK, RCLK, OE, and SER, respectively, see sensoraya v2.1 schematic)

  Created: 2026-02-02
  Author: ayatec (Michal Liptak)
  License: MIT
*/

#define PIN_SR_SRCLK      21  // 595 SR - SRCLK
#define PIN_SR_RCLK       45  // 595 SR - RCLK
#define PIN_SR_OE         47  // 595 SR - OE
#define PIN_SR_SER        48  // 595 SR - SER

byte Outputs595_1    = 0b00000000;    // U7 cache
byte Outputs595_2    = 0b00000000;    // U8 cache
const byte TOTAL_RELAYS = 10;
byte currentRelay=10;

//  Send the current caches to both Shift Registers
void ShRegWrite () {                
    digitalWrite(PIN_SR_RCLK, LOW);         // freezes the 74HC595 output
    shiftOut(PIN_SR_SER, PIN_SR_SRCLK, MSBFIRST, Outputs595_2); // send cache for the second register (U8)
    shiftOut(PIN_SR_SER, PIN_SR_SRCLK, MSBFIRST, Outputs595_1); // send cache for the first register (U7)
    digitalWrite(PIN_SR_RCLK, HIGH);        // unfreezes the 74HC595 output
}


//  Modify the bit position in the Shift Register cache
void RegWrite (byte pos, byte stat) {
  if (pos<=7){          // positions 0-7 belong to the first register (U7)
    bitWrite(Outputs595_1, pos, stat);
  }
  else if (pos<=15) {  // positions 8-15 belong to the second register (U8)
    bitWrite(Outputs595_2, pos - 8, stat);
  }  
}

void setup()
 { 
  pinMode(PIN_SR_OE, OUTPUT);
  digitalWrite(PIN_SR_OE, HIGH);  // Keep relays off during boot
  pinMode(PIN_SR_SRCLK, OUTPUT);
  pinMode(PIN_SR_SER, OUTPUT); 
  pinMode(PIN_SR_RCLK, OUTPUT); 
  ShRegWrite();                   // Clear registers by writing initial zeros
  digitalWrite(PIN_SR_OE, LOW);   // Enable SR after init
}

void loop()  { 
    // 1. Turn off the previous relay in the cache
    RegWrite(currentRelay, 0);

    // 2. Move to the next relay
    currentRelay++;
    if (currentRelay >= TOTAL_RELAYS) currentRelay = 0;

    // 3. Turn on the new relay in the cache
    RegWrite(currentRelay, 1);

    // 4. Push the changes to the hardware
    ShRegWrite();

    delay(1000);
}
