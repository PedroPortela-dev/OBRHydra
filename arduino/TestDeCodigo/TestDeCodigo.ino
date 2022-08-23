#define sensForaD 8
#define sensForaE 11
#define sensDentroD 9
#define sensDentroE 10

#define PRETOdentro 0
#define BRANCOdentro 1

#define PRETOfora 1
#define BRANCOfora 0

#define POWER 70
#define POWER3 60
#define POWER1 120
#define POWER2 110

#define velmotorD 2
#define md1 3
#define md2 4


#define velmotorE 5
#define me1 6
#define me2 7

unsigned long timer, timer2;

bool seguirLinha = true, curva90 = false;
int velD=0 , velE=0,i =0 ;

class SensorCor{
  public:
    
    unsigned int valorVermelho = 0;
    unsigned int valorVerde = 0;
    unsigned int valorAzul = 0;
    unsigned int valorBranco = 0;
  
    int pinS2, pinS3, pinOut;
    
    SensorCor(int pinOut, int pinS2, int pinS3){
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
    
      //Azul
      digitalWrite(pinS2, LOW);
      digitalWrite(pinS3, HIGH);
      valorAzul = pulseIn(pinOut, !digitalRead(pinOut));
    
      //Verde
      digitalWrite(pinS2, HIGH);
      valorVerde = pulseIn(pinOut, !digitalRead(pinOut));
    }

    bool verificador()
    {
      //Detecta a cor
      detectaCor();
    
      if(valorVermelho-valorVerde > 5){ // Calibragem para verificação do verde
        if(millis()-timer > 100){
          Serial.println("Verde");
          return true; 
        }
        Serial.println("SERA VERDE!");
        return false;
      }
      timer = millis();
      Serial.println("!Verde");
      return false;
    }

  private:
};

SensorCor *corD = new SensorCor(53, 51, 49);
SensorCor *corE = new SensorCor(52, 50, 48);

void printh(){
//  Serial.print("DireitaF :");
//  Serial.println(digitalRead(sensForaD));
//  Serial.print("DireitaD :");
//  Serial.println(digitalRead(sensDentroD));
//  Serial.print("EsquerdaF :");
//  Serial.println(digitalRead(sensForaE));
//  Serial.print("EsquerdaD :");
//  Serial.println(digitalRead(sensDentroE));
//  Serial.println(); corE->verificador();

  Serial.print("COR :");
  corD->verificador();
  Serial.print("BRANCO:");
  Serial.println(corD->valorBranco);
  Serial.print("VERMELHO:");
  Serial.println(corD->valorVermelho);
  Serial.print("VERDE:");
  Serial.println(corD->valorVerde);
  Serial.print("AZUL:");
  Serial.println(corD->valorAzul);
  Serial.println();
}

void mover(int velD, int velE){
  if(velD>0){
    digitalWrite(md1, 1);
    digitalWrite(md2, 0);
    analogWrite(velmotorD,velD);
  }else if(velD<0){
    digitalWrite(md1, 0);
    digitalWrite(md2, 1);
    analogWrite(velmotorD,-(velD));
  }else{
    digitalWrite(md1, 1);
    digitalWrite(md2, 1);
    analogWrite(velmotorD,0);
  }
  
  if(velE>0){
    digitalWrite(me1, 1);
    digitalWrite(me2, 0);
    analogWrite(velmotorE,velE);
  }else if(velE<0){
    digitalWrite(me1, 0);
    digitalWrite(me2, 1);
    analogWrite(velmotorE,-(velE));
  }else{
    digitalWrite(me1, 1);
    digitalWrite(me2, 1);
    analogWrite(velmotorE,0);
  }
}

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

  pinMode(sensForaD,INPUT);
  pinMode(sensForaE,INPUT);
  pinMode(sensDentroD,INPUT);
  pinMode(sensDentroE,INPUT);

  Serial.begin(9600);
}

void verificacaoVerde(){
  
      mover(POWER3,POWER3); //Frente
      delay(300);   // 700
      
      mover(0,0); 
      int i = 0, d = 0, e =0, vetor[d], vetor2[e];
      while(i < 10){
            
        vetor[d] = corD->verificador();
        vetor[e] = corE->verificador();
        d++;
        e++;
        delay(100);
        i++;
      }
      i = 0;
      int conferidor;
      bool a = false;
      while(d < 10){
        conferidor += vetor[d]; 
        if(conferidor > 6){
          a=true;
          break;
        }
      }
      

      mover(-POWER,-POWER); 
      delay(700); //Pra frente ideal = 250
}

