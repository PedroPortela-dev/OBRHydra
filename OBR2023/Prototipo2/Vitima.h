#include "Variaveis.h"
#include "Objetos.h"
#include "Despejo.h"

void atualizarDist(){
  distB = distanceSensorDown.measureDistanceCm();
  distC = distanceSensorUp.measureDistanceCm();
  distD = distanceSensorRight.measureDistanceCm();
  distE = distanceSensorLeft.measureDistanceCm();
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
    drive->direita90();
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
    drive->esquerda90();
  }
  else if(distparedeE - distE > 4){
    Serial.print("Distancia direita: ");
    Serial.println(distD);
    Serial.print("Distancia esquerda: ");
    Serial.println(distE);
    Serial.println("Achei vitima na esquerda");
    drive->freiar();
    delay(5000);
    drive->esquerda90();
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
    drive->direita90();
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