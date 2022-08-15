#define sensForaD 8
#define sensForaE 11
#define sensDentroD 9
#define sensDentroE 10

#define PRETOdentro 0
#define BRANCOdentro 1

#define PRETOfora 1
#define BRANCOfora 0

#define POWER 70
#define POWER1 120

#define velmotorD 2
#define md1 3
#define md2 4


#define velmotorE 5
#define me1 6
#define me2 7

unsigned long timer;

bool seguirLinha = true, curva90 = false;
int velD=0 , velE=0,i =0 ;

void printh(){
//  Serial.print("DireitaF :");
//  Serial.println(digitalRead(sensForaD));
  Serial.print("DireitaD :");
  Serial.println(digitalRead(sensDentroD));
//  Serial.print("EsquerdaF :");
//  Serial.println(digitalRead(sensForaE));
  Serial.print("EsquerdaD :");
  Serial.println(digitalRead(sensDentroE));
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

void loop() {
  
  while(seguirLinha){
//    time = millis();

    // Seguir Linha
   // /*
    if(digitalRead(sensForaD)== PRETOfora && digitalRead(sensForaE)== BRANCOfora){
      
      mover(POWER,POWER);
      delay(150);

      //Direita
      mover(-POWER1,POWER1);
      while(digitalRead(sensDentroE) == BRANCOdentro){
        if(digitalRead(sensForaE)== PRETOfora ){
          //Esquerda
          mover(POWER1,-POWER1);
          while(digitalRead(sensDentroD) == BRANCOdentro){};
          break;
        }
      }
//      timer = millis();
    }
    else if(digitalRead(sensForaE) == PRETOfora && digitalRead(sensForaD)==BRANCOfora){
      
      mover(POWER,POWER);
      delay(150);

      //Esquerda
      mover(POWER1,-POWER1);
      //testar continuidade
      while(digitalRead(sensDentroD) == BRANCOdentro){
        if(digitalRead(sensForaD)== PRETOfora ){
          //Direita 
          mover(-POWER1,POWER1);
          while(digitalRead(sensDentroE) == BRANCOdentro){};
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
