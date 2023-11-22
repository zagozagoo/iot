/*hardware Mapping */
const uint8_t PIN_INPUT = 26;

/*constants*/
const uint16_t ADC_MAX = 4095;
const float VREF = 3.3f;

void setup()
{
  pinMode(PIN_INPUT, INPUT);

  Serial.begin(115200);
}

void loop()
{
  uint16_t input = analogRead(PIN_INPUT);
  float tensao = input * VREF / ADC_MAX;

  //Serial.write(); printa caracter
  //Serial.print(tensao); printa o valor junto
  Serial.println(tensao); //pula linha
}
