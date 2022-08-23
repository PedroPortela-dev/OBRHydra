#define sensForaD 15
#define sensForaE 14

void setup() {  

  pinMode(sensForaD,INPUT);
  pinMode(sensForaE,INPUT);

  Serial.begin(9600);
}

void loop() {
  Serial.print("Direita: ");
  Serial.println(digitalRead(sensForaD));
  Serial.print("Esquerda: ");
  Serial.println(digitalRead(sensForaE));
  Serial.println();
  delay(500);
  
}
