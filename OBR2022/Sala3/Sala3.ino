// Inclusão das Bibliotecas
#include<Wire.h>
#include <HCSR04.h>
#include <Servo.h>

#define sensForaD !digitalRead(8)
#define sensForaE !digitalRead(10)
#define sensDentroD digitalRead(9)
#define sensDentroE digitalRead(11)
#define PRETO 1
#define BRANCO 0
int POWER = 70;
#define POWER1 200
#define DELAY 200
#define DELAY1 500
#define DELAY2 1700
// #define P_NEUTRA m_despejo.write(0)
// #define P_Despejo m_despejo.write(90)

class Motor{
  public:
    Motor(int v, int p1, int p2, bool forward, int pExtra = 0){
      this->p1=p1;
      this->p2=p2;
      this->pv=v;
      this->forward=forward;
      this->pExtra = pExtra;
      pinMode(p1,OUTPUT);
      pinMode(p2,OUTPUT);
      pinMode(pv,OUTPUT);
      digitalWrite(p1,LOW);
      digitalWrite(p2,LOW);
      digitalWrite(pv,0);
    }
    void frente(int v){
      v+= pExtra;
      if(forward){
        digitalWrite(p1,LOW);
        digitalWrite(p2,HIGH);
        analogWrite(pv,v);  
      }else{
        digitalWrite(p1,HIGH);
        digitalWrite(p2,LOW);
        analogWrite(pv,v);
      }
    }
    void tras(int v){
      v+= pExtra;
      if(forward){
        digitalWrite(p1,HIGH);
        digitalWrite(p2,LOW);
        analogWrite(pv,v);
      }else{
        digitalWrite(p1,LOW);
        digitalWrite(p2,HIGH);
        analogWrite(pv,v);  
      }
    }
    void parar(){
      digitalWrite(p1,LOW);
      digitalWrite(p2,LOW);
      analogWrite(pv,0);
    }
    void freiar(){
      digitalWrite(p1,HIGH);
      digitalWrite(p2,HIGH);
      analogWrite(pv,255);
    }
    void mover(int vel){
      vel+= pExtra;
      if(vel > 0){
        frente(vel);
      }
      else if(vel < 0){
        vel = -vel;
        tras(vel);
      }else{
        freiar();
      }
    }
  private:
    int p1,p2,pv, pExtra;
    bool forward;
};

class Driver{
  public:
    Driver(Motor *md, Motor *me){
      this->mD=md;
      this->mE=me;
    }
    void mover(int velD, int velE){
      mD->mover(velD);
      mE->mover(velE);
    }
    void frente(int vel){
      mD->frente(vel);
      mE->frente(vel);
    }
    void tras(int vel){
      mD->tras(vel);
      mE->tras(vel);
    }
    void direita(int vel){
      mD->tras(vel);
      mE->frente(vel);
    }
    void esquerda(int vel){
      mD->frente(vel);
      mE->tras(vel);
    }
    void parar(){
      mD->parar();
      mE->parar();
    }
    void freiar(){
      mD->freiar();
      mE->freiar();      
    }
    void direita90(){
      direita(POWER); //Giro a Direita
      delay(DELAY1);
    }
    void esquerda90(){
      esquerda(POWER); //Giro a Direita
      delay(DELAY1);
    }
  private:
    Motor *mD;
    Motor *mE;
};

class SensorCor{
  public:
    unsigned int valorVermelho = 0;
    unsigned int valorVerde = 0;
    unsigned int valorAzul = 0;
    unsigned int valorBranco = 0;
  
    int pinS2, pinS3, pinOut;
    
    SensorCor(int pinS2, int pinS3, int pinOut){
      this->pinS2 = pinS2;
      this->pinS3 = pinS3;
      this->pinOut = pinOut;

      pinMode(pinS2, OUTPUT);
      pinMode(pinS3, OUTPUT);
      pinMode(pinOut, INPUT);
    }

