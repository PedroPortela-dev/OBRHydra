//#include <SoftwareSerial.h>
//SoftwareSerial BTserial(10, 11); // RX | TX

#define velmotorD 3
#define md1 4
#define md2 5

#define velmotorE 11
#define me1 12
#define me2 13

#define sensDentroD 7 //sensor dentro diretir
#define sensForaD 8  //sensor fora diretir porta 8 digital 
#define sensDentroE 6 
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

void sentido(bool mdA, bool mdB, bool meA, bool meB){
  
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
  sentido(1,0,1,0);
  velocidade(255,255);
  delay(500);

  //Girando E
  sentido(0,1,1,0);    
  velocidade(255,255);
  delay(3100);

  //Frente
  sentido(0,1,0,1);
  velocidade(255,255);
  delay(1000);
  
  //Girando D
  sentido(1,0,0,1); 
  velocidade(255,255);
  delay(3100);

  //Frente
  sentido(0,1,0,1);
  velocidade(255,255);
  delay(1500);
  
  //Girando D
  sentido(1,0,0,1); 
  velocidade(255,255);
  delay(3100);

  //Frente
  sentido(0,1,0,1);
  velocidade(255,255);
  delay(1000);
  
  //Girando E
  sentido(0,1,1,0);    
  velocidade(255,255);
  delay(3100);
  
}

void loop() {
  
  while(seguirLinha){
    time = millis();

    // Seguir Linha
    
    if((digitalRead(sensDentroD)==0 && digitalRead(sensDentroE)==0) || (digitalRead(sensDentroD)==1 && digitalRead(sensDentroE)==1) ){
      sentido(0,1,0,1); // Frente
      curva90 = false;
      timeInicio = time;
      timeFim = 0;
      velocidade(150,150);
      
    }else if (digitalRead(sensDentroD)==0 && digitalRead(sensDentroE)==1){ 
      sentido(0,1,1,0);    
      velocidade(180,180);
    }else if(digitalRead(sensDentroD)==1 && digitalRead(sensDentroE)==0){
      sentido(1,0,0,1);
      velocidade(180,180);
    }
    
    if(digitalRead(sensDentroD)==1 ^ digitalRead(sensDentroE)==1){
      curva90 = true;
      timeFim = time - timeInicio;
    }
    
    if(curva90 &&  timeFim >=  500){
      if(digitalRead(sensDentroD) == 1){
        sentido(1,0,0,1);
      }else{
        sentido(0,1,1,0);
      }
      
      velocidade(190,190);
      delay(tmp);
      timeFim = 0;  
    }

    // Curva Longa
    /*
    if(digitalRead(sensForaD)==1){
      
    //Girando D
    sentido(1,0,0,1); 
    velocidade(255,255);
    delay(1550);
    
    sentido(0,1,0,1); // Frente
    velocidade(255,255);
    delay(300);
      
    }else if(digitalRead(sensForaE)==1){
      
    //Girando E
    sentido(0,1,1,0);    
    velocidade(255,255);
    delay(3100);

    sentido(0,1,0,1); // Frente
    velocidade(255,255);
    delay(300);
    
    }
    
    // Obstaculo
    if(digitalRead(dist1) == 0){
      //obstaculo();
    }
    */
    
  
    Serial.println(digitalRead(sensDentroD));
    Serial.println(digitalRead(sensForaD));
    Serial.println(digitalRead(sensDentroE));
    Serial.println(digitalRead(sensForaE));
    Serial.println("");
    delay(1000);
  }
  
  //BTserial.println("Erro: " + erro);
  //BTserial.println("Dir: " + analogRead(ldrD));
 // BTserial.println("Esq: " + analogRead(ldrE));
  
}
