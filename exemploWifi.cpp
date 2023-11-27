#include <WiFi.h>
#include <Arduino.h>

void setup()
{
    Serial.begin(115200);

    Wifi.mode(WIFI_STA);
    Wifi.disconnect(); //garante que a esp nao ta conectada em outro wifi
}

void loop()
{
    int qtde_wifi = Wifi.scanNetworks();

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
        Serial.print(Wifi.SSID(i));  //Nome da rede
        Serial.print(" RSSI: ");
        Serial.print(Wifi.RSSI(i)); //intensidade do sinal
        Serial.print(" Auth: ");
        Serial.print(Wifi.encryptionType(i));
        Serial.println;

    }

    delay(5000);
}
