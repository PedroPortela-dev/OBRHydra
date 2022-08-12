#define velmotorD 3
#define md1 4
#define md2 5

#define velmotorE 11
#define me1 12
#define me2 13

#define sensDentroD 6
#define sensForaD 8
#define sensDentroE 7
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
    digitalWrite(md1, 0);
    digitalWrite(md2, 1);
    analogWrite(velmotorD,velD);
  }else if(velD<0){
    digitalWrite(md1, 1);
    digitalWrite(md2, 0);
    analogWrite(velmotorD,-velD);
  }else{
    digitalWrite(md1, 1);
    digitalWrite(md2, 1);
    analogWrite(velmotorD,0);
  }
  
  if(velE>0){
    digitalWrite(me1, 0);
    digitalWrite(me2, 1);
    analogWrite(velmotorE,velE);
  }else if(velE<0){
    digitalWrite(me1, 1);
    digitalWrite(me2, 0);
    analogWrite(velmotorE,-velE);
  }else{
    digitalWrite(me1, 1);
    digitalWrite(me2, 1);
    analogWrite(velmotorE,0);
  }
}

void sentido(bool mdB, bool mdA, bool meB, bool meA){
  
  digitalWrite(md1,mdA);
  digitalWrite(md2,mdB);
  
  digitalWrite(me1,meA);
  digitalWrite(me2,meB);
 
}

void velocidade(int velD, int velE){
    analogWrite(velmotorD,velD);
    analogWrite(velmotorE,velE); 
}

void obstaculo(){

  //Tras
  mover(-255,-255);
  delay(500);

  //Girando E    
  mover(255,-255);
  delay(3100);

  //Frente
  mover(255,255);
  delay(1000);
  
  //Girando D
  mover(-255,255);
  delay(3100);

  //Frente
  mover(255,255);
  delay(1500);
  
  //Girando E
  mover(255,-255);
  delay(3100);

  //Frente
  mover(255,255);
  delay(1000);
  
  //Girando D    
  mover(255,255);
  delay(3100);
  
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

//    if(verde && millis()-timer>400){
//      fazer o verde
//    }
  
}

/*
  while(seguirLinha){
    time = millis();

    // Seguir Linha
    if((digitalRead(sensForaD)==0 && digitalRead(sensForaE)==0 && digitalRead(sensDentroD)==0 && digitalRead(sensDentroE)==0) || (digitalRead(sensForaD)==1 && digitalRead(sensForaE)==1 && digitalRead(sensDentroD)==1 && digitalRead(sensDentroE)==1)){
      sentido(0,1,0,1); // Frente
      velocidade(150,150);   
    }
    
    else if(digitalRead(sensForaD)==1 && digitalRead(sensForaE)==0){
      //Tras      
      sentido(1,0,1,0);
      velocidade(120,120);
      delay(150);
      
      sentido(0,1,1,0);     
      velocidade(120,120);
      delay(400); 
       
      i++;
      if(millis() - timeInicio >= 300 && i >= 2 && curva90){
        curvaDireita();
      }
      
    }
    
    else if(digitalRead(sensForaE)==1 && digitalRead(sensForaD)==0){
      //Tras
      sentido(1,0,1,0);
      velocidade(120,120);
      delay(150);
      
      sentido(1,0,0,1);
      velocidade(120,120);
      delay(400);   
      
      i++;
      if(time - timeInicio >= 300 && i >= 2 && curva90){
        curvaEsquerda();
      }
    }else if (digitalRead(sensDentroD)==0 && digitalRead(sensDentroE)==1){ 
      sentido(0,1,1,0);   
      velocidade(120,120);
    }else if(digitalRead(sensDentroD)==1 && digitalRead(sensDentroE)==0){
      sentido(1,0,0,1);
      velocidade(120,120);
    }
    
    // Curva Longa
    if(digitalRead(sensForaD) == 1 ^ digitalRead(sensForaE) == 1){
      if(!curva90){
        i=0;
        timeInicio = time;
        curva90 = true;
      }
    }
    
    if(curva90 &&  time - timeInicio >= 2000){
        i=0;
        timeInicio=0;
        curva90=false;
    }

    
    

    // Obstaculo
    if(digitalRead(dist1) == 0){
      //obstaculo();
    }
    
  */
}