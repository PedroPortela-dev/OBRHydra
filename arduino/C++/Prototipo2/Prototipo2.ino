// Inclusão das Bibliotecas
#include<Wire.h>
#include <HCSR04.h>

#define sensForaD 8
#define sensForaE 11
#define sensDentroD 9
#define sensDentroE 10
#define PRETOd 0
#define BRANCOd 1
#define PRETOf 1
#define BRANCOf 0
int POWER = 70;
#define POWER1 110
#define POWER2 100
#define POWER3 140
#define DELAY 250
#define DELAY1 500
#define DELAY2 1700
#define DELAY3 2000

class Motor{
  public:
    Motor(int p1, int p2, int v, bool forward){
      this->p1=p1;
      this->p2=p2;
      this->pv=v;
      this->forward=forward;
      pinMode(p1,OUTPUT);
      pinMode(p2,OUTPUT);
      pinMode(pv,OUTPUT);
      digitalWrite(p1,LOW);
      digitalWrite(p2,LOW);
      digitalWrite(pv,0);
    }
    void frente(int v){
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
    int p1,p2,pv;
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

unsigned long timer;

Motor *md = new Motor(3,4,2, false);
Motor *me = new Motor(6,7,5, false);
Driver *drive = new Driver(md,me);
SensorCor *SensorCorD = new SensorCor(53, 51, 49);
SensorCor *SensorCorE = new SensorCor(52, 50, 48);
UltraSonicDistanceSensor distanceSensorDown(13, 12);
UltraSonicDistanceSensor distanceSensorUp(13, 12);
UltraSonicDistanceSensor distanceSensorLeft(13, 12);
UltraSonicDistanceSensor distanceSensorRight(13, 12);
String corD, corE;
float distD, distU,distR, distL;
const int MPU = 0x68;
float AccX, AccY, AccZ, Temp, GyrX, GyrY, GyrZ, UltimoValorGyrZ, DerivadaGyrZ, MinDerivadaGyrZ, MaxDerivadaGyrZ, MediaGyrZ, MaxMediaGyrZ;
float ultimosValoresGyrZ[20];
int i = 0;
bool Subida = false, Descendo = false, Rampa = false, Sala3 = false;

void setup() {

  pinMode(sensForaD,INPUT);
  pinMode(sensForaE,INPUT);
  pinMode(sensDentroD,INPUT);
  pinMode(sensDentroE,INPUT);

  MPUsetup();

  Serial.begin(9600);
}

void loop() {
  if(Sala3){

  }else{
    verificacaoSeguidor();
    verificacaoObstaculo();
    verificacaoMPU();
  }
}

void verificacaoSeguidor(){
//  
//  atualizacaoCor();
//  
//  if(corD == "Vermelho" && corE == "Vermelho"){
//    Serial.println("Chegada");
//    Chegada();
//  }
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
  if(digitalRead(sensForaE)==PRETOf && digitalRead(sensForaD)==PRETOf){
    Serial.println("Cruz");
    Cruz();
  }
  if(digitalRead(sensForaE)==PRETOf && digitalRead(sensForaD)==BRANCOf){
    Serial.println("EstForaE");
    EsquerdaLonga();
  }
  else if(digitalRead(sensForaE)==BRANCOf && digitalRead(sensForaD)==PRETOf){
    Serial.println("EstForaD");
    DireitaLonga();
  }
  else if (digitalRead(sensDentroE)==BRANCOd && digitalRead(sensDentroD)==PRETOd){ 
    Serial.println("Dir");
    Direita();
  }
  else if (digitalRead(sensDentroE)==PRETOd && digitalRead(sensDentroD)==BRANCOd){
    Serial.println("Esq");
    Esquerda();  
  }
  else if (digitalRead(sensDentroE)==BRANCOd && digitalRead(sensDentroD)==BRANCOd){
    Serial.println("Frente");
    Frente();
  }
  else if (digitalRead(sensDentroE)==PRETOd && digitalRead(sensDentroD)==PRETOd){
    Serial.println("Frente");
    Frente();
  }
}

void atualizacaoCor(){
  corD = SensorCorD->verificador();
  corE = SensorCorE->verificador();
}

void Chegada(){
  drive->freiar();
  exit(0);
}

void MeiaVolta(){
  drive->direita(POWER);
  delay(DELAY2+DELAY1);
  while(digitalRead(sensDentroD) != PRETOd){};
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
 while(digitalRead(sensDentroD) != PRETOd){};
 
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
 while(digitalRead(sensDentroE) != PRETOd){};
 

}

void Cruz(){
  drive->frente(POWER);
  delay(DELAY);
}

void DireitaLonga(){
  
  drive->frente(POWER);
  delay(DELAY);
  
  drive->mover(-POWER2, POWER1);
  while(digitalRead(sensDentroE) != PRETOd){
    if(digitalRead(sensForaE)==PRETOf){
      drive->mover(POWER1,-POWER2);
      while(digitalRead(sensDentroD) != PRETOd){};
      return;
    }
  }
}

void EsquerdaLonga(){
  
  drive->frente(POWER);
  delay(DELAY);
  
  drive->mover(POWER1,-POWER2);
  while(digitalRead(sensDentroD) != PRETOd){
    if(digitalRead(sensForaD)==PRETOf){ 
      drive->mover(-POWER2,POWER1);
      while(digitalRead(sensDentroE) != PRETOd){};
      return;
    }
  }
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
  if(distD > 0 && distU > 0){
    if(distD < 10 && distU < 10){
      Serial.println("Obstaculo");
      obstaculo();
    }
    else if(distD < 10 && distU > 30){
      Serial.println("Kit resgate");
      kitResgate();
    }
  }
}

void atualizacaoDist(){
  distD = distanceSensorDown.measureDistanceCm();
  distU = distanceSensorUp.measureDistanceCm();
  distR = distanceSensorRight.measureDistanceCm();
  distL = distanceSensorLeft.measureDistanceCm();
}

void obstaculo()
{
  drive->direita(POWER); //Giro a Direita
  delay(DELAY1);

  drive->frente(POWER); //Frente
  delay(DELAY2);

  drive->esquerda(POWER); //Giro a Esquerda
  delay(DELAY1);

  drive->frente(POWER); //Frente
  delay(DELAY3);

  drive->esquerda(POWER); //Giro a Esquerda
  delay(DELAY1);

  drive->frente(POWER); //Frente
  while (digitalRead(sensDentroD) == BRANCOd || digitalRead(sensDentroE) == BRANCOd || digitalRead(sensForaD) == BRANCOf || digitalRead(sensForaE) == BRANCOf){}  
  delay(DELAY);

  drive->direita(POWER); //Giro a Direita
  while(digitalRead(sensDentroE)== BRANCOd){}
}

void kitResgate(){
  
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

  DerivadaGyrZ = UltimoValorGyrZ - GyrZ;
  UltimoValorGyrZ = GyrZ;
  
  MaxDerivadaGyrZ = (DerivadaGyrZ > MaxDerivadaGyrZ)? DerivadaGyrZ:MaxDerivadaGyrZ;
  MinDerivadaGyrZ = (DerivadaGyrZ < MinDerivadaGyrZ)? DerivadaGyrZ:MinDerivadaGyrZ;
  if(MaxDerivadaGyrZ > 5000){
    Subida = true;
    MaxDerivadaGyrZ = 0; 
  }

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

  if(abs(MediaGyrZ) < -10){
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
    if(distD < 10 && distL < 10){
      Rampa = true;
    }
    POWER = 150;
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