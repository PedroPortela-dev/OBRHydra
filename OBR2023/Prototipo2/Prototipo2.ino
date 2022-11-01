// Inclusão das Bibliotecas

#include "Variaveis.h"
#include "Objetos.h"
#include "SeguidorLinha.h"
#include "Obstaculo.h"
#include "Rampa.h"
#include "Sala3.h"
#include "IdVitima.h"

void setup() {
  // m_despejo.attach(9);
  MPUsetup();
}

void loop() {
  if(!Sala3){
    loopSala3();
  }else{
    // verificacaoSeguidor();
    // verificacaoObstaculo();
    verificacaoMPU();
  }
}

void testeIdentificacao(){
//      esquerda90();
      do{
        atualizarDist();
        vitima();
      }while(!despejei);
}