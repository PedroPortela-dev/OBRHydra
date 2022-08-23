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
int lado;
String entrada = ".";

boolean triangulo(){
  if(lado%2 == 0){

  }
  else{

  }
}

void capturarVitima(){
   drive->tras(100);
   delay(200);
   drive->direita(128);
   delay(500);
   drive->tras(100);
   delay(500);
   int timer = millis();
   do{
    drive->frente(100); 
   }while(distFrente > 3);
   int timer2 = millis() - timer;
   drive->tras(128);   
   delay(timer2);
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
    //FUI NO BANHEIRO
    entradaEsquerda();
  }else{
    entradaDireita();
  }
  acharTriangulo();
}

void acharTriangulo(){
  if(distb >= 70){
    Serial.println("ACHEI TRIANGULO");
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
  drive->freiar();
  delay(40);
  drive->tras(100);
  delay(500);
  md->frente(128);
  me->freiar();
  delay(1700);
  drive->tras(100);
  delay(1000);
}