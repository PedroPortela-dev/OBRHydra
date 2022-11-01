#include "Variaveis.h"
#include "Objetos.h"

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


void direita90(){
  drive->direita(110);
  delay(DELAY1);
}

void esquerda90(){
  drive->esquerda(110);
  delay(DELAY1);
}