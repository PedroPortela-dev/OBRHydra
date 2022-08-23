#define sensForaD 8
#define sensForaE 11
#define sensDentroD 9
#define sensDentroE 10


void setup() {
  pinMode(sensDentroD,INPUT);
  pinMode(sensForaD,INPUT);
  pinMode(sensDentroE,INPUT);
  pinMode(sensForaE,INPUT);

  Serial.begin(9600);
}

void loop() {
  Serial.print("SENSOR FORA D: ");
  Serial.println( digitalRead(sensForaD));
  Serial.print("SENSOR FORA E: ");
  Serial.println( digitalRead(sensForaE));
  Serial.print("SENSOR DENTRO D: ");
  Serial.println( digitalRead(sensDentroD));
  Serial.print("SENSOR DENTRO E: ");
  Serial.println( digitalRead(sensDentroE));
  Serial.println();
  delay(500);
  // put your main code here, to run repeatedly:

}
