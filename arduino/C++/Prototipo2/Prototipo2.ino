#include <HCSR04.h>

#define sensForaD 8
#define sensForaE 11
#define sensDentroD 9
#define sensDentroE 10
#define PRETO 1
#define BRANCO 0
#define POWER 70
#define POWER1 120
#define DELAY 200

class Motor{
  public:
    Motor(int p1, int p2, int v, bool forward){
      this->p1=p1;
      this->p2=p2;
      this->pv=v;
      this->forward=forward;
      pinMode(p1,OUTPUT);
      pinMode(p2,OUTPUT);
      pinMode(pv,OUTPUT);
      digitalWrite(p1,LOW);
      digitalWrite(p2,LOW);
      digitalWrite(pv,0);
    }
    void frente(int v){
      if(forward){
        digitalWrite(p1,LOW);
        digitalWrite(p2,HIGH);
        analogWrite(pv,v);  
      }else{
        digitalWrite(p1,HIGH);
        digitalWrite(p2,LOW);
        analogWrite(pv,v);
      }
    }
    void tras(int v){
      if(forward){
        digitalWrite(p1,HIGH);
        digitalWrite(p2,LOW);
        analogWrite(pv,v);
      }else{
        digitalWrite(p1,LOW);
        digitalWrite(p2,HIGH);
        analogWrite(pv,v);  
      }
    }
    void parar(){
      digitalWrite(p1,LOW);
      digitalWrite(p2,LOW);
      analogWrite(pv,0);
    }
    void freiar(){
      digitalWrite(p1,HIGH);
      digitalWrite(p2,HIGH);
      analogWrite(pv,255);
    }
    void mover(int vel){
      if(vel > 0){
        frente(vel);
      }
      else if(vel < 0){
        vel = -vel;
        tras(vel);
      }
    }
  private:
    int p1,p2,pv;
    bool forward;
};

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
  private:
    Motor *mD;
    Motor *mE;
};

class SensorCor{
  public:
    unsigned int valorVermelho = 0;
    unsigned int valorVerde = 0;
    unsigned int valorAzul = 0;
    unsigned int valorBranco = 0;
  
    int pinS2, pinS3, pinOut;
    
    SensorCor(int pinS2, int pinS3, int pinOut){
      this->pinS2 = pinS2;
      this->pinS3 = pinS3;
      this->pinOut = pinOut;

      pinMode(pinS2, OUTPUT);
      pinMode(pinS3, OUTPUT);
      pinMode(pinOut, INPUT);
    }

    void detectaCor() {
      //Vermelho
      digitalWrite(pinS2, LOW);
      digitalWrite(pinS3, LOW);
      valorVermelho = pulseIn(pinOut, !digitalRead(pinOut));
      
      //Sem filtro
      digitalWrite(pinS2, HIGH);
      valorBranco = pulseIn(pinOut, !digitalRead(pinOut));
    
      //Verde
      digitalWrite(pinS3, HIGH);
      valorVerde = pulseIn(pinOut, !digitalRead(pinOut));
    }

    String verificador()
    {
      //Detecta a cor
      detectaCor();

      if(valorVermelho < 5){
        Serial.println("VERMELHO");
        return "Vermelho";
      }else if(valorBranco > 10){
        Serial.println("PRETO");
        return "Preto";
      }else if(valorVermelho > 11){
         Serial.println("VERDE");
         return "Verde";
      }else{
        Serial.println("BRANCO");
        return "Branco";
      }

    
//      if(valorBranco <= 3){
//        Serial.println("Branco");
//        return "Branco";
//      }else
//      if(abs(valorVermelho-valorVerde) < 5){
//        Serial.println("Preto");
//        return "Preto";
//      }else
//      //Verifica se a cor vermelha foi detectada
//      if (valorVermelho < valorVerde) {
//        Serial.println("Vermelho");
//        return "Vermelho";
//      } else 
//      if (valorVerde < valorVermelho)  //Verifica se a cor verde foi detectada
//      {
//        Serial.println("Verde");
//        return "Verde";
//      }
    }
};

unsigned long timer;

Motor *md = new Motor(3,4,2, false);
Motor *me = new Motor(6,7,5, false);
Driver *drive = new Driver(md,me);
SensorCor *sensorCorD = new SensorCor(6,7,8);
SensorCor *sensorCorE = new SensorCor(5,3,4);
UltraSonicDistanceSensor distanceSensorDown(13, 12);
UltraSonicDistanceSensor distanceSensorUp(13, 12);
String corD, corE;
int distD, distU;

void setup() {

  pinMode(sensForaD,INPUT);
  pinMode(sensForaE,INPUT);
  pinMode(sensDentroD,INPUT);
  pinMode(sensDentroE,INPUT);

  Serial.begin(9600);
}

void loop() {
 verificacaoSeguidor();
// verificacaoObstaculo();
}

