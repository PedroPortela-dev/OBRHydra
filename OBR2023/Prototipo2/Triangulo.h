#include "Variaveis.h"
#include "Objetos.h"
#include "IdVitima.h"

void parede(){
  int velD;
  int velE;
  if(entrada == "direita"){
    velE = 150;
    velD = 0;
    drive->esquerda90();
  }else{
    drive->direita90();
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