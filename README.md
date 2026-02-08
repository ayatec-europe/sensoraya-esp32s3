# ayatec sensoraya v2.x (ESP32-S3) Arduino IDE Examples

Official Arduino IDE example sketches for ayatec sensoraya v2.x programmable I/O boards (based on ESP32-S3).
ayatec sensoraya v2.x is a family of programmable IoT I/O boards with Wi-Fi and W5500 Ethernet based on Espressif ESP32-S3, 
developed by ayatec europe and intended for use in bespoke embedded systems.

## Supported Hardware
- ayatec sensoraya v2.x

## Requirements
- Arduino IDE
- ESP32 core by Espressif
- relevant basic libraries where applicable

## Getting Started
1. Install Arduino IDE
2. Install ESP32 boards package
3. Install ayatec_sensoraya_2_demo library
4. Choose ESP32S3 Dev Module board
5. Open File → Examples → ayatec_sensoraya_2_demo library → 01_Basics → 01_Blink
   
Recommended board set-up:
- Board: ESP32S3 Dev Module
- USB CDC On Boot: Disabled
- CPU frequency: 240MHz
- Events Run on: Core 1
- Arduino Runs on: Core 0
- Flash size: 4MB
- Flash mode: QIO
- Partition scheme: Minimal SPIFFS (1.9MB APP / 1.9MB OTA / 190kB FS)
- Erase all flash: Disabled
- PSRAM: Disabled
- JTAG Adapter: Disabled


## Repository Structure
- examples/ – Arduino example sketches
- docs/ – pinouts, schematics, guides

## License
MIT License