void verificacaoSeguidor(){
//  
//  atualizacaoCor();
//  
//  if(corD == "Vermelho" && corE == "Vermelho"){
//    Serial.println("Chegada");
//    Chegada();
//  }
//  else if (corD == "Verde" && corE == "Verde"){
//    Serial.println("MeiaVolta");
//    MeiaVolta();
//  }
//  else if (corD == "Verde" && corE != "Verde"){
//    Serial.println("DirVerde");
//    DireitaVerde();
//  }
//  else if (corD != "Verde" && corE == "Verde"){
//    Serial.println("EsqVerde");
//    EsquerdaVerde();
//  } else 
//  if(digitalRead(sensForaE)==PRETO && digitalRead(sensForaD)==PRETO){
//    Serial.println("Cruz");
//    Cruz();
//  }
  if(digitalRead(sensForaE)==PRETO && digitalRead(sensForaD)==BRANCO){
    Serial.println("EstForaE");
    EsquerdaLonga();
  }
  else if(digitalRead(sensForaE)==BRANCO && digitalRead(sensForaD)==PRETO){
    Serial.println("EstForaD");
    DireitaLonga();
  }
  else if (digitalRead(sensDentroE)==BRANCO && digitalRead(sensDentroD)==PRETO){ 
    Serial.println("Dir");
    Direita();
  }
  else if (digitalRead(sensDentroE)==PRETO && digitalRead(sensDentroD)==BRANCO){
    Serial.println("Esq");
    Esquerda();  
  }
  else if (digitalRead(sensDentroE)==BRANCO && digitalRead(sensDentroD)==BRANCO){
    Serial.println("Frente");
    Frente();
  }
  else if (digitalRead(sensDentroE)==PRETO && digitalRead(sensDentroD)==PRETO){
    Serial.println("Frente");
    Frente();
  }
}

void atualizacaoCor(){
  corD = sensorCorD->verificador();
  corE = sensorCorE->verificador();
}

void Chegada(){
  drive->freiar();
  exit(0);
}

void MeiaVolta(){
  drive->direita(POWER);
  while(digitalRead(sensDentroD) != PRETO){};
}

void DireitaVerde(){
 
 drive->frente(POWER);
 timer = millis();
 while(millis()-timer>DELAY){
   atualizacaoCor();
   if(corE == "Verde"){
      MeiaVolta();
      return;
   }
 }
 
 drive->direita(POWER);
 delay(DELAY);
 while(digitalRead(sensDentroD) != PRETO){};
 
}

void EsquerdaVerde(){

 drive->frente(POWER);
 timer = millis();
 while(millis()-timer>DELAY){
   atualizacaoCor();
   if(corD == "Verde"){
      MeiaVolta();
      return;
   }
 }
 
 drive->esquerda(POWER);
 delay(DELAY);
 while(digitalRead(sensDentroE) != PRETO){};
 

}

void Cruz(){
  drive->frente(POWER);
  delay(DELAY);
}

void DireitaLonga(){
  
  drive->frente(POWER);
  delay(DELAY);
  
  drive->direita(POWER1);
  while(digitalRead(sensDentroE) != PRETO){
    if(digitalRead(sensForaE)==PRETO){
      drive->frente(POWER);
      delay(DELAY);
      //Direita 
      drive->esquerda(POWER1);
      while(digitalRead(sensDentroD) != PRETO){};
      break;
    }
  }
}

void EsquerdaLonga(){
  
  drive->frente(POWER);
  delay(DELAY);
  
  drive->esquerda(POWER1);
  while(digitalRead(sensDentroD) != PRETO){
    if(digitalRead(sensForaD)==PRETO){
      drive->frente(POWER);
      delay(DELAY);
      //Direita 
      drive->direita(POWER1);
      while(digitalRead(sensDentroE) != PRETO){};
      break;
    }
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

void verificacaoObstaculo(){

  atualizacaoDist();

  if(distD < 10 && distU < 10){
    Serial.println("Obstaculo");
    obstaculo();
  }
  else if(distD < 10 && distU < 30){
    Serial.println("Rampa");
    // Rampa
  }
  else if(distD < 10 && distU > 30){
    Serial.println("Kit resgate");
    kitResgate();
  }
  
}

void atualizacaoDist(){
  distD = distanceSensorDown.measureDistanceCm();
  distU = distanceSensorUp.measureDistanceCm();
}

void obstaculo(){
  drive->direita(POWER);
  delay(DELAY*2);
  drive->frente(POWER);
  delay(DELAY);
  drive->esquerda(POWER);
  delay(DELAY*2);
  drive->frente(POWER);
  delay(DELAY);
  drive->esquerda(POWER);
  delay(DELAY*2);
  drive->frente(POWER);
  delay(DELAY);
  drive->direita(POWER);
  delay(DELAY*2);
}

void kitResgate(){
  
}