    void detectaCor() {
      //Vermelho
      digitalWrite(pinS2, LOW);
      digitalWrite(pinS3, LOW);
      valorVermelho = pulseIn(pinOut, !digitalRead(pinOut));
      
      //Sem filtro
      digitalWrite(pinS2, HIGH);
      valorBranco = pulseIn(pinOut, !digitalRead(pinOut));
    
      //Verde
      digitalWrite(pinS3, HIGH);
      valorVerde = pulseIn(pinOut, !digitalRead(pinOut));
    }

    String verificador()
    {
      //Detecta a cor
      detectaCor();

      if(valorVermelho < 5){
        Serial.println("VERMELHO");
        return "Vermelho";
      }else if(valorBranco > 10){
        Serial.println("PRETO");
        return "Preto";
      }else if(valorVermelho > 11){
         Serial.println("VERDE");
         return "Verde";
      }else{
        Serial.println("BRANCO");
        return "Branco";
      }
    }
};

unsigned long timer, timer2;

Motor *md = new Motor(2,3,4, false);
Motor *me = new Motor(5,6,7, false, 35);
Motor *coleta = new Motor(A15, A14, A13, false);
// Motor *braço = new Motor(A15, A14, A13, false);
// Servo m_despejo;
Driver *drive = new Driver(md,me);
SensorCor *SensorCorD = new SensorCor(53, 51, 49);
SensorCor *SensorCorE = new SensorCor(52, 50, 48);
UltraSonicDistanceSensor distanceSensorDown(A12, A11);
UltraSonicDistanceSensor distanceSensorUp(A6, A5);
UltraSonicDistanceSensor distanceSensorLeft(A10, A9);
UltraSonicDistanceSensor distanceSensorRight(A8, A7);
String corD, corE;
float distB, distC,distD, distE;

const int MPU = 0x68;
float AccX, AccY, AccZ, Temp, GyrX, GyrY, GyrZ, UltimoValorGyrZ, DerivadaGyrZ, MinDerivadaGyrZ, MaxDerivadaGyrZ, MediaGyrZ, MaxMediaGyrZ;
float ultimosValoresGyrZ[20];
int i = 0;
bool Subida = false, Descendo = false, Rampa = false, Sala3 = false;

void setup() {

  // m_despejo.attach(9);

  pinMode(sensForaD,INPUT);
  pinMode(sensForaE,INPUT);
  pinMode(sensDentroD,INPUT);
  pinMode(sensDentroE,INPUT);

  MPUsetup();

  Serial.begin(9600);
}

void loop() {
  if(Sala3){
    loopSala3();
  }else{
    drive->frente(POWER);
    // verificacaoSeguidor();
    // verificacaoObstaculo();
    verificacaoMPU();
    // P_NEUTRA;
  }
}

void verificacaoSeguidor(){
//  
//  atualizacaoCor();
//  
//  else if (corD == "Verde" && corE == "Verde"){
//    Serial.println("MeiaVolta");
//    MeiaVolta();
//  }
//  else if (corD == "Verde" && corE != "Verde"){
//    Serial.println("DirVerde");
//    DireitaVerde();
//  }
//  else if (corD != "Verde" && corE == "Verde"){
//    Serial.println("EsqVerde");
//    EsquerdaVerde();
//  } else 
  if (sensDentroE==BRANCO && sensDentroD==PRETO){
    Direita();
  }
  else if (sensDentroE==PRETO && sensDentroD==BRANCO){
    Esquerda();  
  }
  else if (sensDentroE==BRANCO && sensDentroD==BRANCO){
    Frente();
  }
  else if (sensDentroE==PRETO && sensDentroD==PRETO){
    Frente();
  }
}

void atualizacaoCor(){
  corD = SensorCorD->verificador();
  corE = SensorCorE->verificador();
}

void MeiaVolta(){
  drive->direita(POWER);
  delay(DELAY2+DELAY1);
  while(sensDentroD != PRETO){};
}

