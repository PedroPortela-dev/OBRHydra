#include <HCSR04.h>

// PORTA 5 COM DEFEITO
#define velmotorD 3
#define md1 4
#define md2 2

#define velmotorE 13
#define me1 7
#define me2 8

// trigger PWM
#define p_trigger1 24
#define p_echo1 22
#define p_trigger2 28
#define p_echo2 26
#define p_trigger3 10
#define p_echo3 11
#define sensFrente 0

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


UltraSonicDistanceSensor distanceSensorDown(p_trigger2, p_echo2);
UltraSonicDistanceSensor distanceSensorUp(p_trigger1,p_echo1);
UltraSonicDistanceSensor distanceSensorLeft(52, 53);
UltraSonicDistanceSensor distanceSensorRight(p_trigger3, p_echo3);
Motor *md = new Motor(md1,md2,velmotorD, false);
Motor *me = new Motor(me1,me2,velmotorE, false);
Driver *drive = new Driver(md,me);

float distb, distc, distd, diste;
int lado = 1;
String entrada = ".";

void irMeio(){
  // O SIMPLES:
  drive->tras(128);
  delay(3000);
  frente(100);
  while(dist > 33){}
  return;
  //ACHEI COMPLICADO E PODE NÃO DAR CERTO ENTÃO OPTEI PELO SIMPLES
  // bool distanciei = false;
  //   if(distc < 33 && !distanciei){
  //     drive->frente(100);
  //     while(distc < 33){}
  //     distanciei = !distanciei;
  //   }else if(distc > 33 && !distanciei){
  //     drive->tras(100);
  //     while(distc > 33){}
  //     distanciei = !distanciei
  //   }
  //   else{
  //     drive->freiar();
  //     delay(50);
  //     return
  //   }
}

boolean triangulo(){
  irMeio();
  if(entrada == "direita"){
    drive->esquerda(100);
    delay(1500);
    if(lado%2 == 0){
      vitima();
    }else{
      drive->frente(100);
      delay(1000);
      drive->direita(100);
      delay(1500);
      drive->tras(100);
      delay(3000);
      vitima();
    }
  }
  else{
    drive->direita(100);
    delay(1500);
    if(lado%2 == 0){
      vitima();
    }else{
      drive->frente(100);
      delay(1000);
      drive->esquerda(100);
      delay(1500);
      drive->tras(100);
      delay(3000);
      vitima();
    }
  }
}

void vitima(){
  bool voltar = false;
  if(distd < 38){
    drive->direita(100);
    delay(1500);
    timer = millis();
    drive->frente(100);
    while(distc > 5){}
    timer2 = millis() - timer;
    capturarVitima();
    drive->tras(100);
    delay(timer2);
  }
  else if(diste < 38){
    drive->esquerda(100);
    delay(1500);
    timer = millis();
    drive->frente(100);
    while(distc > 5){}
    timer2 = millis() - timer;
    capturarVitima();
    drive->tras(100);
    delay(timer2);
  }
  else{
    if(distc > 5 && !voltar){
      drive->frente(80);
    }else{
      voltar = true;
      if(distd >=70 || diste >=70){
        despejo();
      }else{
        drive->tras(80);
      }
    }
  }
}

void despejo(){
  if(distd >=70){
    drive->esquerda(100);
    delay(1500);
    drive->tras(100);
    delay(4000);
    // DESPEJAR
  }else{
    drive->direita(100);
    delay(1500);
    drive->tras(100);
    delay(4000);
    // DESPEJAR
  }
}


void capturarVitima(){
  //DESCE GARRA
  drive->frente(128);
  delay(3000);
  //SOBE GARRA
}


void acharTriangulo(){
  if(distb >= 70){
    Serial.println("TRIANGULO");
    while(triangulo()){};
  }else if(distc - distb > 10){
    Serial.println("VITIMA");
    capturarVitima();
  }
  else if(distc <10){
    lado++;
    Serial.println("PAREDE");
    parede();
  }else{
    drive->frente(100);
    Serial.println("FRENTE");
  }
}

void parede(){
  int velD = 0;
  int velE = 0;
  if(entrada == "direita"){
    velE = 128;
    velD = 0;
  }else{
    velE = 0;
    velD = 128
  }
  drive->freiar();
  delay(40);
  drive->tras(100);
  delay(500);
  drive->mover(128,0);
  delay(1700);
  drive->tras(100);
  delay(1000);
}

void setup() {
  Serial.begin(9600);
}

void loop() {
  distb = distanceSensorDown.measureDistanceCm();
  distc = distanceSensorUp.measureDistanceCm();
  distd = distanceSensorRight.measureDistanceCm();
  diste = distanceSensorLeft.measureDistanceCm();
  if(distd < diste){
    entrada = "esquerda"
  }else{
    entrada = "direita"
  }
  acharTriangulo();
}