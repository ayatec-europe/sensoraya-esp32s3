/*
  Name: Ethernet WebServer MQTT
  Board: esp32 -> ESP32S3 Dev Module / DevKit
  Hardware: ayatec sensoraya v2.1

  Description:
  Maintains connection to local network via W5500 Ethernet module.
  Maintains webserver, responding to root HTTP requests.
  Maintains connection to the MQTT broker, publishing regular messages.

  Pin usage:
  - W5500: IO35, IO36, IO37, IO38 (SCK, MOSI, MISO, and SS, respectively, see sensoraya v2.1 schematic)

  Dependencies:
  - EthernetWebServer by Khoi Hoang
  - PubSubClient by Nick O'Leary

  Created: 2026-02-08
  Author: ayatec (Michal Liptak)
  License: MIT
*/

#include <SPI.h>
#include <EthernetWebServer.h>
#include <PubSubClient.h>

void mqttcallback(char* topic, byte* payload, unsigned int length) {
  // handle message arrived
}

#define PIN_MISO          37  // W5500 MISO
#define PIN_MOSI          36  // W5500 MOSI
#define PIN_SCK           35  // W5500 SCK
#define PIN_SS            38  // W5500 SCS


// Set your MQTT connection details here:
#define MQTT_USER         ""
#define MQTT_PASS         ""
#define MQTT_PORT         1883
IPAddress MQTT_SERVER_IP(192, 168, 1, 1);


#define MESSAGE           "Hello from sensoraya"

EthernetClient  ethClient;
PubSubClient    mqttClient(MQTT_SERVER_IP, MQTT_PORT, mqttcallback, ethClient);
EthernetWebServer ethernetServer(80);
unsigned long Ref2s, Ref10s, RefMQTT;  // non-blocking sketch timestamps
boolean eth_cable_flag, eth_connected;
byte mac[] = {  0xDE, 0xED, 0xBA, 0xFE, 0xFE, 0xED };  // ethernet mac
IPAddress lastEthIP; 

void ETHEventLoop() {   // maintains connection to the local network via W5500 ethernet module
    if (Ethernet.hardwareStatus() == EthernetNoHardware) {
      Serial.println("Ethernet module not found!");
      eth_connected = false;
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
        eth_connected = false;
      }
    }

    if (Ethernet.localIP() != lastEthIP) {
      Serial.print("IP address: ");
      Serial.println(Ethernet.localIP());
      lastEthIP = Ethernet.localIP();
    }
}



void mqttConnectLoop() {    // try to reconnect regularly if not connected 
 if (!mqttClient.connected() && (millis() - RefMQTT >= 10000UL)) {         
    if (mqttClient.connect("sensoraya", MQTT_USER, MQTT_PASS)){
      Serial.println("Connected to MQTT broker!");
      }
    else
    {
      Serial.print("...failed, rc=");
      Serial.print(mqttClient.state());
      Serial.println(" try again in 10 seconds");
    }
    RefMQTT=millis();
    } 
}

void setup(){
  Serial.begin(115200);
  delay(1000);
  SPI.begin(PIN_SCK, PIN_MISO, PIN_MOSI); // W5500 SCK, MISO, MOSI
  Ethernet.init(PIN_SS); // W5500 CS
  Ethernet.begin(mac);
  delay(200);
  ethernetServer.on("/", []() {  // return message on request
        Serial.print("[HTTP] ");
        Serial.println(MESSAGE);
        ethernetServer.send(200, "text/plain", MESSAGE);
      });
  ethernetServer.begin();
  Serial.println("\n\nWebserver started.");
}

void loop(){
if (millis() - Ref2s > 2000){ // maintain connections
      ETHEventLoop();
      if(eth_connected) mqttConnectLoop();
      Ref2s = millis();
  }
if (millis() - Ref10s > 10000){  // publish message every 10 seconds
      if(mqttClient.connected()){
        mqttClient.publish("sensoraya/test", MESSAGE, false);
        Serial.print("[MQTT] ");
        Serial.println(MESSAGE);
      }
      Ref10s = millis();
  }

ethernetServer.handleClient();
mqttClient.loop();
delay(1);          // required in minimal example to avoid wdt reboot
}
