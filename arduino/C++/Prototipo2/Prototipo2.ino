#define velmotorD 3
#define md1 4
#define md2 5

#define velmotorE 11
#define me1 12
#define me2 13

#define sensForaD 8
#define sensForaE 2

#define dist1 22

#define tmp 700

String movimento;
bool seguirLinha = true, curva90 = false;
int velD=0 , velE=0,i =0 ;
unsigned long time, timeInicio, timeFim;

void setup() {  
  pinMode(velmotorD,OUTPUT);
  pinMode(md1,OUTPUT);
  pinMode(md2,OUTPUT);
  digitalWrite(md1,LOW);
  digitalWrite(md2,LOW);
  analogWrite(velmotorD,velD);

  pinMode(velmotorE,OUTPUT);
  pinMode(me1,OUTPUT);
  pinMode(me2,OUTPUT);
  digitalWrite(me1,LOW);
  digitalWrite(me2,LOW);
  analogWrite(velmotorE,velE);

  // Falta S0 e S1 dos sensores de cor (Direira,Esquerda)

  pinMode(sensForaD,INPUT);
  pinMode(sensForaE,INPUT);

  pinMode(dist1, INPUT);

  Serial.begin(9600);
//  BTserial.begin(9600);
}

void mover(int velD, int velE){
  if(velD>0){
    sentidoD(0,1,velD);
  }else if(velD<0){
    sentidoD(1,0,-velD);
  }else{
    sentidoD(1,1,0);
  }
  
  if(velE>0){
    sentidoE(0,1,velE);
  }else if(velE<0){
    sentidoE(1,0,-velE);
  }else{
    sentidoE(1,1,0);
  }
}

void sentidoD(bool mdA, bool mdB, int velMD){
 
  digitalWrite(md1,mdA);
  digitalWrite(md2,mdB);
  analogWrite(velmotorD,velMD);
 
}

void sentidoE(bool meA, bool meB, int velME){
  
  digitalWrite(me1,meA);
  digitalWrite(me2,meB);
  analogWrite(velmotorE,velME);
 
}

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

  private:
   
};


SensorCor *sensorCorD = new SensorCor(6, 7, 8);
SensorCor *sensorCorE = new SensorCor(5,3,4);


void verificacaoSeguidor(){
  
String corD = sensorCorD->verificador();
String corE = sensorCorE->verificador();
  
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
//  else if (digitalRead(sensForaD)==1 && digitalRead(sensForaE)==0){
//    Serial.println("DirLong");
//    DireitaLonga();  
//  }
//  else if (digitalRead(sensForaE)==1 && digitalRead(sensForaD)==0){
//    Serial.println("EsqLong");
//    EsquerdaLonga();  
//  }
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
}

void Chegada(){
//  mover(0,0);
  verificacaoSeguidor();
}

void MeiaVolta(){
  verificacaoSeguidor();
}

void DireitaVerde(){
//  
//  mover(120,120);
//  delay(200);
//  
//  mover(-120,120);
//  while(corE == "Branco"){}
//  
  verificacaoSeguidor();
}

void EsquerdaVerde(){

//  mover(120,120);
//  delay(200);
//  
//  mover(120,-120);
//  while(corD == "Branco"){}
//  
  verificacaoSeguidor();

}

void DireitaLonga(){
  
//  mover(120,120);
//  delay(200);
//  
//  mover(-120,120);
//  while(corE == "Branco"){
//    if(digitalRead(sensForaE)==1){
//      mover(120,-120);
//      while(corD == "Branco"){};
//      break;
//    } 
//  }
  verificacaoSeguidor();
}

void EsquerdaLonga(){
  
//  mover(120,120);
//  delay(200);
//  
//  mover(120,-120);
//  while(corD == "Branco"){
//    if(digitalRead(sensForaD)==1){
//      mover(-120,120);
//      while(corE == "Branco"){};
//      break;
//    }
//  }
  verificacaoSeguidor();
}

void Direita(){
//  mover(-120,120);
  verificacaoSeguidor();
}

void Esquerda(){
//  mover(120,-120);
  verificacaoSeguidor();
}

void Frente(){
//  mover(150,150);
  verificacaoSeguidor();
}


