#include <Arduino.h>
#include <WiFi.h>
#include <FirebaseESP32.h>

const char* WIFI_SSID = "Redmi 9";
const char* WIFI_PW = "12345678";

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

void setup()
{
  Serial.begin(115200);
  connectWiFi(WIFI_SSID, WIFI_PW);
}

void loop()
{
  delay(5000);
}
