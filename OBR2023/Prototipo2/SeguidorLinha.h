#include "Variaveis.h"
#include "Objetos.h"

void setupLinha(){
  pinMode(sensForaD,INPUT);
  pinMode(sensForaE,INPUT);
  pinMode(sensDentroD,INPUT);
  pinMode(sensDentroE,INPUT);
}

void verificacaoSeguidor(){ 
  if (sensDentroE==BRANCO && sensDentroD==PRETO){
    drive->direita(POWER1);
  }
  else if (sensDentroE==PRETO && sensDentroD==BRANCO){
    drive->esquerda(POWER1); 
  }
  else if (sensDentroE==BRANCO && sensDentroD==BRANCO){
    drive->frente(POWER);
  }
  else if (sensDentroE==PRETO && sensDentroD==PRETO){
    drive->frente(POWER);
  }
}