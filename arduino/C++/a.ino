class SensorCor{
  public:
    
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
    
      if(valorBranco <= 4){
        Serial.println("Branco");
        return "Branco";
      }else
      if(abs(valorVermelho-valorVerde) < 5){
        Serial.println("Preto");
        return "Preto";
      }else
      //Verifica se a cor vermelha foi detectada
      if (valorVermelho < valorVerde) {
        Serial.println("Vermelho");
        return "Vermelho";
      } else 
      if (valorVerde < valorVermelho)  //Verifica se a cor verde foi detectada
      {
        Serial.println("Verde");
        return "Verde";
      }
    }

  private:
    unsigned int valorVermelho = 0;
    unsigned int valorVerde = 0;
    unsigned int valorAzul = 0;
    unsigned int valorBranco = 0;
  
    int pinS2, pinS3, pinOut;
};
