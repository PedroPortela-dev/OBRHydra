#include "Variaveis.h"
#include "Objetos.h"

void despejo(){
  if(distD >=70){
    drive->esquerda90();
    drive->tras(128);
    delay(2000);
    drive->freiar();
    timer = millis();
    while (millis()-timer<200)
    {
      // P_Despejo;
    }
  }else{
    drive->direita90();
    drive->tras(100);
    delay(2000);
    drive->freiar();
    // P_Despejo;
    // DESPEJAR
    despejei = true;
    despejos++;
  }
}