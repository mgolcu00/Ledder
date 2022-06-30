#ifndef CONNECTION_HPP
#define CONNECTION_HPP
#include <WiFi.h>
#include <PubSubClient.h>
#include <Arduino_JSON.h>

// Wifi credentials
#define ssid "<WIFISSID>"
#define password "<PASSWORD>"

// MQTT credentials
#define mqtt_broker "172.20.10.2" // localhost ip
#define in_topic "main/esp32/in"
#define random_topic "main/esp32/in/random"
#define out_topic "main/esp32/out"
#define mqtt_username "username"
#define mqtt_password "password"
#define mqtt_port 1883

// clients
extern WiFiClient espClient;
extern PubSubClient client;

void connectToWifi();
void connectToMQTT();
// void callback(char *topic, byte *payload, unsigned int length);
void convertToJson(byte *payload, unsigned int length, JSONVar &json);

#endif