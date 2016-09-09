#include <MFRC522.h>
#include <SPI.h>

#define SS_PIN 10
#define RST_PIN 9

MFRC522 rfid(SS_PIN,RST_PIN);
MFRC522::MIFARE_Key key;

int zumbador = 5;
int boton = 6;
int ledRojo = 8;
int ledVerde = 4;

/* CONFIGURACIONES BOMBA */
int frecuencia = 2800;
int tiempoBomba = 1200;
float fragmento = tiempoBomba / 10;
float primeraFase = fragmento * 7;
float segundaFase = fragmento * 2;
float terceraFase = fragmento;




void setup() {
  // put your setup code here, to run once:
  pinMode(zumbador, OUTPUT);
  pinMode(boton, INPUT);
  pinMode(ledRojo, OUTPUT);
  pinMode(ledVerde, OUTPUT);
  Serial.begin(9600); //Inicializa la velocidad de Serial
  SPI.begin();  //Función que inicializa SPI
  rfid.PCD_Init();      //Función  que inicializa RFID

  digitalWrite(ledVerde, HIGH);
}

void beep(){
  tone(zumbador, 2800);
  digitalWrite(ledRojo, HIGH);
  delay(500);
  noTone(zumbador);
  digitalWrite(ledRojo, LOW);
}

void customBeep(int freq, int espera, int tiempos){
  for (int i = 0; i <= tiempos; i++){
    tone(zumbador, freq);
    digitalWrite(ledRojo, HIGH);
    delay(espera/2);
    noTone(zumbador);
    delay(espera/2);
    digitalWrite(ledRojo, LOW);
  }
}
void fastBeep(){
  tone(zumbador, 2800);
  digitalWrite(ledRojo, HIGH);
  delay(1000);
  noTone(zumbador);
  digitalWrite(ledRojo, LOW);
  delay(100);
}

void boom(){
  tone(zumbador, 240);
  digitalWrite(ledRojo, HIGH);
  delay(500);
  tone(zumbador, 200);
  digitalWrite(ledRojo, LOW);
  delay(500);
  tone(zumbador, 160);
  digitalWrite(ledRojo, HIGH);
  delay(2000);
  digitalWrite(ledRojo, LOW);
  noTone(zumbador);
}

void desactivar(){
  tone (zumbador, 160);
  delay(500);
  tone(zumbador, 200);
  delay(500);
  tone(zumbador, 240);
  delay(2000);
  noTone(zumbador);
}

void bomba(int segundos){
  int milisegundos = segundos * 1000;
  int tiempoActual = 0;
  int tiempoRestante = milisegundos;
  bool explotada = false;
  bool desactivada = false;
  bool desactivando = false;
  int tiempoDesactivando = 0;
  
  while (!explotada && !desactivada){
    desactivando = rfid.PICC_IsNewCardPresent();

    while (desactivando && !desactivada){
      tiempoDesactivando++;
      beep();
      Serial.println(tiempoDesactivando);
      if (tiempoDesactivando >= 20)
        desactivada = true;

      desactivando = rfid.PICC_IsNewCardPresent();
      desactivando = rfid.PICC_IsNewCardPresent();
    }
    if (!desactivada){
      
      Serial.println("nodesactivando");
      tiempoRestante = milisegundos - tiempoActual;
      tiempoDesactivando = 0;
      if (tiempoRestante >= primeraFase){
        delay(1500);
        tiempoActual += 2000;
        beep();
      }
      else if (tiempoRestante >= segundaFase && tiempoRestante < primeraFase){
        delay(500);
        tiempoActual += 1000;
        beep();
      }
      else if (tiempoRestante < segundaFase && tiempoRestante > 0){
        fastBeep();
        tiempoActual += 500;
      }
      else{
  
        delay(500);
        for (int i = 0; i < 20; i++){
          beep();
        }
        explotada = true;
      }  
    }
  }
  
  delay(500);

  if (desactivada)
    desactivar();

    delay(10000);
}

void loop() {
  if (rfid.PICC_IsNewCardPresent()) { //Verifica si hay una tarjeta
    digitalWrite(ledVerde, LOW);
    beep();
    delay(100);
    digitalWrite(ledVerde, HIGH);
    beep();
    delay(100);
    digitalWrite(ledVerde, LOW);
    beep();
    digitalWrite(ledRojo, HIGH);
    delay(5000);
    digitalWrite(ledRojo, LOW);
    bomba(30);  
  }
  digitalWrite(ledVerde, HIGH);
}
