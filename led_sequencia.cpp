#include <Arduino.h>

const uint8_t PIN_INPUT = 15;
const uint8_t PIN_LED[] = {25, 26, 27};

const uint32_t ADC_MAX = 4095;
const float VREF = 3.3f;

void setup()
{
  for(int i = 0; i < 3; i++)
  {
    pinMode(PIN_LED[i], OUTPUT);
  }
  pinMode(PIN_INPUT, INPUT);

  Serial.begin(115200);
}

void loop(){

  uint16_t input_value = analogRead(PIN_INPUT);

  float voltage = input_value * VREF / ADC_MAX;

  Serial.println(voltage);

  if(voltage >= 1 && voltage < 2)
  {
    digitalWrite(PIN_LED[0], HIGH);
    digitalWrite(PIN_LED[1], LOW);
    digitalWrite(PIN_LED[2], LOW);
  }
  else if(voltage >= 2 && voltage < 3)
  {
    digitalWrite(PIN_LED[0], HIGH);
    digitalWrite(PIN_LED[1], HIGH);
    digitalWrite(PIN_LED[2], LOW);
  }
  else if(voltage >= 3)
  {
    digitalWrite(PIN_LED[0], HIGH);
    digitalWrite(PIN_LED[1], HIGH);
    digitalWrite(PIN_LED[2], HIGH);
  } else
  {
    digitalWrite(PIN_LED[0], LOW);
    digitalWrite(PIN_LED[1], LOW);
    digitalWrite(PIN_LED[2], LOW);
  }
}
