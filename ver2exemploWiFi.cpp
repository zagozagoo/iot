#include <WiFi.h>
#include <Arduino.h>

const char* WIFI_SSID = "nomedarede";
const char* WIFI_PW = "senha";

void setup()
{
    Serial.begin(115200);

    WiFi.mode(WIFI_STA);
    WiFi.disconnect(); //garante que a esp nao ta conectada em outro wifi
    
    int qtde_wifi = WiFi.scanNetworks();

    if(qtde_wifi == 0) //caso nao tenha nenhum wifi ele nao faz nada
    {
            return;
    }
   
    WiFi.begin(WIFI_SSID, WIFI_PW);

    Serial.print("Conectando")
    while(WiFi.status() != WL_CONNECTED) //enquanto nao estiver conectado ele continua tentando
        {
            Serial.print(".");
            delay(300);
        }
}

void loop()
{
    delay(5000);
}
