/*
  Name: Onboard_NTC
  Board: esp32 -> ESP32S3 Dev Module / DevKit
  Hardware: ayatec sensoraya v2.1

  Description:
  Sets the 74HC595 Shift register to X7 input of Analog multiplexer and continuously
  reads the temperature reading of the connected on-board NTC thermistor.

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

byte Outputs595_1    = 0b00000000;    // Bit cache for 74HC595D #1 (U7 as per 2.x schmatic), 00000000 = all relays off
byte Outputs595_2    = 0b01111000;    // Bit cache for 74HC595D #2 (U8 as per 2.x schmatic), 01110000 = targets the analog multiplexer to the on-board NTC with relays off

// Steinhart-Hart (Beta version) equation
float readNTCTemperature(int adcReading,float VadcMax = 3.3,float Rseries = 10000,float VntcTop = 0,float VntcBottom = 0,float R0 = 10000.0,float T0 = 25.0 + 273.15,float B = 3950.0){
    if (adcReading <= 0) adcReading = 1; // avoid divide by zero
    if (adcReading >= 4095) adcReading = 4094; // prevent overflow
    float Vadc = adcReading * VadcMax / 4095.0; // Convert ADC reading to voltage at ADC pin
    // Correct for voltage divider if top/bottom resistors provided
    float Vntc = Vadc;
    if (VntcTop > 0 && VntcBottom > 0) Vntc *= (VntcTop + VntcBottom) / VntcBottom;
    float Rntc = Rseries / ((VadcMax / Vntc) - 1.0); // Compute NTC resistance
    float T = 1.0 / ( (1.0 / T0) + (1.0 / B) * log(Rntc / R0) ); // Convert to temperature using Beta formula
    return T - 273.15; // convert Kelvin to Celsius
}

void setup()
 { 
  Serial.begin(115200);
  pinMode(PIN_SR_OE, OUTPUT);
  pinMode(PIN_SR_SRCLK, OUTPUT);
  pinMode(PIN_SR_SER, OUTPUT);
  pinMode(PIN_SR_RCLK, OUTPUT);
  pinMode(PIN_ANALOG, INPUT);
  digitalWrite(PIN_SR_OE, HIGH);  // disable SR at boot
  
  analogReadResolution(12);
  analogSetPinAttenuation(PIN_ANALOG, ADC_0db);   // Set 0 dB attenuation (0–1V range) for the analog input

  
  // send cache to the Shift Register
  digitalWrite(PIN_SR_RCLK, LOW);         // freezes the 74HC595 output
  shiftOut(PIN_SR_SER, PIN_SR_SRCLK, MSBFIRST, Outputs595_2); // send cache for the second register (U8)
  shiftOut(PIN_SR_SER, PIN_SR_SRCLK, MSBFIRST, Outputs595_1); // send cache for the first register (U7)
  digitalWrite(PIN_SR_RCLK, HIGH);        // unfreezes the 74HC595 output
  digitalWrite(PIN_SR_OE, LOW);           // enable SR after init

  Serial.println("Booting ayatec sensoraya v2.0 PCB functionality demo - onboard NTC");
}

void loop()  {    // read analog input and transform to °C
  int adc = analogRead(PIN_ANALOG);
  float tempC = readNTCTemperature(adc);
  Serial.printf("Temp: %.2f°C (Raw adc: %d)\n", tempC, adc);
  delay(500);
}
