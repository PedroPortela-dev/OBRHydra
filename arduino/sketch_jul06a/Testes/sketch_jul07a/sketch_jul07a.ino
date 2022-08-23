#include <SoftwareSerial.h>

SoftwareSerial serial1(0, 1); // RX, TX


//  MOTOR

#define velmotorD 3
#define md1 4
#define md2 5

#define velmotorE 11
#define me1 12
#define me2 13


//  SENSOR

#define ldrD A1
#define ldrE A0

#define tmp 100


int velD=0, velE=0;

void setup() {

//  MOTOR
  pinMode(velmotorD,OUTPUT);
  pinMode(velmotorE,OUTPUT);
  
  pinMode(md1,OUTPUT);
  pinMode(md2,OUTPUT);
  
  pinMode(me1,OUTPUT);
  pinMode(me2,OUTPUT);
  
  digitalWrite(md1,LOW);
  digitalWrite(md2,LOW);
  
  digitalWrite(me1,LOW);
  digitalWrite(me2,LOW);
  
  analogWrite(velmotorD,velD);
  analogWrite(velmotorE,velE);

// SENSOR

  pinMode(ldrD, INPUT);
  pinMode(ldrE, INPUT);

  serial1.begin(9600);

}

void loop() {

  int erro = analogRead(ldrD) - analogRead(ldrE);
//
 // map(erro, 0, 1023, 0, 255);
//
//  velD = 255 + erro;
//  velE = 255 - abs(erro);
//
//  analogWrite(velmotorD,velD);
//  analogWrite(velmotorE,velE);

  velD = 255;
  velE = 255;
  
  analogWrite(velmotorD,velD);
  analogWrite(velmotorE,velE);
  
  digitalWrite(md1,LOW);
  digitalWrite(md2,HIGH);
  
  digitalWrite(me1,HIGH);
  digitalWrite(me2,LOW);

  serial1.print(erro);
  
  delay(tmp);


}
