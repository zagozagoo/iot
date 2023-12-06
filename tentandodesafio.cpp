#include <Arduino.h>
#include "FirebaseESP32.h"
#include <WiFi.h>
#include "DHT.h"
#include <PubSubClient.h>

#define DHTTYPE DHT11

long tempo = 0;
long tempo_led = 0;


const char* WIFI_SSID = "Vivo-Internet-BF17";
const char* WIFI_PW = "78814222";

const uint8_t PIN_LED = 25;

const uint8_t PIN_INPUT = 26;

DHT dht(PIN_INPUT, DHTTYPE);

const char* MQTT_BROKER = "test.mosquitto.org";
const int MQTT_PORT = 1883;
const char* MQTT_USERNAME = "";
const char* MQTT_PASSWORD = "";

WiFiClient wifi_client;
PubSubClient mqtt_client(wifi_client);

const char* FB_HOST = "https://iiot-dta-default-rtdb.firebaseio.com"; //dados_gerais
const char* FB_KEY = "Ag5gJMhAnTWQgDVhegkDRF1uTjJfpMUDkXB8WBEa";

FirebaseData fbdo;
FirebaseAuth fbauth;
FirebaseConfig fbconfig;

bool connectWiFi(const char* ssid, const char* pw)
{
  WiFi.mode(WIFI_STA);
  WiFi.disconnect();

  int qtde_wifi = WiFi.scanNetworks();

  if(qtde_wifi == 0)
  {
    return false;
  }

  WiFi.begin(ssid, pw);

  Serial.print("Conectando");
  int tentativas = 0;
  while(WiFi.status() != WL_CONNECTED)
  {
    tentativas++;
    Serial.print(".");
    delay(200);

    if(tentativas > 300)
    {
      Serial.print("Falha na conexao WiFi !");
      return false;
    }
  }

  Serial.print("Conectado com o IP: ");
  Serial.println(WiFi.localIP());

  return true;
}


void callback(char* topic, byte* payload, unsigned int lenght)
{
    String message = "";
    for(int i = 0; i < lenght; i++)
    {
        message += (char)payload[i];
    }

    if (strcmp(topic, "iiot-dta/check") == 0)
    {
        if(message == "100zago")
        {
            mqtt_client.publish("iiot-dta/check", "1zago");
        }
        else if(message == "200zago")
        {
            tempo_led = millis();
            digitalWrite(PIN_LED, HIGH);
        }
    }

    if (strcmp(topic, "iiot-dta/request") == 0)
    {
        if (message == "100zago")
        {
            FirebaseJson json;
            json.set("/temperature", dht.readTemperature());
            json.set("/humidity", dht.readHumidity());

            bool status = Firebase.updateNode(fbdo, "/challenge02/subsys_07", json);
            if(!status)
            {
                Serial.println(fbdo.errorReason().c_str());
            }
        }
    }
}

void setup()
{
    dht.begin();
    Serial.begin(115200);
    connectWiFi(WIFI_SSID, WIFI_PW);

    mqtt_client.setServer(MQTT_BROKER, MQTT_PORT);
    mqtt_client.setCallback(callback);

    while (!mqtt_client.connected())
    {
        String client_id = "dta_esp32_";
        client_id += String(WiFi.macAddress());

        if(mqtt_client.connect(client_id.c_str(), MQTT_USERNAME, MQTT_PASSWORD))
        {
        Serial.println("Conexao MQTT bem sucedida");
        }
    }

    mqtt_client.subscribe("testdta");

    fbconfig.database_url = FB_HOST;
    fbconfig.signer.tokens.legacy_token = FB_KEY;
    fbdo.setBSSLBufferSize(4096, 1024);
    Firebase.reconnectWiFi(true);
    Firebase.begin(&fbconfig, &fbauth);
}

void loop()
{
    if (millis() - tempo_led > 5000)
    {
        tempo_led = millis();
    }
    if (millis() - tempo > 30000)
    {
        mqtt_client.publish("testdta", "test da zago");

        FirebaseJson json;
        json.set("/humidity", dht.readHumidity());
        json.set("/temperature", dht.readTemperature());

        bool status = Firebase.updateNode(fbdo, "/challenge02/subsys_07/", json);

        if(!status)
        {
            Serial.println(fbdo.errorReason().c_str());
        }
        tempo = millis();
    }

    mqtt_client.loop(); //adicionei
}
