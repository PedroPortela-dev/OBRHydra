//#include <SoftwareSerial.h>
//SoftwareSerial BTserial(10, 11); // RX | TX

#define sensForaD 8
#define sensForaE 2
#define dist1 22

#define tmp 20

bool seguirLinha = true;
unsigned long timer;

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
  private:
    Motor *mD;
    Motor *mE;
};

Motor *md=new Motor(4, 5, 3,true);
Motor *me=new Motor(12, 13, 11,true);
Driver *drive = new Driver(md,me);

class SensorCor{
  public:
    
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
    
      if(valorBranco <= 4){
        Serial.println("Branco");
        return "Branco";
      }else
      if(abs(valorVermelho-valorVerde) < 5){
        Serial.println("Preto");
        return "Preto";
      }else
      //Verifica se a cor vermelha foi detectada
      if (valorVermelho < valorVerde) {
        Serial.println("Vermelho");
        return "Vermelho";
      } else 
      if (valorVerde < valorVermelho)  //Verifica se a cor verde foi detectada
      {
        Serial.println("Verde");
        return "Verde";
      }
    }

  private:
    unsigned int valorVermelho = 0;
    unsigned int valorVerde = 0;
    unsigned int valorAzul = 0;
    unsigned int valorBranco = 0;
  
    int pinS2, pinS3, pinOut;
};

SensorCor *sensorCorD = new SensorCor(6, 7, 8);
SensorCor *sensorCorE = new SensorCor(9, 10, 11);

void obstaculo(){

  //Tras
  drive->mover(-255,-255);
  delay(500);

  //Girando E    
  drive->mover(255,-255);
  delay(3100);

  //Frente
  drive->mover(255,255);
  delay(1000);
  
  //Girando D
  drive->mover(-255,255);
  delay(3100);

  //Frente
  drive->mover(255,255);
  delay(1500);
  
  //Girando E
  drive->mover(255,-255);
  delay(3100);

  //Frente
  drive->mover(255,255);
  delay(1000);
  
  //Girando D    
  drive->mover(255,255);
  delay(3100);
  
}

void setup() {
  pinMode(sensForaD,INPUT);
  pinMode(sensForaE,INPUT);

  pinMode(dist1, INPUT);

  Serial.begin(9600);
//  BTserial.begin(9600);
}

void loop() {
  
  while(seguirLinha){

    String corD = sensorCorD->verificador();
    String corE = sensorCorD->verificador();
    timer = millis();

    // Seguir Linha
    if(digitalRead(sensForaD)==1 && digitalRead(sensForaE)==0){
      
      drive->mover(120,120);
      delay(200);

      //Direita
      drive->mover(-120,120);
      while(corE == "Branco"){
        if(digitalRead(sensForaE)==1){
          //Esquerda
          drive->mover(120,-120);
          while(corD == "Branco"){};
          break;
        }
      }
      timer = millis();
    }
    else if(digitalRead(sensForaE)==1 && digitalRead(sensForaD)==0){
      
      drive->mover(120,120);
      delay(200);

      //Esquerda
      drive->mover(120,-120);
      //testar continuidade
      while(corD == "Branco"){
        if(digitalRead(sensForaD)==1){
          //Direita 
          drive->mover(-120,120);
          while(corE == "Branco"){};
          break;
        }
      }
      timer = millis();
    }
    else if (corD == "Branco" && corE == "PRETO"){ 
      //Esquerda  
      drive->mover(120,-120);
      
    }
    
    else if(corE == "Branco" && corD == "PRETO"){
      //direita
      drive->mover(-120,120);
    
    }

    else{      
      drive->mover(150,150);
    }

    if((corD == "Verde" || corE == "Verde") && millis()-timer>400){
//      fazer o verde
    }
  }
}
