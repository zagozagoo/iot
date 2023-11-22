#include "FirebaseESP32.h"
#include <WiFi.h>
#include <cmath>

#define WIFI_NOME "luiz"
#define WIFI_SENHA "s3nh4asa"

#define FB_HOST "https://prova-iot-tds3-default-rtdb.firebaseio.com/"
#define FB_AUTH "j5ZA4M6RrZYlQGl39EGA4ORegOmOiTXQxQIJTYk4"

const uint8_t PIN1_NTC10K = 26;
const uint8_t PIN2_NTC10K = 25;

const uint8_t BUTTON = 35;

const uint8_t LED1 = 18;
const uint8_t LED2 = 19;
const uint8_t LED3 = 21;
const uint8_t LED4 = 22;
const uint8_t LED5 = 23;

const uint8_t LEDS[] = {LED1, LED2, LED3, LED4, LED5}

FirebaseData firebaseData;
FirebaseJson json;

float voltage1 = 0;
float resistence1 = 0;
float temperature1 = 0;
float simplifier1 = 0;

float voltage2 = 0;
float resistence2 = 0;
float temperature2 = 0;
float simplifier2 = 0;

int critical_event = 0;

void setup() {
    pinMode(PIN1_NTC10K, INPUT);
    pinMode(PIN2_NTC10K, INPUT);

    pinMode(BUTTON, INPUT);

    for(int i = 0; i < 4; i++)
    {
        pinMode(LEDS[i], OUTPUT);
    }

    Serial.begin(115200);

    WiFi.begin(WIFI_NOME, WIFI_SENHA);

    while (WiFi.status() != WL_CONNECTED)
    {
        Serial.print(".");
        delay(100);
    }

    Firebase.begin(FB_HOST, FB_AUTH);
    Firebase.reconnectWiFi(true);
    Firebase.setReadTimeout(firebaseData, 6000);
    Firebase.setWriteSizeLimit(firebaseData, "tiny");
}

void loop() {
    // Temperatura PIN1_NTC10K
    voltage1 = static_cast<float>(analogRead(PIN1_NTC10K)) * 3.3 / 4095.;
    resistence1 = 1e4 / (3.3 - voltage1);
    simplifier1 = resistence1 - 1.29611833e+01;
    temperature1 = -7.61498101e-02 * log(simplifier1) * *3 + 3.40889111e+00 * log(simplifier1) * *2 + (-6.45957953e+01) * log(simplifier1) + 3.90218793e+02;

    // Temperatura PIN2_NTC10K
    voltage2 = static_cast<float>(analogRead(PIN2_NTC10K)) * 3.3 / 4095.;
    resistence2 = 1e4 / (3.3 - voltage2);
    simplifier2 = resistence2 - 1.29611833e+01;
    temperature2 = -7.61498101e-02 * log(simplifier2) * *3 + 3.40889111e+00 * log(simplifier2) * *2 + (-6.45957953e+01) * log(simplifier2) + 3.90218793e+02;

    float media_temp = (temperature1 + temperature2) / 2;
    float desvio_padrao = abs(temperature1 + temperature2) / sqrt(2);

    if(desvio_padrao > 1)
    {
        digitalWrite(LED4, HIGH);
        critical_event++;
    }
    else
        digitalWrite(LED4, LOW);

    // desculpa marcos eu sei que esses if's ficaram horrorosos
    if(media_temp >= 23 && media_temp < 25)
    {
        digitalWrite(LED1, HIGH);
        digitalWrite(LED2, LOW);
        digitalWrite(LED3, LOW);
    }

    else if(media_temp >= 25 && media_temp < 27)
    {
        digitalWrite(LED1, HIGH);
        digitalWrite(LED2, HIGH);
        digitalWrite(LED3, LOW);
    }

    else if(media_temp >= 27)
    {
        digitalWrite(LED1, HIGH);
        digitalWrite(LED2, HIGH);
        digitalWrite(LED3, HIGH);
    }

    else
    {
        digitalWrite(LED1, LOW);
        digitalWrite(LED2, LOW);
        digitalWrite(LED3, LOW);
    }

    json.set("/qtd_eventos", critical_event);
    json.set("/temp_media", media_temp);
    json.set("/temp_desvio", desvio_padrao);
    json.set("/sensores/sensor_1", temperature1);
    json.set("/sensores/sensor_2", temperature2);

    if(digitalRead(BUTTON) == HIGH)
    {
        delay(10);
        if(digitalRead(BUTTON) == LOW)
        {
            Serial.println("Enviando dados para o Firebase");
            digitalWrite(LED5, HIGH);
            Firebase.updateNode(firebaseData, "/avaliacao", json);
            digitalWrite(LED5, LOW);
        }
    }
    
    delay(1000);
}
