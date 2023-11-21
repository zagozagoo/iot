/*hardware Mapping */
const uint8_t PIN_INPUT = 26;

/*constants*/
const uint16_t ADC_MAX = 4095;
const float VCC = 3.3;

void setup()
{
  pinMode(PIN_INPUT, INPUT);

  Serial.begin(115200);
}

void loop()
{
  int input = analogRrad(PIN_INPUT);
  float tensao = static_cast<float>(input) * VCC / ADC_MAX;

  //Serial.write(); printa caracter
  //Serial.print(tensao); printa o valor junto
  Serial.println(tensao); //pula linha
}
