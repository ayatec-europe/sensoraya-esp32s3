/*
  Name: Ethernet WebServer
  Board: esp32 -> ESP32S3 Dev Module / DevKit
  Hardware: ayatec sensoraya v2.1

  Description:
  Maintains connection to local network via W5500 Ethernet module.
  Answers to root HTTP requests.

  Pin usage:
  - W5500: IO35, IO36, IO37, IO38 (SCK, MOSI, MISO, and SS, respectively, see sensoraya v2.1 schematic)

  Dependencies:
  - EthernetWebServer by Khoi Hoang

  Created: 2026-02-08
  Author: ayatec (Michal Liptak)
  License: MIT
*/

#include <SPI.h>
#include <EthernetWebServer.h>

#define PIN_MISO          37  // W5500 MISO
#define PIN_MOSI          36  // W5500 MOSI
#define PIN_SCK           35  // W5500 SCK
#define PIN_SS            38  // W5500 SCS

#define MESSAGE "Hello from sensoraya"
EthernetWebServer ethernetServer(80);

unsigned long lastEthCheck;  // non-blocking sketch timestamp
boolean eth_cable_flag;
byte mac[] = {  0xDE, 0xED, 0xBA, 0xFE, 0xFE, 0xED };  // ethernet mac
IPAddress lastEthIP; 

void ETHEventLoop() {   // maintains connection to the local network via W5500 ethernet module
    if (Ethernet.hardwareStatus() == EthernetNoHardware) {
      Serial.println("Ethernet module not found!");
      eth_cable_flag = false;
    }

    if (Ethernet.linkStatus() == LinkON) {
      if (!eth_cable_flag) {
        Serial.println("Ethernet cable found!");
        eth_cable_flag = true;
      }

    } else {
      if (eth_cable_flag) {
        Serial.println("Ethernet cable disconnected!");
        eth_cable_flag = false;
      }
    }

    if (Ethernet.localIP() != lastEthIP) {
      if (Ethernet.localIP() == IPAddress(0, 0, 0, 0)) {
        Serial.println("Lost IP address!");
        eth_connected = false;
      } else {
        Serial.print("New IP Assigned: ");
        Serial.println(Ethernet.localIP());
        eth_connected = true;
      }
      lastEthIP = Ethernet.localIP();  // Update last known assigned IP address
    }
}

void setup(){
  Serial.begin(115200);
  delay(1000);

  SPI.begin(PIN_SCK, PIN_MISO, PIN_MOSI); // W5500 SCK, MISO, MOSI
  Ethernet.init(PIN_SS); // W5500 CS
  Ethernet.begin(mac);

  ethernetServer.on("/", []() {  // return message on request
        Serial.println(MESSAGE);
        ethernetServer.send(200, "text/plain", MESSAGE);
      });
  ethernetServer.begin();

  Serial.println("Webserver started.");
}

void loop(){
if (millis() - lastEthCheck > 2000){ // maintain connections
      ETHEventLoop();
      lastEthCheck = millis();
  }

ethernetServer.handleClient();
delay(1);          // required in minimal example to avoid wdt reboot
}
