#include <WiFi.h>
#include <Arduino.h>

void setup()
{
    Serial.begin(115200);

    WiFi.mode(WIFI_STA);
    WiFi.disconnect(); //garante que a esp nao ta conectada em outro wifi

    WiFi.begin("ssid", "senhas");
}

void loop()
{
    int qtde_wifi = WiFi.scanNetworks();

    if(qtde_wifi == 0) //caso nao tenha nenhum wifi ele nao faz nada
    {
            return;
    }
    for (int i = 0; i i < qtde_wifi; i++)
    {
        Serial.printf("Quantidade de redes disponiveis:  %i \n", qtde_wifi)
        Serial.print("Indice: ");
        Serial.print(i + 1);
        Serial.print(" SSID: ");
        Serial.print(WiFi.SSID(i));  //Nome da rede
        Serial.print(" RSSI: ");
        Serial.print(WiFi.RSSI(i)); //intensidade do sinal
        Serial.print(" Auth: ");
        Serial.print(WiFi.encryptionType(i));
        Serial.println;

    }

    delay(5000);
}
