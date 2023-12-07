#include <Arduino.h>
#include <WiFi.h>
#include <PubSubClient.h>
#include <FirebaseESP32.h>
#include <DHT.h>

const uint8_t PIN_LED = 25;

const uint8_t PIN_DHT = 26;
#define DHTTYPE DHT11
DHT dht(PIN_DHT, DHTTYPE);

const char* WIFI_SSID = "Redmi 9";
const char* WIFI_PW = "12345678";

const char* FB_HOST = "https://iiot-dta-default-rtdb.firebaseio.com";
const char* FB_KEY = "Ag5gJMhAnTWQgDVhegkDRF1uTjJfpMUDkXB8WBEa";

FirebaseData fbdo;
FirebaseAuth fbauth;
FirebaseConfig fbconfig;

const char* MQTT_BROKER = "test.mosquitto.org";
const int MQTT_PORT = 1883;
const char* MQTT_USERNAME = "";
const char* MQTT_PASSWORD = "";

long tempo = 0;
long tempo_led = 0;

WiFiClient wifi_client;
PubSubClient mqtt_client(wifi_client);

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

void callback(char* topic, byte* payload, unsigned int length)
{
  String message = "";
  for (int i = 0; i < length; i++)
  {
    message += (char)payload[i];
  }

  if(strcmp(topic, "iiot-dta/check") == 0)
  {
    if(message == "100 do Joao")
    {
      mqtt_client.publish("iiot-dta/check", "1 do Joao");
    }
    else if(message == "200")
    {
      tempo_led = millis();
      digitalWrite(PIN_LED, HIGH);
    }
  }

  if(strcmp(topic, "iiot-dta/request") == 0)
  {
    if(message == "100 do Joao")
    {
      FirebaseJson json;
      json.set("/temperature", dht.readTemperature());
      json.set("/humidity", dht.readHumidity());

      bool status = Firebase.updateNode(fbdo, "/challenge02/subsys_08", json);

      if(!status)
      {
        Serial.println(fbdo.errorReason().c_str());
      }
    }
  }
}

void setup()
{

  pinMode(PIN_LED, OUTPUT);
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

  mqtt_client.subscribe("iiot-dta/check");
  mqtt_client.subscribe("iiot-dta/request");

  fbconfig.database_url = FB_HOST;
  fbconfig.signer.tokens.legacy_token = FB_KEY;
  fbdo.setBSSLBufferSize(4096, 1024);
  Firebase.reconnectWiFi(true);
  Firebase.begin(&fbconfig, &fbauth);
}

void loop()
{
  if(millis() - tempo_led > 5000) {
    digitalWrite(PIN_LED, LOW);
    tempo_led == millis();
  }
  if(millis() - tempo > 30000) {
    mqtt_client.publish("testdta", "test do joao");

    FirebaseJson json;
    json.set("/temperature", dht.readTemperature());
    json.set("/humidity", dht.readHumidity());

    bool status = Firebase.updateNode(fbdo, "/challenge02/subsys_08", json);

    if(!status)
    {
      Serial.println(fbdo.errorReason().c_str());
    }

    tempo = millis();
  }

  mqtt_client.loop();

}
