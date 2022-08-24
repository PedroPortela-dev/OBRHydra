#include <HCSR04.h>

// PORTA 5 COM DEFEITO
#define velmotorD 5
#define md1 6
#define md2 7

#define velmotorE 4
#define me1 3
#define me2 2

#define triggerE 31
#define echoE 33
#define triggerD 29 
#define echoD 27
#define triggerC 35
#define echoC 37
#define triggerB 13
#define echoB 12

#define POWER 70
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
      analogWrite(pv,0);
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
      }
      else{
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


UltraSonicDistanceSensor distanceSensorDown(triggerB, echoB);
UltraSonicDistanceSensor distanceSensorUp(triggerC,echoC);
UltraSonicDistanceSensor distanceSensorLeft(triggerE, echoE);
UltraSonicDistanceSensor distanceSensorRight(triggerD, echoD);
Motor *md = new Motor(md1,md2,velmotorD, 0);
Motor *me = new Motor(me1,me2,velmotorE, 0);
Driver *drive = new Driver(md,me);

float distb, distc, distd, diste, distVitima;
int lado = 1;
String entrada = ".";
boolean despejei = false;
int despejos =0;

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
    }while(distc < 60);
  }
  if(entrada == "direita"){
    if(lado%2 == 0){
      drive->frente(100);
      do{
        atualizarDist();
        Serial.println("frente ate o meio");
      }
      while(distc > 33);
      esquerda90();
      do{
        atualizarDist();
        vitima();
      }while(!despejei);
    //}else{
      esquerda90();
      drive->frente(100);
      do{atualizarDist();}while(distc > 33);
      direita90();
      drive->tras(128);
      delay(1000);
      do{
        atualizarDist();
        vitima();
      }while(distc > 5);
    }
  }
  else{
    if(lado%2 == 0){
      drive->frente(100);
      do{atualizarDist();}while(distc > 33);
      direita90();
      do{
        atualizarDist();
        vitima();
      }while(distc > 5);
    }else{
      esquerda90();
      drive->frente(100);
      do{atualizarDist();}while(distc > 33);
      esquerda90();
      drive->tras(128);
      delay(1000);
      do{
        atualizarDist();
        vitima();
      }while(distc > 5);
    }
  }
}
bool voltar = false;
float distparedeD = 0;
float distparedeE = 0;
void vitima(){
  if(distd >30 && distd <60){
    distparedeD = distd;
  }
  if(diste >30 && diste <60){
    distparedeE = diste;
  }
  if(distparedeD - distd > 4){
    Serial.println("Achei vitima na direita");
    Serial.print("Distancia direita: ");
    Serial.println(distd);
    Serial.print("Distancia esquerda: ");
    Serial.println(diste);
    drive->freiar();
    delay(5000);
    direita90();
    distVitima = distc;
    drive->tras(1);
    delay(1000);
    //descer garra
    drive->frente(100);
    do{atualizarDist();}while(distc > 5);
    //subir garra
    drive->tras(100);
    do{atualizarDist();}while(distc < distVitima);
    esquerda90();
  }
  else if(distparedeE - diste > 4){
    Serial.print("Distancia direita: ");
    Serial.println(distd);
    Serial.print("Distancia esquerda: ");
    Serial.println(diste);
    Serial.println("Achei vitima na esquerda");
    drive->freiar();
    delay(5000);
    esquerda90();
    distVitima = distc;
    drive->tras(100);
    delay(1000);
    //descer garra
    drive->frente(100);
    do{atualizarDist();}while(distc > 5);
    //subir garra
    drive->tras(100);
    do{atualizarDist();}while(distc < distVitima);
    direita90();
  }
  else{
    atualizarDist();
    if(distc > 5 && !voltar){
      Serial.println("FRENTE, buscando vitima");
      drive->frente(100);  
    }else{
      voltar = true;
      if(distd >=70 || diste >=70){
        delay(200);
        Serial.println("Despejo");
        Serial.print("Distancia direita: ");
        Serial.println(distd);
        Serial.print("Distancia esquerda: ");
        Serial.println(diste);
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
  Serial.println(diste);
  Serial.print("Sensor direita: ");
  Serial.println(distd);
  
}

void despejo(){
  if(distd >=70){
    esquerda90();
    drive->tras(128);
    delay(2000);
    // DESPEJAR
  }else{
    direita90();
    drive->tras(100);
    delay(2000);
    // DESPEJAR
    despejei = true;
    despejos++;
  }
}


void capturarVitima(){
  drive->frente(128);
  delay(3000);
  //SOBE GARRA
}


void acharTriangulo(){
  triangulo();
//  if(distb >= 70){
//    Serial.println("TRIANGULO");
//    triangulo();
////    drive->freiar();
////    delay(5000);
//  }else if(distc - distb > 10){
//    Serial.println("VITIMA");
//    // capturarVitima();
//  }
//  else if(distc <10){
//    lado++;
//    Serial.println("PAREDE");
//    Serial.println(entrada);
//    parede();
//  }else{
//    drive->frente(180);
//    Serial.println("FRENTE");
//  }
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

void setup() {
  Serial.begin(9600);
}

void direita90(){
  drive->direita(110);
  delay(1300);
}

void esquerda90(){
  drive->esquerda(110);
  delay(1300);
}

bool entrei = false;
int comecar = 0;
void loop() { 
  atualizarDist();
//  printDist();
//  delay(1000);
//  if(distd < diste && !entrei && diste != -1 && distd != -1){
//    entrada ="direita";
//    entrei = true;
//  }else if(!entrei && diste != -1 && distd != -1){
//    entrada = "esquerda";
//    entrei = true;
//  }
////  acharTriangulo();
  if(comecar > 10){
    testeIdentificacao();  
  }else{
     if (Serial.available() > 0) {
      // lê do buffer o dado recebido:
     Serial.println("Comecar o codigo: 0 ou 1");
     comecar = Serial.read();
     Serial.println(comecar,DEC);
     }
  }
}

void atualizarDist(){
  distb = distanceSensorDown.measureDistanceCm();
  distc = distanceSensorUp.measureDistanceCm();
  distd = distanceSensorRight.measureDistanceCm();
  diste = distanceSensorLeft.measureDistanceCm();
}

void printDist(){
  Serial.print("Distancia cima: ");
  Serial.println(distc);
  Serial.print("Distancia baixo: ");
  Serial.println(distb);
  Serial.println("Distancia direita: ");
  Serial.println(distd);
  Serial.println("Distancia esquerda: ");
  Serial.println(diste);
}