void loop() {
  
  while(seguirLinha){
//    time = millis();
//  Serial.print("Direita :");
//  corD->verificador(); 
//  Serial.print("Esquerda :");
//  corE->verificador();
//  Serial.println();
    // Seguir Linha
   // /*
//   if(corD->verificador() == true ){ //|| corE->verificador() == true
//    mover(0,0);
//    printh();
//    delay(5000);
//   }
    if(digitalRead(sensForaD)== PRETOfora && digitalRead(sensForaE)== BRANCOfora){

//      verificacaoVerde();
        mover(POWER,POWER);
        delay(200);
//      if(corD->verificador() == true){
//        mover(0,0);
//        delay(5000);
//      }
      
      //Direita
      mover(-POWER2,POWER1);
      while(digitalRead(sensDentroE) == BRANCOdentro){
//        corD->verificador();
        if(digitalRead(sensForaE)== PRETOfora ){
          //Esquerda
          mover(POWER1,-POWER2);
          while(digitalRead(sensDentroD) == BRANCOdentro){
//            corD->verificador();
          }
          break;
        }
      }
//      timer = millis();
    }
    else if(digitalRead(sensForaE) == PRETOfora && digitalRead(sensForaD)==BRANCOfora){
      
//      verificacaoVerde();

        mover(POWER,POWER);
        delay(200);
//      if(corD->verificador() == true){
//        mover(0,0);
//        delay(5000);
//      }

      //Esquerda
      mover(POWER1,-POWER2);
      //testar continuidade
      while(digitalRead(sensDentroD) == BRANCOdentro){
//        corD->verificador();
        if(digitalRead(sensForaD)== PRETOfora ){
          //Direita 
          mover(-POWER2,POWER1);
          while(digitalRead(sensDentroE) == BRANCOdentro){
//            corD->verificador();
          }
          break;
        }
      }
//      timer = millis();
    }
    else //*/
    if (digitalRead(sensDentroD)==BRANCOdentro && digitalRead(sensDentroE)==PRETOdentro){ 
      //Esquerda  
      mover(POWER1,-POWER1);
      
    }
    
    else if(digitalRead(sensDentroD)== PRETOdentro && digitalRead(sensDentroE)== BRANCOdentro){
      //direita
      mover(-POWER1,POWER1);
    
    }

    else{      
      
      mover(POWER,POWER);
      
    }
    
  
 printh();
// delay(500);
  }    
}

/*
  if(digitalRead(sensForaE)==PRETO && digitalRead(sensForaD)==PRETO){
    Serial.println("Cruz");
    drive->frente(POWER);
    delay(DELAY);
  }
  else if(digitalRead(sensForaE)==PRETO && digitalRead(sensForaD)==BRANCO){
    Serial.println("EstForaE");
    
    drive->frente(POWER);
    delay(DELAY);
  //  timer = millis();
  //  while(millis()-timer>DELAY){
  //    if(digitalRead(sensForaD)==PRETO){
  //        Cruz();
  //        return;
  //    }
  //  }
    
    drive->esquerda(POWERCURVA);
    while(digitalRead(sensDentroD) != PRETO){}
    
  }
  else if(digitalRead(sensForaE)==BRANCO && digitalRead(sensForaD)==PRETO){
    Serial.println("EstForaD");
    
    drive->frente(POWER);
    delay(DELAY);
  //  timer = millis();
  //  while(millis()-timer>DELAY){
  //    if(digitalRead(sensForaE)==PRETO){
  //        Cruz();
  //        return;
  //    }
  //  }
    
    drive->direita(POWERCURVA);
    while(digitalRead(sensDentroE) != PRETO){}
    
  }
  else if (digitalRead(sensDentroE)==BRANCO && digitalRead(sensDentroD)==PRETO){ 
    Serial.println("Dir");
    drive->direita(POWERCURVA);
    
  }
  else if (digitalRead(sensDentroE)==PRETO && digitalRead(sensDentroD)==BRANCO){
    Serial.println("Esq");
    drive->esquerda(POWERCURVA);
    
  }
  else if (digitalRead(sensDentroE)==BRANCO && digitalRead(sensDentroD)==BRANCO){
    Serial.println("Frente");
    drive->frente(POWER);
    
  }
  else if (digitalRead(sensDentroE)==PRETO && digitalRead(sensDentroD)==PRETO){
    Serial.println("Frente");
    drive->frente(POWER);
    
  }*/
