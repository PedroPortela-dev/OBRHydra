#define p1m1 2
#define p2m1 4
#define pvelm1 3
#define velMax 255
#define velMedio 128
#define velParar 0

#define p1m2 9
#define p2m2 8
#define pvelm2 10
#define tmp 100

#define ldrEsq A15
#define ldrDir A14

#define forceMax 255
#define forceMed 128

int vel, erro = 0;

class Motor{
  public:
    int p1,p2,pv;
    Motor(int p1, int p2, int v){
      this->p1=p1;
      this->p2=p2;
      this->pv=v;
      pinMode(p1,OUTPUT);
      pinMode(p2,OUTPUT);
      pinMode(pv,OUTPUT);
      digitalWrite(p1,LOW);
      digitalWrite(p2,LOW);
      digitalWrite(pv,0);
    }
    void frente(int v){
      digitalWrite(p1,HIGH);
      digitalWrite(p2,LOW);
      analogWrite(pv,v);
    }
    void tras(int v){
      digitalWrite(p1,LOW);
      digitalWrite(p2,HIGH);
      analogWrite(pv,v);
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
};

class Driver{
  public:
    Motor *mD;
    Motor *mE;
    
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
};

Motor *md=new Motor(p1m1, p2m1, pvelm1);
Motor *me=new Motor(p1m2, p2m2, pvelm2);
Driver drive(md, me);

void setup(){
  vel=255;
  Serial.begin(9600);
}

void loop(){
  
  erro = analogRead(ldrDir) - analogRead(ldrEsq);
  
  map(erro, 0, 1023, 0, 255);
  
  int velD = forceMed - erro;
  int velE = forceMax + erro;
  
  drive.mover(velD, velE);

  Serial.println(erro);
  Serial.println(velD);
  Serial.println(velE);
    
  delay(tmp);
  
}
