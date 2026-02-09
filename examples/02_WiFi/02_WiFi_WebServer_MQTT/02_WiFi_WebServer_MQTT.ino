/*
  Name: WiFi WebServer + MQTT
  Board: esp32 -> ESP32S3 Dev Module / DevKit
  Hardware: ayatec sensoraya v2.1

  Description:
  Maintains connection to local network via WiFi.
  Answers to root HTTP requests.
  Maintains connection to the MQTT broker, publishing regular messages.

  Pin usage:
  - N/A

  Dependencies:
  - PubSubClient by Nick O'Leary

  Created: 2026-02-08
  Author: ayatec (Michal Liptak)
  License: MIT
*/

#include <WiFi.h>
#include <WiFiClient.h>
#include <WebServer.h>
#include <PubSubClient.h>

// Set your MQTT and WiFi connection details here:
#define WIFI_SSID         ""
#define WIFI_PASSWORD     ""
#define MQTT_USER         ""
#define MQTT_PASS         ""
#define MQTT_PORT         1883
IPAddress MQTT_SERVER_IP(192, 168, 1, 1);

#define MESSAGE           "Hello from sensoraya"

void mqttcallback(char* topic, byte* payload, unsigned int length) {
  // handle message arrived
}

WiFiClient  wifiClient;
PubSubClient    mqttClient(MQTT_SERVER_IP, MQTT_PORT, mqttcallback, wifiClient);
WebServer wifiServer(80);
unsigned long Ref2s, Ref10s, RefMQTT;  // non-blocking sketch timestamps

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

  WiFi.begin(WIFI_SSID, WIFI_PASSWORD);

  wifiServer.on("/", []() {  // return message on request
        Serial.print("[HTTP] ");
        Serial.println(MESSAGE);
        wifiServer.send(200, "text/plain", MESSAGE);
      });
  wifiServer.begin();

  Serial.println("Connecting to Wi-Fi...");
  while (WiFi.status() != WL_CONNECTED) {
        delay(500);
        Serial.print(".");
      } 
  Serial.print("\nWebserver available on ");
  Serial.println(WiFi.localIP());
}

void loop(){
if (millis() - Ref2s > 2000){ // maintain connections
      
      mqttConnectLoop();
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

wifiServer.handleClient();
mqttClient.loop();
delay(1);          // required in minimal example to avoid wdt reboot
}
