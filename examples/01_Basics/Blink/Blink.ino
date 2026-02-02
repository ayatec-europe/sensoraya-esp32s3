/*
  Name: Blink
  Board: esp32 -> ESP32S3 Dev Module / DevKit
  Hardware: ayatec sensoraya v2.1

  Description:
  Blinks the on-board P2 MOSFET status LED connected to IO16.
  The LED turns ON for 1 second and OFF for 1 second repeatedly.

  Pin usage:
  - IO16: MOSFET P2 (D10 diode, see sensoraya v2.1 schematic)

  Created 2026-02-02
  Author: ayatec [Michal Liptak](mliptak@ayatec.eu)
  License: MIT
*/

constexpr uint8_t LED_P2_PIN = 16;

void setup(){ 
  pinMode(LED_P2_PIN, OUTPUT);
}

void loop(){ 
  digitalWrite(LED_P2_PIN, HIGH);
  delay(1000);
  digitalWrite(LED_P2_PIN, LOW);
  delay(1000);
}
