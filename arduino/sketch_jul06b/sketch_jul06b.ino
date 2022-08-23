// Programa : Pique um LED
// Autor : FILIPEFLOP
#define ledR 7
#define ldr A15

void setup()
{
  //Define a porta do led como saida
  pinMode(ledR, OUTPUT);
  pinMode(ldr, INPUT);
  Serial.begin(9600);
}
  
void loop()
{
  //Acende o led
  digitalWrite(ledR, HIGH);

  Serial.println(analogRead(ldr));
  //Aguarda intervalo de tempo em milissegundos
  delay(100);
    
}
