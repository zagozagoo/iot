const uint8_t PIN_BUTTON = 19;
const uint8_t PIN_LED = 26;

bool led_state = LOW;
bool botao_anterior = LOW;
bool pressionado = LOW;
long tempo = 0;

void setup()
{
  pinMode(PIN_BUTTON, INPUT);
  pinMode(PIN_LED, OUTPUT);
}

void loop()
{
  botao_anterior = pressionado;
  pressionado = digitalRead(PIN_BUTTON);

  if (pressionado && !botao_anterior && (millis() - tempo > 50)) //Millis eh o tempo atual                                          
  {
    led_state = !led_state; //assim inverte o proprio estado p ativar e desativar
    digitalWrite(PIN_LED,led_state);
    tempo = millis();
  }
 }
