// Inclusão das Bibliotecas

#include "Variaveis.h"
#include "Objetos.h"
#include "SeguidorLinha.h"
#include "Obstaculo.h"
#include "Rampa.h"

void setup() {

  // m_despejo.attach(9);

  MPUsetup();

  Serial.begin(9600);
}

void loop() {
  if(!Sala3){
    loopSala3();
  }else{
    // verificacaoSeguidor();
    // verificacaoObstaculo();
    verificacaoMPU();
    // P_NEUTRA;
  }
}

void loopSala3() { 
  atualizarDist();
  printDist();
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
 if(distB>= 70){
   Serial.println("TRIANGULO");
  //  triangulo();
   drive->freiar();
   delay(1000);
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
   drive->frente(100);
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
  delay(DELAY1);
}

void esquerda90(){
  drive->esquerda(110);
  delay(DELAY1);
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
  // Serial.println("Distancia direita: ");
  // Serial.println(distD);
  // Serial.println("Distancia esquerda: ");
  // Serial.println(distE);
}
