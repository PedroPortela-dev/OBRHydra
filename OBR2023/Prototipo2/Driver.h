#ifndef DRIVER_H_INCLUDED
#define DRIVER_H_INCLUDED

#include "Motor.h"
#include "Variaveis.h"

class Driver{
  public:
    Driver(Motor *md, Motor *me){
      this->mD=md;
      this->mE=me;
    }
    void mover(int velD, int velE){
      mD->mover(velD);
      mE->mover(velE);
    }
    void frente(int vel){
      mD->frente(vel);
      mE->frente(vel);
    }
    void tras(int vel){
      mD->tras(vel);
      mE->tras(vel);
    }
    void direita(int vel){
      mD->tras(vel);
      mE->frente(vel);
    }
    void esquerda(int vel){
      mD->frente(vel);
      mE->tras(vel);
    }
    void parar(){
      mD->parar();
      mE->parar();
    }
    void freiar(){
      mD->freiar();
      mE->freiar();      
    }
    void direita90(){
      direita(POWER); //Giro a Direita
      delay(DELAY1);
    }
    void esquerda90(){
      esquerda(POWER); //Giro a Direita
      delay(DELAY1);
    }
  private:
    Motor *mD;
    Motor *mE;
};

#endif