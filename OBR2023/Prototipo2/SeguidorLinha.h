#ifndef SEGUIDORLINHA_H_INCLUDED
#define SEGUIDORLINHA_H_INCLUDED

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
    Direita();
  }
  else if (sensDentroE==PRETO && sensDentroD==BRANCO){
    Esquerda();  
  }
  else if (sensDentroE==BRANCO && sensDentroD==BRANCO){
    Frente();
  }
  else if (sensDentroE==PRETO && sensDentroD==PRETO){
    Frente();
  }
}

void Direita(){
  drive->direita(POWER1);
}

void Esquerda(){
  drive->esquerda(POWER1);
}

void Frente(){
  drive->frente(POWER);
}


#endif