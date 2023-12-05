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

  Serial.printf("Humidade: %4.2f \nTemperatura: %7.2f\n\n", humidade, temperatura); //o 7 da o valor total da string pra ficar alinhada

  if (isnan(humidade) || isnan(temperatura))
  {
    Serial.println("Falha na leitura do sensor DHT");
    return;
  }
}   
################################

#include "DHT.h"

const uint8_t PIN_INPUT = 26;

/*---Constants---*/
#define DHTTYPE DHT11

/*---Variable---*/
const uint16_t ADC_MAX = 4095;
const float VCC = 3.3f;
DHT dht(PIN_INPUT, DHTTYPE);

void setup()
{
  dht.begin();
  Serial.begin(115200);
}

void loop() 
{
  float humidity = dht.readHumidity();
  float temperature = dht.readTemperature();

  if (isnan(humidity) || isnan(temperature))
  {
    Serial.println("Failed to read the sensor DHT");
    return;
  }

  Serial.printf("Humidity: %7.2f \nTemperature: %4.2f\n\n", humidity, temperature);

  delay(2000);
}

