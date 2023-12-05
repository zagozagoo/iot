/*---------------------------------------------------------------------------*\

    E ngineering | Bosch ETS: Engineering Technical School
    T echnical   | IIoT Training: Industrial Internet of Things
    S chool      | Powertrain Solutions @ Bosch Brazil

\*---------------------------------------------------------------------------*/

/*--- Includes ---*/
#include <Arduino.h>
#include <PubSubClient.h>
#include <WiFi.h>

/*--- Constants ---*/
const char* WIFI_SSID = "WIFI_SSID";
const char* WIFI_PASSWORD = "WIFI_PASSWORD";

const char* MQTT_BROKER = "MQTT_BROKER";
const char* MQTT_USERNAME = "MQTT_USERNAME";
const char* MQTT_PASSWORD = "MQTT_PASSWORD";
const int MQTT_PORT = 0000;

/*--- Variables ---*/
WiFiClient esp_client;
PubSubClient mqtt_client(esp_client);

/*--- Functions ---*/
void connectWifi(const char ssid[], const char pass[])
{
    WiFi.begin(ssid, pass);

    if (Serial) { Serial.print("Connecting"); }
    while (WiFiClass::status() != WL_CONNECTED)
    {
        if (Serial)
        {
            Serial.write('.');
            delay(500);
        }
    }

    if (Serial)
    {
        Serial.print("Connected with IP: ");
        Serial.println(WiFi.localIP());
    }
}

void callback(char topic[], byte payload[], uint16_t length)
{
    Serial.printf("Message arrived in topic: %s\n", topic);

    for (uint16_t i = 0; i < length; i++)
    {
        Serial.print(static_cast<char>(payload[i]));
    }
    Serial.print("\n\n");
}

/*--- Setup ---*/
void setup()
{
    // Serial communication
    Serial.begin(115200);

    // WiFi
    connectWifi(WIFI_SSID, WIFI_PASSWORD);

    // MQTT
    mqtt_client.setServer(MQTT_BROKER, MQTT_PORT);
    mqtt_client.setCallback(callback);
    String mqtt_client_id = "esp32-client-" + String(WiFi.macAddress());
    while (!mqtt_client.connected())
    {
        Serial.printf("The client %s connects to the MQTT broker\n",
                      mqtt_client_id.c_str());

        if (mqtt_client.connect(mqtt_client_id.c_str(), MQTT_USERNAME, MQTT_PASSWORD))
        {
            Serial.println("MQTT broker connected");
        }
        else
        {
            Serial.print("Failed with state ");
            Serial.print(mqtt_client.state());
            delay(2000);
        }
    }

    mqtt_client.publish("iiot-dta/request", "Hello, world!");
    mqtt_client.subscribe("iiot-dta/request");
}

/*--- Loop ---*/
void loop()
{
    mqtt_client.loop();
}
