#include <Arduino.h>
#include <WiFi.h>
#include <PubSubClient.h>

const char* WIFI_SSID = "belisarius";
const char* WIFI_PW = "senhaaaa";

const char* MQTT_BROKER = "test.mosquitto.org";
const int MQTT_PORT = 1883;
const char* MQTT_USERNAME = "";
const char* MQTT_PASSWORD = "";

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
  Serial.printf("Menasgem recebida no topico %s: ", topic);
  for (int i = 0; i < length; i++)
  {
    Serial.print((char)payload[i]);
  }

  Serial.println();
}

void setup()
{

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
}

void loop()
{
  mqtt_client.publish("testdta", "test do joao");
  delay(5000);
  
}
