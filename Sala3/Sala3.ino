#include <HCSR04.h>

// PORTA 5 COM DEFEITO
#define velmotorD 3
#define md1 2
#define md2 4

#define velmotorE 6
#define me1 7
#define me2 8

// trigger PWM
#define p_trigger1 11 
#define p_trigger2 A0
#define p_echo1 10
#define p_echo2 12
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
      analogWrite(pv,128);
    }
    void mover(int vel){
      if(vel > 0){
        frente(vel);
      }
      else if(vel < 0){
        vel = -vel;
        tras(vel);
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


UltraSonicDistanceSensor sensorDistance1(p_trigger1,p_echo1);
UltraSonicDistanceSensor sensorDistance2(p_trigger2,p_echo2);
Motor *md = new Motor(md1,md2,velmotorD, true);
Motor *me = new Motor(me1,me2,velmotorE, false);
Driver *drive = new Driver(md,me);

void acharTriangulo(float distbaixo){
  if(distbaixo > 65){
    Serial.println("ACHEI TRIANGULO");
    drive->freiar();
  }
  else if(distbaixo < 5){
    Serial.println("VIRAR E CONTINUAR PROCURANDO");
    drive->tras(100);
    delay(1000);
    drive->esquerda(128);
    delay(1000);
    drive->tras(100);
    delay(1000);
  }else{
    drive->frente(100);
    Serial.println("IR PARA FRENTE");
  }
  Serial.print("DISTANCE BAIXO: ");
  Serial.println(distbaixo);
}

void capturarVitima(float distFrente){
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

void acharVitimas(float distD,float distFrente){
  float distParede;
//  if(distD < 70 && distD > 60){
//    distParede = distD;
//  }
//  else if(distParede - distD > 3){
//    Serial.println("Achei Vitima");
//    drive->freiar();
//    capturarVitima(distFrente);
//  }
  if(distD <55){
    Serial.println("Achei Vitima");
    drive->freiar();
    capturarVitima(distFrente);
  }
  else{
    Serial.println("Procurando vitima");
    drive->tras(80);
  }
  Serial.print("DISTANCIA PAREDE");
  Serial.println(distParede);
  Serial.print("DISTANCIA ATUAL: ");
  Serial.println(distD);
  Serial.print("Variacao");
  Serial.println(distParede - distD);
  Serial.println("-----------------------");
}

void setup() {
  Serial.begin(9600);
}

void loop() {
  float distD = sensorDistance1.measureDistanceCm();
  float distBaixo = sensorDistance2.measureDistanceCm();
  //acharTriangulo(distBaixo);
  acharVitimas(distD,distBaixo);
  delay(1000);
}

  //float distcima = sensorDistance1.measureDistanceCm();
  //float distbaixo = sensorDistance2.measureDistanceCm();
//  bool sensfrente = digitalRead(sensFrente);
  //acharTriangulo(distcima,distbaixo,sensfrente);
