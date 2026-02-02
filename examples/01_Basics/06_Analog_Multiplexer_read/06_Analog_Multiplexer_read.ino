/*
  Name: Analog_Multiplexer_read
  Board: esp32 -> ESP32S3 Dev Module / DevKit
  Hardware: ayatec sensoraya v2.1

  Description:
  Continuously gathers analog readings I1-I8 from Analog Multiplexer
  using 74HC595 Shift register and dumps them into serial port.

  Pin usage:
  - IO21, IO45, IO47, IO48: Shift register control pins (SRCLK, RCLK, OE, and SER, respectively, see sensoraya v2.1 schematic)
  - IO8: Analog input pin

  Created: 2026-02-02
  Author: ayatec (Michal Liptak)
  License: MIT
*/

#define PIN_ANALOG        8 
#define PIN_SR_SRCLK      21  // 595 SR - SRCLK
#define PIN_SR_RCLK       45  // 595 SR - RCLK
#define PIN_SR_OE         47  // 595 SR - OE
#define PIN_SR_SER        48  // 595 SR - SER

byte Outputs595_1    = 0b00000000;       // U7 cache
byte Outputs595_2    = 0b00000000;       // U8 cache
const uint8_t MULTIPL_INPUTS  = 8;       // number of multiplexer positions
int multi_cache[MULTIPL_INPUTS] = {0,0,0,0,0,0,0,0};  // analog multiplexer cache for the current readings
uint8_t multi_pos = 0;                   // multiplexer position cycle counter

//  Send the current caches to both Shift Registers
void ShRegWrite () {  
    multi_cache[multi_pos]=analogRead(PIN_ANALOG);  // saves reading on current position to cache
    multi_pos++;                                    // move to the next multiplexer position
    if(multi_pos >= MULTIPL_INPUTS) multi_pos = 0; 
    // write the multiplexer position to the last 3 bits of the second Shift Register
    bitWrite(Outputs595_2, 4, bitRead(multi_pos,0));
    bitWrite(Outputs595_2, 5, bitRead(multi_pos,1));
    bitWrite(Outputs595_2, 6, bitRead(multi_pos,2));  

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
  Serial.begin(115200);
  pinMode(PIN_SR_OE, OUTPUT);
  digitalWrite(PIN_SR_OE, HIGH);  // Keep relays off during boot
  pinMode(PIN_SR_SRCLK, OUTPUT);
  pinMode(PIN_SR_SER, OUTPUT); 
  pinMode(PIN_SR_RCLK, OUTPUT); 
  pinMode(PIN_ANALOG, INPUT);

  analogReadResolution(12);
  analogSetPinAttenuation(PIN_ANALOG, ADC_0db);   // Set 0 dB attenuation (0–1V range) for the analog input

  ShRegWrite();                   // Clear registers by writing initial zeros
  digitalWrite(PIN_SR_OE, LOW);   // Enable SR after init
}

void loop()  { 
  // 1. Read inputs
  for(uint8_t i=0; i < MULTIPL_INPUTS; i++){
    ShRegWrite();
    delay(5);
  }
  // 2. Dump readings to serial
  Serial.printf("Readings: I1: %d, I2: %d, I3: %d, I4: %d, I5: %d, I6: %d, I7: %d, I8: %d\n", multi_cache[6], multi_cache[5], multi_cache[4], multi_cache[3], multi_cache[2], multi_cache[1], multi_cache[0], multi_cache[7]);
  delay(1000); 
}
