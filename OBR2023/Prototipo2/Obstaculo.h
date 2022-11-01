#include "Variaveis.h"
#include "Objetos.h"

void verificacaoObstaculo(){

  atualizacaoDist();
  if(distD > 1 && distC > 1){
    if(distD < 10 && distC < 10){
      Serial.println("Obstaculo");
      obstaculo();
    }
    else if(distD < 10 && distC > 30){
      Serial.println("Kit resgate");
      kitResgate();
    }
  }
}

void atualizacaoDist(){
  distD = distanceSensorRight.measureDistanceCm();
  distC = distanceSensorUp.measureDistanceCm();
  distB = distanceSensorDown.measureDistanceCm();
  distE = distanceSensorLeft.measureDistanceCm();
}

void obstaculo()
{
  
  drive->direita90();

  drive->frente(POWER); //Frente
  do{atualizacaoDist();} while (distE > 20 || distE == -1);
  do{atualizacaoDist();} while (distE <= 20 && distE != -1);

  drive->esquerda90();

  drive->frente(POWER); //Frente
  do{atualizacaoDist();} while (distE > 20 || distE == -1);
  do{atualizacaoDist();} while (distE <= 20 && distE != -1);

  drive->esquerda90(); //Giro a Esquerda

  drive->frente(POWER); //Frente
  while (sensDentroD == BRANCO || sensDentroE == BRANCO || sensForaD == BRANCO || sensForaE == BRANCO){}  
  delay(DELAY);

  drive->direita(POWER); //Giro a Direita
  while(sensDentroE== BRANCO){}
}

void kitResgate(){
  //baixar garra
  capturarVitima();
  drive->tras(POWER); //Frente
  delay(DELAY1);


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