
#include "connection.h"

WiFiClient espClient;
PubSubClient client(espClient);

void connectToWifi()
{
    Serial.println("Connecting to WiFi...");
    WiFi.begin(ssid, password);
    while (WiFi.status() != WL_CONNECTED)
    {
        delay(500);
        Serial.print(".");
    }
    Serial.println("");
    Serial.println("WiFi connected");
    Serial.println("IP address: ");
    Serial.println(WiFi.localIP());
}

void connectToMQTT()
{
    Serial.println("Connecting to MQTT...");
    client.setServer(mqtt_broker, mqtt_port);
    // client.setCallback(callback);
    String client_id = "esp32-";
    client_id += String(WiFi.localIP());
    client_id += "-";
    client_id += String(WiFi.macAddress());
    while (!client.connected())
    {

        Serial.print(".");
        delay(1000);
        if (client.connect(client_id.c_str(), mqtt_username, mqtt_password))
        {
            Serial.println("Local MQTT broker connected");
        }
        else
        {
            Serial.print("failed with state : ");
            Serial.println(client.state());
            delay(2000);
        }
    }
    client.subscribe(in_topic);
    client.subscribe(out_topic);
}

// void callback(char *topic, byte *payload, unsigned int length)
// {
//     Serial.print("Message arrived in topic: ");
//     Serial.println(topic);
//     Serial.print("Message:");
//     for (int i = 0; i < length; i++)
//     {
//         Serial.print((char)payload[i]);
//     }
//     JSONVar json;
//     convertToJson(payload, length, json);
//     String text = (const char *)json["text"];
//     int r = json["r"];
//     int g = json["g"];
//     int b = json["b"];
//     Serial.println();
//     Serial.println("-----------------------");
//     int *color = new int[3];
//     color[0] = r;
//     color[1] = g;
//     color[2] = b;
// }

void convertToJson(byte *data, unsigned int l, JSONVar &json)
{
    char buffer[l + 1];
    for (int i = 0; i < l; i++)
    {
        buffer[i] = (char)data[i];
    }
    buffer[l] = '\0';
    json = JSON.parse(buffer);
}
