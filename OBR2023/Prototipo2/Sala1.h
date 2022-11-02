#include "SeguidorLinha.h"
#include "Obstaculo.h"
#include "Rampa.h"
#include "Sala3.h"

void sala1(){
  if(!Sala3){
    loopSala3();
  }else{
    verificacaoSeguidor();
    verificacaoObstaculo();
    verificacaoMPU();
  }
}