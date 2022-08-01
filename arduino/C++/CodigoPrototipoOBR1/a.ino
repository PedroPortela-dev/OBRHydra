/*
     Q0456-Sketch.ino
     AUTOR:   BrincandoComIdeias
     LINK:    https://www.youtube.com/brincandocomideias ; https://cursodearduino.net/
     COMPRE:  https://www.arducore.com.br/
     SKETCH:  Reconhecimento de Cores - Sensor de Cor GY-31
     DATA:    15/04/2019

   ATUALIZACAO: em ../../.... por .... o que .......
*/

#define pinS0 4
#define pinS1 5
#define pinS2 6
#define pinS3 7
#define pinOut 8
#define pinLED 3

//Pinos dos LEDs
#define pinoLedVerm 2
#define pinoLedVerd 3
#define pinoLedAzul 4

unsigned int valorVermelho = 0;
unsigned int valorVerde = 0;
unsigned int valorAzul = 0;
unsigned int valorBranco = 0;

void detectaCor();

void setup()
{
  pinMode(pinS0, OUTPUT);
  pinMode(pinS1, OUTPUT);
  pinMode(pinS2, OUTPUT);
  pinMode(pinS3, OUTPUT);
  pinMode(pinLED, OUTPUT);
  pinMode(pinOut, INPUT);

  pinMode(pinoLedVerm, OUTPUT);
  pinMode(pinoLedVerd, OUTPUT);
  pinMode(pinoLedAzul, OUTPUT);

  Serial.begin(9600);
  digitalWrite(pinS0, HIGH);
  digitalWrite(pinS1, LOW);

  delay(2000);
  //Liga LED
  digitalWrite(pinLED, HIGH);
}

void loop(){
  verificador();
}

void verificador()
{
  //Detecta a cor
  detectaCor();

  //Mostra valores no serial monitor
//  Serial.print("Vermelho :");
//  Serial.print(valorVermelho);
//
//  Serial.print(" Verde : ");
//  Serial.print(valorVerde);
//
////  Serial.print(" Azul : ");
////  Serial.print(valorAzul);
//
//  Serial.print(" Branco : ");
//  Serial.print(valorBranco);
//  Serial.println();

  if(valorBranco <= 4){
    Serial.println("Branco");
  }else
  if(abs(valorVermelho-valorVerde) < 5){
    Serial.println("Preto");
  }else
  //Verifica se a cor vermelha foi detectada
  if (valorVermelho < valorVerde) {
    Serial.println("Vermelho");
  } else 
  if (valorVerde < valorVermelho)  //Verifica se a cor verde foi detectada
  {
    Serial.println("Verde");
  }
  //Delay para apagar os leds e reiniciar o processo
  delay(500);
}


// *********** Função de leitura so sensor de cor ********************
void detectaCor() {
  //Vermelho
  digitalWrite(pinS2, LOW);
  digitalWrite(pinS3, LOW);
  valorVermelho = pulseIn(pinOut, digitalRead(pinOut) == HIGH ? LOW : HIGH);
  
  //Sem filtro
  digitalWrite(pinS2, HIGH);
  valorBranco = pulseIn(pinOut, digitalRead(pinOut) == HIGH ? LOW : HIGH);

  //Azul
  digitalWrite(pinS2, LOW);
  digitalWrite(pinS3, HIGH);
  valorAzul = pulseIn(pinOut, digitalRead(pinOut) == HIGH ? LOW : HIGH);

  //Verde
  digitalWrite(pinS2, HIGH);
  valorVerde = pulseIn(pinOut, digitalRead(pinOut) == HIGH ? LOW : HIGH);
}
