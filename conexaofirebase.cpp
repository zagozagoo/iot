#include <Arduino.h>
#include <WiFi.h>
#include <FirebaseESP32.h>

const char* WIFI_SSID = "Redmi 9";
const char* WIFI_PW = "12345678";

const char* FB_HOST = "https://iiot-dta-default-rtdb.firebaseio.com";
const char* FB_KEY = "Ag5gJMhAnTWQgDVhegkDRF1uTjJfpMUDkXB8WBEa";

const uint8_t NTC1_PIN = 33;
const uint8_t NTC2_PIN = 32;
const uint8_t LDR_PIN = 35;

FirebaseData fbdo;
FirebaseAuth fbauth;
FirebaseConfig fbconfig;

float getTemp(uint16_t sensor){
  uint16_t input_data = analogRead(sensor);
  float voltage = input_data * 3.3 / 4095; //3.3 eh o 3v3 da placa, pode variar

  float res = 10000.0 * voltage / (3.3 - voltage); //10 000 eh o valor do resistor(cores)


//as proximas 4 linhas sao constantes pro ntc, sempre mantem:

  float a = -19.49123972;
  float b = 204.88328885;
  float c  = 208.78577114;

  float temp = a * log(res - b) + c;

  return temp;
}

float getLight(uint16_t sensor){
  uint16_t input_data = analogRead(sensor);
  float voltage = input_data * 3.3 / 4095;

  return voltage;
}

bool connectWiFi(const char* ssid, const char* pw)
{
  WiFi.mode(WIFI_STA);
  WiFi.disconnect();

  int qtde_wifi = WiFi.scanNetworks();

  if(qtde_wifi == 0)
  {
    return false;
  }

  WiFi.begin(ssid, pw);

  Serial.print("Conectando");
  int tentativas = 0;
  while(WiFi.status() != WL_CONNECTED)
  {
    tentativas++;
    Serial.print(".");
    delay(200);

    if(tentativas > 300)
    {
      Serial.print("Falha na conexao WiFi !");
      return false;
    }
  }

  Serial.print("Conectado com o IP: ");
  Serial.println(WiFi.localIP());

  return true;
}

void setup()
{
//pinmode: se for sensor ta enviando entao eh input, ja led,som, tela lcd eh output

  pinMode(NTC1_PIN, INPUT);
  pinMode(NTC2_PIN, INPUT);
  pinMode(LDR_PIN, INPUT);

  Serial.begin(115200);
  connectWiFi(WIFI_SSID, WIFI_PW);

  fbconfig.database_url = FB_HOST;
  fbconfig.signer.tokens.legacy_token = FB_KEY;
  fbdo.setBSSLBufferSize(4096, 1024);
  Firebase.reconnectWiFi(true);
  Firebase.begin(&fbconfig, &fbauth);
}

void loop()
{
  float temp1 = getTemp(NTC1_PIN);
  float temp2 = getTemp(NTC2_PIN);
  float light_level = getLight(LDR_PIN);

  FirebaseJson json;
  json.set("/subsys_08/temperature/ntc_1", temp1);
  json.set("/subsys_08/temperature/ntc_2", temp2);
  json.set("/subsys_08/light/ldr_1", light_level);

  bool status = Firebase.updateNode(fbdo, "iiot-dta", json);


  if(!status)
  {
    Serial.println(fbdo.errorReason().c_str());
  }

  delay(5000);
}
