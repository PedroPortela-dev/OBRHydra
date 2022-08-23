#define motorD1 12
#define motorD2 13
#define velMotorD 11

#define motorE1 2
#define motorE2 4
#define velMotorE 3

#define velMax 255
#define velMedio 128
#define velParar 0
#define tmp 100

#define ldrEsq A15
#define ldrDir A14

#define forceMax 255
#define forceMed 128

int velD = 0, velE = 0, erro = 0;

void setup(){

  pinMode(motorD1,OUTPUT);
  pinMode(motorD2,OUTPUT);
  
  pinMode(motorE1,OUTPUT);
  pinMode(motorE2,OUTPUT);

  digitalWrite(motorD1, LOW);
  digitalWrite(motorD2, LOW);
  
  digitalWrite(motorE1, LOW);
  digitalWrite(motorE2, LOW);

  analogWrite(velMotorD, velD);
  analogWrite(velMotorE, velE);
  
  Serial.begin(9600);
}

void loop(){
  
  erro = analogRead(ldrDir) - analogRead(ldrEsq);
  
  map(erro, 0, 1023, 0, 255);
  
  velD = forceMed - erro;
  velE = forceMax + erro;

  digitalWrite(motorD1, HIGH);
  digitalWrite(motorD2, LOW);
  
  digitalWrite(motorE1, HIGH);
  digitalWrite(motorE2, LOW);

  analogWrite(velMotorD, velD);
  analogWrite(velMotorE, velE);
  
  Serial.println(erro + velD + velE);
   
  delay(tmp);
  
}
