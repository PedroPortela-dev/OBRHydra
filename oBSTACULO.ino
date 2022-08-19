#include <HCSR04.h>

// PORTA 5 COM DEFEITO
#define velmotorD 2
#define md1 3
#define md2 4

#define velmotorE 5
#define me1 6
#define me2 7

#define POWER 70
#define POWER1 110
#define POWER2 100
#define POWER3 140

// trigger PWM
#define p_trigger1 36
#define p_echo1 37

#define p_trigger2 23
#define p_echo2 25

#define sensFrente 0

unsigned long timer, timer2;

bool seguirLinha = true, curva90 = false;
int velD=0 , velE=0,i =0 ;

void setup() {

  pinMode(velmotorD,OUTPUT);
  pinMode(md1,OUTPUT);
  pinMode(md2,OUTPUT);
  digitalWrite(md1,LOW);
  digitalWrite(md2,LOW);
  analogWrite(velmotorD,velD);

  pinMode(velmotorE,OUTPUT);
  pinMode(me1,OUTPUT);
  pinMode(me2,OUTPUT);
  digitalWrite(me1,LOW);
  digitalWrite(me2,LOW);
  analogWrite(velmotorE,velE);

  Serial.begin(9600);
}

void mover(int velD, int velE){
  if(velD>0){
    digitalWrite(md1, 1);
    digitalWrite(md2, 0);
    analogWrite(velmotorD,velD);
  }else if(velD<0){
    digitalWrite(md1, 0);
    digitalWrite(md2, 1);
    analogWrite(velmotorD,-(velD));
  }else{
    digitalWrite(md1, 1);
    digitalWrite(md2, 1);
    analogWrite(velmotorD,0);
  }
  
  if(velE>0){
    digitalWrite(me1, 1);
    digitalWrite(me2, 0);
    analogWrite(velmotorE,velE);
  }else if(velE<0){
    digitalWrite(me1, 0);
    digitalWrite(me2, 1);
    analogWrite(velmotorE,-(velE));
  }else{
    digitalWrite(me1, 1);
    digitalWrite(me2, 1);
    analogWrite(velmotorE,0);
  }
}


//UltraSonicDistanceSensor sensorDistance1(p_trigger1,p_echo1);
UltraSonicDistanceSensor sensorDistance2(p_trigger2,p_echo2);

void obstaculo(float distbaixo){
  
  bool obst = false;

  if(distbaixo < 13 && distbaixo > 0){
    mover(0,0);
    delay(100);
    obst = true;
  }

  if(obst){
    
    mover(-POWER3, POWER3); //Giro a Direita
    delay(500);
    
    mover(POWER, POWER); //Frente
    delay(1700);

    mover(POWER3, -POWER3); //Giro a Esquerda
    delay(500);

    mover(POWER, POWER); //Frente
    delay(2000);
    
    mover(POWER3, -POWER3); //Giro a Esquerda
    delay(500);

    mover(POWER, POWER); //Frente
    delay(1700);
    
    
    mover(0,0);
    delay(5000);
  }

}

void loop() {
//  float distD = sensorDistance1.measureDistanceCm();
  float distBaixo = sensorDistance2.measureDistanceCm();
  
  Serial.print("DISTANCE BAIXO: ");
  Serial.println(distBaixo);

  mover(70,70);
  
  obstaculo(distBaixo);
}

  //float distcima = sensorDistance1.measureDistanceCm();
  //float distbaixo = sensorDistance2.measureDistanceCm();
//  bool sensfrente = digitalRead(sensFrente);
  //acharTriangulo(distcima,distbaixo,sensfrente);
