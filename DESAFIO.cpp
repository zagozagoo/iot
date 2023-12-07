/*--- Includes ---*/
#include "DHT.h"
#include "FirebaseESP32.h"
#include "PubSubClient.h"
#include <WiFi.h>

/*--- Hardware Mapping ---*/
const uint8_t PIN_DHT = 26;
const uint8_t PIN_LED = 25;

/*--- Constants ---*/
#define DHT_TYPE DHT11

const char WIFI_SSID[] = "belisarius";
const char WIFI_PASSWORD[] = "senhaaaa";

const char FB_HOST[] = "https://iiot-dta-default-rtdb.firebaseio.com";
const char FB_SECRET[] = "Ag5gJMhAnTWQgDVhegkDRF1uTjJfpMUDkXB8WBEa";

const char MQTT_BROKER[] = "test.mosquitto.org";
const int MQTT_PORT = 1883;
const char MQTT_USERNAME[] = "";
const char MQTT_PASSWORD[] = "";

/*--- Variables ---*/
DHT dht(PIN_DHT, DHT_TYPE);

FirebaseData fbdo;
FirebaseAuth fbauth;
FirebaseConfig fbconfig;

WiFiClient wifi_client;
PubSubClient mqtt_client(wifi_client);

uint32_t fbtime = 0;
uint32_t ledtime = 0;

float temperature;
float humidity;

/*--- Functions ---*/
void connectWiFi(const char* ssid, const char* password)
{
    Serial.print("Conectando");
    WiFi.begin(ssid, password);

    while(WiFi.status() != WL_CONNECTED)
    {
        Serial.write('.');
        delay(500);
    }

    Serial.print("\nConectado com o IP: ");
    Serial.println(WiFi.localIP());
}

void connectMQTT(const char* broker, const int port)
{
    mqtt_client.setServer(broker, port);
    mqtt_client.setCallback(callback);
    String mqtt_client_id = "esp32-client-" + String(WiFi.macAddress());
    while (!mqtt_client.connected())
    {
        Serial.printf("The client %s connects to the MQTT broker\n",
                      mqtt_client_id.c_str());

        if (mqtt_client.connect(mqtt_client_id.c_str(), "", ""))
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
}

void sendData()
{
    FirebaseJson json;
    json.set("temperature", temperature);
    json.set("humidity", humidity);

    bool status = Firebase.updateNode(fbdo, "/challenge02/subsys_00", json);
    if (!status)
    {
        Serial.print("Falha ao enviar ao Firebase: ");
        Serial.println(fbdo.errorReason().c_str());
    }
}

void callback(const char* topic, const byte* payload, const uint16_t lenght)
{
    char message[lenght + 1];
    message[lenght] = '\0';
    for (uint16_t i = 0; i < lenght; i++)
    {
        message[i] = static_cast<char>(payload[i]);
    }

    Serial.printf("Mensagem recebida no topico %s\n", topic);

    if (strcmp(topic, "iiot-dta/check") == 0)
    {
        if (strcmp(message, "100") == 0)
        {
            mqtt_client.publish("iiot-dta/check", "1");
        }
        else if (strcmp(message, "200") == 0)
        {
            ledtime = millis();
            digitalWrite(PIN_LED, HIGH);
        }
    }
    else if (strcmp(topic, "iiot-dta/request") == 0)
    {
        if (strcmp(message, "100") == 0)
        {
            sendData();
        }
    }
}

void setup()
{
    // Hardware mapping
    pinMode(PIN_LED, OUTPUT);

    // Serial
    Serial.begin(115200);

    // DHT
    dht.begin();

    // WiFi
    connectWiFi(WIFI_SSID, WIFI_PASSWORD);

    // Firebase
    fbconfig.database_url = FB_HOST;
    fbconfig.signer.tokens.legacy_token = FB_SECRET;
    fbdo.setBSSLBufferSize(4096, 1024);
    Firebase.reconnectWiFi(true);
    Firebase.begin(&fbconfig, &fbauth);

    // MQTT
    connectMQTT(MQTT_BROKER, MQTT_PORT);
    mqtt_client.subscribe("iiot-dta/check");
    mqtt_client.subscribe("iiot-dta/request");
}

void loop()
{
    temperature = dht.readTemperature();
    humidity = dht.readHumidity();

    if (isnan(temperature) || isnan(humidity))
    {
        Serial.println("Falha ao ler o sensor DHT");
        return;
    }

    if (millis() - fbtime > 30000)
    {
        fbtime = millis();
        sendData();
    }

    if (millis() - ledtime > 5000)
    {
        digitalWrite(PIN_LED, LOW);
    }

    if (temperature > 30.0f)
    {
        mqtt_client.publish("iiot-dta/request", "10");
    }