void loop() {
String corD = sensorCorD->verificador();
String corE = sensorCorE->verificador();
//  verificacaoSeguidor();
  Serial.print("Direita :");
  Serial.println(corD);
  Serial.print("Esquerda :");
  Serial.println(corE);
  delay(700);
}

/*
#define velmotorD 3
#define md1 4
#define md2 5

#define velmotorE 11
#define me1 12
#define me2 13

#define sensForaD 8
#define sensForaE 2

#define dist1 22

#define tmp 700

bool seguirLinha = true, curva90 = false;
int velD=0 , velE=0,i =0 ;
unsigned long time, timeInicio, timeFim;

void setup() {
  pinMode(velmotorD,OUTPUT);
  pinMode(md1,OUTPUT);
  pinMode(md2,OUTPUT);
  digitalWrite(md1,LOW);
  digitalWrite(md2,LOW);
  analogWrite(velmotorD,velD);

  pinMode(velmotorE,OUTPUT);
  pinMode(me1,OUTPUT);
  pinMode(me2,OUTPUT);
  digitalWrite(me1,LOW);
  digitalWrite(me2,LOW);
  analogWrite(velmotorE,velE);

  pinMode(sensDentroD,INPUT);
  pinMode(sensForaD,INPUT);
  pinMode(sensDentroE,INPUT);
  pinMode(sensForaE,INPUT);

  pinMode(dist1, INPUT);

  Serial.begin(9600);
//  BTserial.begin(9600);
}

void mover(int velD, int velE){
  if(velD>0){
    sentidoD(0,1,velD);
  }else if(velD<0){
    sentidoD(1,0,-velD);
  }else{
    sentidoD(1,1,0);
  }
  
  if(velE>0){
    sentidoE(0,1,velE);
  }else if(velE<0){
    sentidoE(1,0,-velE);
  }else{
    sentidoE(1,1,0);
  }
}

void sentidoD(bool mdA, bool mdB, int velMD){
 
  digitalWrite(md1,mdA);
  digitalWrite(md2,mdB);
  analogWrite(velmotorD,velMD);
 
}

void sentidoE(bool meA, bool meB, int velME){
  
  digitalWrite(me1,meA);
  digitalWrite(me2,meB);
  analogWrite(velmotorE,velME);
 
}

void loop() {
  
  while(seguirLinha){
    time = millis();

    // Seguir Linha
    if(digitalRead(sensForaD)==1 && digitalRead(sensForaE)==0){
      
      mover(120,120);
      delay(200);

      //Direita
      mover(-120,120);
      while(digitalRead(sensDentroE) == 0){
        if(digitalRead(sensForaE)==1){
          //Esquerda
          mover(120,-120);
          while(digitalRead(sensDentroD) == 0){};
          break;
        }
      }
      timer = millis();
    }
    else if(digitalRead(sensForaE)==1 && digitalRead(sensForaD)==0){
      
      mover(120,120);
      delay(200);

      //Esquerda
      mover(120,-120);
      //testar continuidade
      while(digitalRead(sensDentroD) == 0){
        if(digitalRead(sensForaD)==1){
          //Direita 
          mover(-120,120);
          while(digitalRead(sensDentroE) == 0){};
          break;
        }
      }
      timer = millis();
    }
    else if (digitalRead(sensDentroD)==0 && digitalRead(sensDentroE)==1){ 
      //Esquerda  
      mover(120,-120);
      
    }
    
    else if(digitalRead(sensDentroD)==1 && digitalRead(sensDentroE)==0){
      //direita
      mover(-120,120);
    
    }

    else{      
      velocidade(150,150);
    }

}

class SensorCor{
  public:
    
    SensorCor(int pinS2, int pinS3, int pinOut){
      this.pinS2 = pinS2;
      this.pinS3 = pinS3;
      this.pinOut = pinOut;

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
    
      //Azul
      digitalWrite(pinS2, LOW);
      digitalWrite(pinS3, HIGH);
      valorAzul = pulseIn(pinOut, !digitalRead(pinOut));
    
      //Verde
      digitalWrite(pinS2, HIGH);
      valorVerde = pulseIn(pinOut, !digitalRead(pinOut));
    }

    String verificador(int valorBranco, int valorVermelho, int valorVerde)
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
}

SensorCor *sensorCorD = new SensorCor(6, 7, 8);
SensorCor *sensorCorE = new SensorCor(9, 10, 11);

*/
