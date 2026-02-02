/*
  Name: MOSFET_Input_Read
  Board: esp32 -> ESP32S3 Dev Module / DevKit
  Hardware: ayatec sensoraya v2.1

  Description:
  Reads the 10 digital input pins (IO1–IO6, IO9–IO13) corresponding
  to the sensoraya inputs (D1–D5, O1–O5) every 200ms and prints
  their states to the Serial monitor. All inputs return normally HIGH.
  The D-pins need to be connected to GND to read LOW.
  The O-pins need to be connected to 5V to read LOW (Common ground on J6 connector needs 
  to be connected to the board's GND, see sensoraya v2.1 schematic). 

  Pin usage:
  - Inputs: IO1, IO2, IO4, IO5, IO6 (D1–D5 - Direct GPIO pins, see sensoraya v2.1 schematic)
            IO9, IO10, IO11, IO12, IO13 (O1–O5 - Optocoupler pins, see sensoraya v2.1 schematic)

  Created: 2026-02-02
  Author: ayatec (Michal Liptak)
  License: MIT
*/

// Input pins array
constexpr uint8_t INPUT_PINS[] = {1,2,4,5,6,9,10,11,12,13};
constexpr uint8_t NUM_INPUTS = 10;

void setup() {
  // Initialize Serial
  Serial.begin(115200);
  delay(100);
  Serial.println("Booting ayatec sensoraya v1.4 PCB functionality demo - Digital read");

  // Set pins as inputs
  for (uint8_t i=0; i<NUM_INPUTS; i++) {
    pinMode(INPUT_PINS[i], INPUT_PULLUP);
  }
}

void loop() {
  Serial.print("Digital read (D1-D5, O1-O5): ");

  // Read each pin and print its state
  for (uint8_t i=0; i<NUM_INPUTS; i++) {
    int state = digitalRead(INPUT_PINS[i]);
    Serial.print(state);
    if (i < NUM_INPUTS - 1) {
      Serial.print(", ");
    }
  }
  Serial.println();

  delay(200); // poll every 200ms
}