void DireitaVerde(){
 drive->frente(POWER);
 timer = millis();
 while(millis()-timer>DELAY){
   atualizacaoCor();
   if(corE == "Verde"){
      MeiaVolta();
      return;
   }
 }
 
 drive->direita(POWER);
 delay(DELAY);
 while(sensDentroD != PRETO){};
}

void EsquerdaVerde(){
 drive->frente(POWER);
 timer = millis();
 while(millis()-timer>DELAY){
   atualizacaoCor();
   if(corD == "Verde"){
      MeiaVolta();
      return;
   }
 }
 
 drive->esquerda(POWER);
 delay(DELAY);
 while(sensDentroE != PRETO){};
}

void Direita(){
  drive->direita(POWER1);
}

void Esquerda(){
  drive->esquerda(POWER1);
}

void Frente(){
  drive->frente(POWER);
}

void verificacaoObstaculo(){

  atualizacaoDist();
  if(distD > 0 && distC > 0){
    if(distD < 10 && distC < 10){
      Serial.println("Obstaculo");
      obstaculo();
    }
    else if(distD < 10 && distC > 30){
      Serial.println("Kit resgate");
      kitResgate();
    }
  }
}

void atualizacaoDist(){
  distD = distanceSensorRight.measureDistanceCm();
  distC = distanceSensorUp.measureDistanceCm();
  distB = distanceSensorDown.measureDistanceCm();
  distE = distanceSensorLeft.measureDistanceCm();
}

void obstaculo()
{
  
  drive->direita90();

  drive->frente(POWER); //Frente
  do{atualizacaoDist();} while (distE > 20 || distE == -1);
  do{atualizacaoDist();} while (distE <= 20 && distE != -1);

  drive->esquerda90();

  drive->frente(POWER); //Frente
  do{atualizacaoDist();} while (distE > 20 || distE == -1);
  do{atualizacaoDist();} while (distE <= 20 && distE != -1);

  drive->esquerda90(); //Giro a Esquerda

  drive->frente(POWER); //Frente
  while (sensDentroD == BRANCO || sensDentroE == BRANCO || sensForaD == BRANCO || sensForaE == BRANCO){}  
  delay(DELAY);

  drive->direita(POWER); //Giro a Direita
  while(sensDentroE== BRANCO){}
}

void kitResgate(){
  //baixar garra
  capturarVitima();
  drive->tras(POWER); //Frente
  delay(DELAY1);


}

void MPUsetup() {
  Serial.begin(9600);

  Wire.begin();
  Wire.beginTransmission(MPU);
  Wire.write(0x6B);
  Wire.write(0);
  Wire.endTransmission(true);

  Wire.beginTransmission(MPU);
  Wire.write(0x1B);
  Wire.write(0x00000000);
  Wire.endTransmission();

  Wire.beginTransmission(MPU);
  Wire.write(0x1C);
  Wire.write(0b00000000);
  Wire.endTransmission();
}

void MPUloop() {
  
  Wire.beginTransmission(MPU);
  Wire.write(0x3B);
  Wire.endTransmission(false);
  Wire.requestFrom(MPU, 14, true);

  AccX = Wire.read() << 8 | Wire.read();
  AccY = Wire.read() << 8 | Wire.read();
  AccZ = Wire.read() << 8 | Wire.read();
  Temp = Wire.read() << 8 | Wire.read();
  GyrX = Wire.read() << 8 | Wire.read(); 
  GyrY = Wire.read() << 8 | Wire.read(); 
  GyrZ = Wire.read() << 8 | Wire.read(); 
  AccX /= 16384;
  AccY /= 16384;
  AccZ /= 16384;
  GyrX /= 131;
  GyrY /= 131;
  GyrZ /= 131;

  // DerivadaGyrZ = UltimoValorGyrZ - GyrZ;
  // UltimoValorGyrZ = GyrZ;
  
  // if(MaxDerivadaGyrZ > 5000){
  //   Subida = true;
  //   MaxDerivadaGyrZ = 0; 
  // }

  ultimosValoresGyrZ[i] = GyrZ;
  if(i < 20){
    i++;
  }else{
    i = 0;
  }

  for(int j = 0; j < 20; j++){
    MediaGyrZ += ultimosValoresGyrZ[j];
  }
  MediaGyrZ /= 20;

  MaxDerivadaGyrZ = (MediaGyrZ > MaxDerivadaGyrZ)? MediaGyrZ:MaxDerivadaGyrZ;
  MinDerivadaGyrZ = (MediaGyrZ < MinDerivadaGyrZ)? MediaGyrZ:MinDerivadaGyrZ;
  // Serial.print("MediaGyrZ");
  // Serial.println(MediaGyrZ);
  // Serial.print("MaxDerivadaGyrZ");
  // Serial.println(MaxDerivadaGyrZ);
  Serial.print("MinDerivadaGyrZ");
  Serial.println(MinDerivadaGyrZ);
  if(MediaGyrZ < -4){
    // drive->tras(300);
    // delay(300);
    // Subida = true;
  }
  if(MediaGyrZ < -10){
    Descendo = true; 
  }else{
    Descendo = false;
    if(Descendo){
      Subida = false;
    }
  }
}

