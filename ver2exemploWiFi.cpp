#include <WiFi.h>
#include <Arduino.h>

const char* WIFI_SSID = "nomedarede";
const char* WIFI_PW = "senha";

const char* = FB_HOST = "https://iotzago-default-rtdb.firebaseio.com/"
const char* FB_KEY = "DQf3mxx66s9voQFO3AKqIRpmeB2cH7qm01PBU3X6" //firebase

bool connectWiFi(const char* ssid, const char* pw)
{
    WiFi.mode(WIFI_STA);
    WiFi.disconnect(); //garante que a esp nao ta conectada em outro wifi
    
    int qtde_wifi = WiFi.scanNetworks();

    if(qtde_wifi == 0) //caso nao tenha nenhum wifi ele nao faz nada
    {
            return;
    }
   
    WiFi.begin(WIFI_SSID, WIFI_PW);

    Serial.print("Conectando");
    int tentativas = 0;
    while(WiFi.status() != WL_CONNECTED) //enquanto nao estiver conectado ele continua tentando
        {
            tentativa++;

            if (tentativa > 300)
            {
                return false;
            }
            
            Serial.print(".");
            delay(300);
        }
    
    Serial.println(Conectando com o IP: );

    Serial.println(WiFi.localIP());
}

void setup()
{
    Serial.begin(115200);
    connectWiFi(WIFI_SSID, WIFI_PW);
}

void loop()
{
    Realtime Database
}
