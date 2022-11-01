#include "Variaveis.h"
#include "Objetos.h"
#include "Triangulo.h"

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