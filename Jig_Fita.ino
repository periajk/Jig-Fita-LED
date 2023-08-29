/* 
JIG DE TESTE UNICO - VALORES:
p/ Jig 1: 
  if (digitalRead(switchNumLeds) == HIGH) {
   virtualNumLed = 25;

   VERDE_TOTAL = 2.14;
   VERMELHO_TOTAL = 2.15;
   AZUL_TOTAL = 2.14;
  }
  else{
    virtualNumLed = NUM_LEDS;

    VERDE_TOTAL = 4.10;
    VERMELHO_TOTAL = 4.10;
    AZUL_TOTAL = 4.10;
  }
  ===================
p/ Jig 2:

*/


#include <FastLED.h>

#define DATA_PIN 5
#define NUM_LEDS 50
#define BRIGHTNESS 255
#define LED_TYPE WS2811
#define COLOR_ORDER BGR

int k = 0;
const int analogInPin = A0;
float sensorValue = 0;
float outputValue = 0;
int BotaoPin = 2;
int botao;
int switchSelector;
int virtualNumLed;
float CorrenteFinal = 0;

int LedVerm = 10;
int LedVerd = 11;
int LedAz = 12;

int LedPinErro = 8;
int LedPinSucess = 9;
int buzzer = 7;  //pin 7
int switchNumLeds = 6;


float correnteVerd1 = 0;
float correnteVerm1 = 0;
float correnteAz1 = 0;

float correnteVerd2 = 0;
float correnteVerm2 = 0;
float correnteAz2 = 0;


float ResistShunt = 0.9;
float ganhoAmpop = 7.66;


float V1 = 0;
float V2 = 0;

float VERDE_TOTAL = 4.10;
float VERMELHO_TOTAL = 4.10;
float AZUL_TOTAL = 4.10;


int LED_COUNT = 0;
int error_count = 0;

CRGB leds[NUM_LEDS];

#define UPDATES_PER_SECOND 100

void setup() {
  FastLED.addLeds<LED_TYPE, DATA_PIN, COLOR_ORDER>(leds, NUM_LEDS);
  FastLED.setBrightness(BRIGHTNESS);
  Serial.begin(9600);
  pinMode(BotaoPin, INPUT_PULLUP);
  pinMode(switchNumLeds, INPUT_PULLUP);

  pinMode(LedVerm, OUTPUT);
  pinMode(LedVerd, OUTPUT);
  pinMode(LedAz, OUTPUT);

  pinMode(LedPinErro, OUTPUT);
  pinMode(LedPinSucess, OUTPUT);
  pinMode(buzzer, OUTPUT);

  switchSelector = digitalRead(switchNumLeds);

  virtualNumLed = NUM_LEDS;
}


