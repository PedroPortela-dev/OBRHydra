#include "Variaveis.h"
#include "Objetos.h"
#include "Sala1.h"
#include "Sala3.h"

void setup() {
  Serial.begin(9600);
  // m_despejo.attach(9);
  MPUsetup();
  Serial.availableForWrite();
}

void loop() {
  caractere = Serial.read();
  switch (caractere)
  {
  case 'a':
    sala1();
    break;
  
  case 'b':
    loopSala3();
  default:
  }
  
}