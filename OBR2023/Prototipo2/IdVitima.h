#include "Variaveis.h"
#include "Objetos.h"
#include "Vitima.h"

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