void verificacaoMPU(){
  MPUloop();
  if(Subida){
    Serial.print("Subida");
    // if(distD < 10 && distD != -1 && distE < 10 && distE != -1){
    //   Rampa = true;
    // }
    POWER = 220;
  }
  else if(Descendo && !Rampa){
    POWER = 0;
  }
  else if(!Subida && Rampa){
    Sala3 = true;
  }
  else{
    POWER = 70;
  }
}

float distVitima;
int lado = 1;
String entrada = ".";
boolean despejei = false;
int despejos =0;
bool voltar = false;
float distparedeD = 0;
float distparedeE = 0;
bool entrei = false;
int comecar = 0;

void loopSala3() { 
  atualizarDist();
  if(distD < distE && !entrei && distE != -1 && distD != -1){
    entrada ="direita";
    entrei = true;
  }else if(!entrei && distE != -1 && distD != -1){
    entrada = "esquerda";
    entrei = true;
  }
  acharTriangulo();
}

void acharTriangulo(){
 if(distB >= 70){
   Serial.println("TRIANGULO");
   triangulo();
//    drive->freiar();
//    delay(5000);
 }else if(distC - distB > 10){
   Serial.println("VITIMA");
   capturarVitima();
 }
 else if(distC <10){
   lado++;
   Serial.println("PAREDE");
   Serial.println(entrada);
   parede();
 }else{
   drive->frente(180);
   Serial.println("FRENTE");
 }
}

void parede(){
  int velD;
  int velE;
  if(entrada == "direita"){
    velE = 150;
    velD = 0;
    esquerda90();
  }else{
    direita90();
    velE = 0;
    velD = 150;
  }
//  drive->freiar();
//  delay(40);
//  drive->tras(128);
//  delay(500);
//  drive->mover(velD,velE);
//  delay(1000);
//  drive->tras(100);
//  delay(800);
}

void testeIdentificacao(){
//      esquerda90();
      do{
        atualizarDist();
        vitima();
      }while(!despejei);
}

void triangulo(){
  if(lado != 1){
    drive->tras(128);
    do{
      atualizarDist();
      Serial.println("Tras ate a parede");
    }while(distC < 60);
  }
  if(entrada == "direita"){
    if(lado%2 == 0){
      drive->frente(100);
      do{
        atualizarDist();
        Serial.println("frente ate o meio");
      }
      while(distC > 33);
      esquerda90();
      do{
        atualizarDist();
        vitima();
      }while(!despejei);
    //}else{
      esquerda90();
      drive->frente(100);
      do{atualizarDist();}while(distC > 33);
      direita90();
      drive->tras(128);
      delay(1000);
      do{
        atualizarDist();
        vitima();
      }while(distC > 5);
    }
  }
  else{
    if(lado%2 == 0){
      drive->frente(100);
      do{atualizarDist();}while(distC > 33);
      direita90();
      do{
        atualizarDist();
        vitima();
      }while(distC > 5);
    }else{
      esquerda90();
      drive->frente(100);
      do{atualizarDist();}while(distC > 33);
      esquerda90();
      drive->tras(128);
      delay(1000);
      do{
        atualizarDist();
        vitima();
      }while(distC > 5);
    }
  }
}

