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
      analogWrite(pv,128);
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
      digitalWrite(pinS2, HIGH);
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

#define sensForaD 8
#define sensForaE 2
#define PRETO 1
#define BRANCO 0

int power = 120;
unsigned long timer;

Motor *md = new Motor(5,7,6, true);
Motor *me = new Motor(3,4,2, false);
Driver *drive = new Driver(md,me);
SensorCor *sensorCorD = new SensorCor(6,7,8);
SensorCor *sensorCorE = new SensorCor(5,3,4);

void verificacaoSeguidor(){
  
  atualizacaoCor();
  
  if(corD == "Vermelho" && corE == "Vermelho"){
    Serial.println("Chegada");
    Chegada();
  }
  else if (corD == "Verde" && corE == "Verde"){
    Serial.println("MeiaVolta");
    MeiaVolta();
  }
  else if (corD == "Verde" && corE != "Verde"){
    Serial.println("DirVerde");
    DireitaVerde();
  }
  else if (corD != "Verde" && corE == "Verde"){
    Serial.println("EsqVerde");
    EsquerdaVerde();
  }
  else if(digitalRead(sensForaE)==PRETO && digitalRead(sensForaD)==PRETO){
    Serial.println("Cruz");
    Cruz();
  }
  else if(digitalRead(sensForaE)==PRETO && digitalRead(sensForaD)==BRANCO){
    Serial.println("EstForaE");
    EsquerdaLonga();
  }
  else if(digitalRead(sensForaE)==BRANCO && digitalRead(sensForaD)==PRETO){
    Serial.println("EstForaD");
    DireitaLonga();
  }
  else if (corD == "Preto" && corE == "Branco"){ 
    Serial.println("Dir");
    Direita();
  }
  else if (corD == "Branco" && corE == "Preto"){
    Serial.println("Esq");
    Esquerda();  
  }
  else if (corD == "Branco" && corE == "Branco"){
    Serial.println("Frente");
    Frente();
  }
  else if (corD == "Preto" && corE == "Preto"){
    Serial.println("Frente");
    Frente();
  }

  printh();
}

void atualizacaoCor(){
  String corD = sensorCorD->verificador();
  String corE = sensorCorE->verificador();
}

void printh(){
  Serial.print("Direita :");
  Serial.println(corD);
  Serial.print("Esquerda :");
  Serial.println(corE);
}

void Chegada(){
  drive->freiar();
  exit(0);
}

void MeiaVolta(){
  drive->direita(power);
  do{atualizacaoCor();}while(corD != "Preto");
}

void DireitaVerde(){
 
 drive->frente(power);
 timer = millis();
 while(millis()-timer>200){
   atualizacaoCor();
   if(corE == "Verde"){
      MeiaVolta();
      return;
   }
 }
 
 drive->direita(power);
 do{atualizacaoCor();}while(corD != "Preto");
 
}

void EsquerdaVerde(){

 drive->frente(power);
 timer = millis();
 while(millis()-timer>200){
   atualizacaoCor();
   if(corD == "Verde"){
      MeiaVolta();
      return;
   }
 }
 
 drive->esquerda(power);
 do{atualizacaoCor();}while(corE != "Preto");
 

}

void Cruz(){
  drive->frente(power);
  delay(200);
}

void DireitaLonga(){
  
  drive->frente(power);
  timer = millis();
  while(millis()-timer>200){
    atualizacaoCor();
    if(digitalRead(sensForaE)==PRETO){
        Cruz();
        return;
    }
  }
  
  drive->direita(power);
 do{atualizacaoCor();}while(corD != "Preto");
}

void EsquerdaLonga(){
  
  drive->frente(power);
  timer = millis();
  while(millis()-timer>200){
    atualizacaoCor();
    if(digitalRead(sensForaD)==PRETO){
        Cruz();
        return;
    }
  }
  
  drive->esquerda(power);
 do{atualizacaoCor();}while(corE != "Preto");
}

void Direita(){
  drive->direita(power);
}

void Esquerda(){
  drive->esquerda(power);
}

void Frente(){
  drive->frente(power);
}

void setup() {

  // Falta S0 e S1 dos sensores de cor (Direira,Esquerda)

  pinMode(sensForaD,INPUT);
  pinMode(sensForaE,INPUT);

  Serial.begin(9600);
//  BTserial.begin(9600);
}

void loop() {
 verificacaoSeguidor();
}