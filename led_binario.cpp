#include <Arduino.h>

const uint8_t PINOS_LED[] = {26,25,33,32};

void setup()
{
  for(auto pin : PINOS_LED)
  {
    pinMode(pin, OUTPUT);
  }
}

void loop ()
{
  for (uint8_t i = 0; i < 16; i++)
  {
    for(uint8_t j = 0; j < 4; j++)
    {
      digitalWrite(PINOS_LED[j], ((i % (2 << j)) - (i % (1 << j))) > 0);
    }
    delay(1000);
  }
}
