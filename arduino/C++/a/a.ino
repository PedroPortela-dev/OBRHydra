class SensorCor{
  public:
    unsigned int valorVermelho = 0;
    unsigned int valorVerde = 0;
    unsigned int valorAzul = 0;
    unsigned int valorBranco = 0;
  
    int pinS2, pinS3, pinOut;
    
    SensorCor(int pinS2, int pinS3, int pinOut){
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
    
      //Verde
      digitalWrite(pinS2, HIGH);
      valorVerde = pulseIn(pinOut, !digitalRead(pinOut));
    }

    String verificador()
    {
      //Detecta a cor
      detectaCor();

      if(valorVermelho <= 5){
        Serial.println("VERMELHO");
        return "Vermelho";
      }else if(valorBranco > 10){
        Serial.println("PRETO");
        return "Preto";
      }else if(valorVermelho > 11){
         Serial.println("VERDE");
         return "Verde";
      }else{
        Serial.println("BRANCO");
        return "Branco";
      }

    
//      if(valorBranco <= 3){
//        Serial.println("Branco");
//        return "Branco";
//      }else
//      if(abs(valorVermelho-valorVerde) < 5){
//        Serial.println("Preto");
//        return "Preto";
//      }else
//      //Verifica se a cor vermelha foi detectada
//      if (valorVermelho < valorVerde) {
//        Serial.println("Vermelho");
//        return "Vermelho";
//      } else 
//      if (valorVerde < valorVermelho)  //Verifica se a cor verde foi detectada
//      {
//        Serial.println("Verde");
//        return "Verde";
//      }
    }

  private:
   
};

SensorCor *corD = new SensorCor(6,7,8);
SensorCor *corE = new SensorCor(5,3,4);
void setup(){
  Serial.begin(9600);
  delay(2000);
}

void loop(){
  //corD->verificador();
  corE->verificador();
  
  Serial.print("VERMELHO: ");
  Serial.println(corE->valorVermelho);
//  Serial.print("VERDE: ");
//  Serial.println(corD->valorVerde);
  Serial.print("BRANCO: ");
  Serial.println(corE->valorBranco);
  delay(500);
  
  
}