void vitima(){
  if(distD >36 && distD <60){
    distparedeD = distD;
  }
  if(distE >38 && distE <60){
    distparedeE = distE;
  }
  if(distparedeD - distD > 4){
    Serial.println("Achei vitima na direita");
    Serial.print("Distancia direita: ");
    Serial.println(distD);
    Serial.print("Distancia esquerda: ");
    Serial.println(distE);
    drive->freiar();
    delay(5000);
    direita90();
    distVitima = distC;
    drive->tras(1);
    delay(1000);
    //descer garra
  // coleta->frente(255);
    drive->frente(100);
    do{atualizarDist();}while(distC > 5);
    //subir garra
  // coleta->freiar();
    drive->tras(100);
    do{atualizarDist();}while(distC < distVitima);
    esquerda90();
  }
  else if(distparedeE - distE > 4){
    Serial.print("Distancia direita: ");
    Serial.println(distD);
    Serial.print("Distancia esquerda: ");
    Serial.println(distE);
    Serial.println("Achei vitima na esquerda");
    drive->freiar();
    delay(5000);
    esquerda90();
    distVitima = distC;
    drive->tras(100);
    delay(1000);
    //descer garra
  // coleta->frente(255);
    drive->frente(100);
    do{atualizarDist();}while(distC > 5);
    //subir garra
  // coleta->freiar();
    drive->tras(100);
    do{atualizarDist();}while(distC < distVitima);
    direita90();
  }
  else{
    atualizarDist();
    if(distC > 5 && !voltar){
      Serial.println("FRENTE, buscando vitima");
      drive->frente(100);  
    }else{
      voltar = true;
      if(distD >=70 || distE >=70){
        delay(200);
        Serial.println("Despejo");
        Serial.print("Distancia direita: ");
        Serial.println(distD);
        Serial.print("Distancia esquerda: ");
        Serial.println(distE);
        drive->freiar();
        delay(5000);
        despejo();
      }else{
        Serial.println("TRAS voltando pro triangulo e identificando");
        atualizarDist();
        drive->tras(100);         
      }
    }
  }
  Serial.print("Sensor esquerda: ");
  Serial.println(distE);
  Serial.print("Sensor direita: ");
  Serial.println(distD);
  
}

void despejo(){
  if(distD >=70){
    esquerda90();
    drive->tras(128);
    delay(2000);
    drive->freiar();
    timer = millis();
    while (millis()-timer<200)
    {
      // P_Despejo;
    }
  }else{
    direita90();
    drive->tras(100);
    delay(2000);
    drive->freiar();
    // P_Despejo;
    // DESPEJAR
    despejei = true;
    despejos++;
  }
}

void capturarVitima(){
  // braço->tras(50);
  delay(500);
  // braço->parar();
  coleta->frente(255); 
  drive->frente(128);
  delay(1000);
  coleta->parar();
  drive->parar();
  // braço->frente(50);
  delay(500);
  // braço->parar();
  coleta->tras(255);
  delay(100);
  coleta->parar();
}


void direita90(){
  drive->direita(110);
  delay(1300);
}

void esquerda90(){
  drive->esquerda(110);
  delay(1300);
}

void atualizarDist(){
  distB = distanceSensorDown.measureDistanceCm();
  distC = distanceSensorUp.measureDistanceCm();
  distD = distanceSensorRight.measureDistanceCm();
  distE = distanceSensorLeft.measureDistanceCm();
}

void printDist(){
  Serial.print("Distancia cima: ");
  Serial.println(distC);
  Serial.print("Distancia baixo: ");
  Serial.println(distB);
  Serial.println("Distancia direita: ");
  Serial.println(distD);
  Serial.println("Distancia esquerda: ");
  Serial.println(distE);
}