void loop() {

  if (digitalRead(switchNumLeds) == HIGH) {
   virtualNumLed = 25;

   VERDE_TOTAL = 2.67;
   VERMELHO_TOTAL = 2.67;
   AZUL_TOTAL = 2.67;
  }
  else{
    virtualNumLed = NUM_LEDS;

    VERDE_TOTAL = 4.35;
    VERMELHO_TOTAL = 4.35;
    AZUL_TOTAL = 4.35;
  }

  if (k == 0) {
    apagar();  // Para garantir LEDs apagados no início
    digitalWrite(LedVerm, HIGH);
    digitalWrite(LedVerd, HIGH);
    digitalWrite(LedAz, HIGH);
    FastLED.show();
    LED_COUNT = virtualNumLed;
    k = 1;
  }

  digitalWrite(LedPinSucess, HIGH);
  delay(150);
  digitalWrite(LedPinSucess, LOW);
  delay(150);

  botao = digitalRead(BotaoPin);
  if (botao == LOW) {
    error_count = 0;

Verde:
    digitalWrite(LedVerd, LOW);

    for (int i = 0; i < LED_COUNT; ++i) {
        leds[i] = CRGB(255, 0, 0);
      }

      FastLED.show();

      delay(300);  //Espera um pouco mais na primeira medição   
          
      V1 = correnteShunt();
      Serial.print("V1 VERDE: ");
      Serial.print(V1);
      Serial.print("\n");

      apagar();


      if ( (V1 < (VERDE_TOTAL - 0.04)) || (V1 > (VERDE_TOTAL + 0.04))) {
        Serial.print("V1 VERDE ERRO: ");
        Serial.print(V1);
        Serial.print("\n");
        Serial.print("erro count: ");
      
        error();
        error_count++;
        goto Fim;
        apagar();
      }
        

    
    digitalWrite(LedVerd, HIGH);


Vermelho:

    digitalWrite(LedVerm, LOW);
    LED_COUNT = virtualNumLed;

    for (int i = 0; i < LED_COUNT; ++i) {
        leds[i] = CRGB(0, 255, 0);
      }

      FastLED.show();

      delay(300);
      V1 = correnteShunt();
      Serial.print("V1 VERMELHO: ");
      Serial.print(V1);
      Serial.print("\n");

      apagar();

      if ( (V1 < (VERMELHO_TOTAL - 0.04)) || (V1 > (VERMELHO_TOTAL + 0.04))) {
        Serial.print("V1 VERMELHO ERRO: ");
        Serial.print(V1);
        Serial.print("\n");
        Serial.print("erro count: ");


        error();
        error_count++;
        goto Fim;
      }
    

    digitalWrite(LedVerm, HIGH);

Azul:

    digitalWrite(LedAz, LOW);
    LED_COUNT = virtualNumLed;

    for (int i = 0; i < LED_COUNT; ++i) {
        leds[i] = CRGB(0, 0, 255);
      }

      FastLED.show();
      delay(300);
      V1 = correnteShunt();
      Serial.print("V1 AZUL: ");
      Serial.print(V1);
      Serial.print("\n");

      apagar();

      if ( (V1 < (AZUL_TOTAL - 0.04)) || (V1 > (AZUL_TOTAL + 0.04))) {
        Serial.print("V1 AZUL ERRO: ");
        Serial.print(V1);
        Serial.print("\n");
        Serial.print("erro count: ");

        error();
        error_count++;
        goto Fim;
      }

    if (error_count < 1) {
      sucess();
    }

Fim:
    apagar();
    FastLED.show();
    k = 0;
    while (true) {
      botao = digitalRead(BotaoPin);
      if (botao == LOW) {
        break;
      }
    }
  }
}


int error() {
  digitalWrite(LedPinErro, HIGH);
  delay(400);
  digitalWrite(LedPinErro, LOW);
  delay(400);
  digitalWrite(LedPinErro, HIGH);
  digitalWrite(buzzer, HIGH);
  delay(400);
  digitalWrite(LedPinErro, LOW);
  digitalWrite(buzzer, LOW);
  delay(400);
  digitalWrite(LedPinErro, HIGH);
  digitalWrite(buzzer, HIGH);
  delay(400);
  digitalWrite(LedPinErro, LOW);
  digitalWrite(buzzer, LOW);

  digitalWrite(LedVerm, HIGH);
  digitalWrite(LedVerd, HIGH);
  digitalWrite(LedAz, HIGH);

  return 0;
}


int sucess() {
  //Serial.print("\t GG ");
  digitalWrite(LedAz, HIGH);
  digitalWrite(LedPinSucess, HIGH);
  delay(800);
  digitalWrite(LedPinSucess, LOW);
  delay(400);
  buzz();
  digitalWrite(LedPinSucess, HIGH);
  delay(800);
  digitalWrite(LedPinSucess, LOW);
}


int apagar() {
  for (int i = 0; i < virtualNumLed; ++i) {
    leds[i] = CRGB::Black;
  }
  return 1;
}


int buzz() {
  digitalWrite(buzzer, HIGH);
  delay(200);
  digitalWrite(buzzer, LOW);
}

float correnteShunt() {
  /*
  int i = 0; 
  
  for(i; i < 5000; ++i)
  {
    sensorValue = (sensorValue * 0.999) + (analogRead(analogInPin) * 0.001);
  }
  
  outputValue = mapf(sensorValue, 0, 1023, 0, 5.0);

  return (outputValue);*/


  /////////////////////////////////////

  float soma = 0;
  int i = 0;
  for (i; i < 50; ++i) {
    sensorValue = analogRead(analogInPin);
    outputValue = mapf(sensorValue, 0, 1023, 0, 5.0);
    delay(1);
    soma = outputValue + soma;
  }
  soma = soma / i;

  return (soma);
}



float mapf(float x, float in_min, float in_max, float out_min, float out_max) {
  return (x - in_min) * (out_max - out_min) / (in_max - in_min) + out_min;
}
