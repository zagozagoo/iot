#include <Arduino.h>
#include "DHT.h"

/*hardware Mapping */
const uint8_t PIN_INPUT = 26;

/*constants*/
#define DHTTYPE DHT11

/*variables*/
DHT dht(PIN_INPUT, DHTTYPE); //obejto dht do tipo dht

void setup()
{
  dht.begin();

  Serial.begin(115200);
}

void loop()
{
  float humidade = dht.readHumidity();
  float temperatura = dht.readTemperature();

  if (isnan(humidade) || isnan(temperatura))
  {
    Serial.println("Falha na leitura do sensor DHT");
    return;
  }
}   

Serial.printf("Humidade: %4.2f \nTemperatura: %7.2f\n\n", humidade, temperatura); //o 7 da o valor total da string pra ficar alinhada
