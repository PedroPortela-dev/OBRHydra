class SensorCor{
  public:
    
    unsigned int valorVermelho = 0;
    unsigned int valorVerde = 0;
    unsigned int valorAzul = 0;
    unsigned int valorBranco = 0;
  
    int pinS2, pinS3, pinOut;
    
    SensorCor(int pinOut, int pinS2, int pinS3){
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
    
      //Azul
      digitalWrite(pinS2, LOW);
      digitalWrite(pinS3, HIGH);
      valorAzul = pulseIn(pinOut, !digitalRead(pinOut));
    
      //Verde
      digitalWrite(pinS2, HIGH);
      valorVerde = pulseIn(pinOut, !digitalRead(pinOut));
    }

    bool verificador()
    {
      //Detecta a cor
      detectaCor();
    
      if(valorAzul > 8 && valorVermelho > valorVerde && valorVerde > valorAzul && valorAzul > valorBranco){
        Serial.println("Verde");
        return true;
      }
      
      Serial.println("!Verde");
      return false;
    }

  private:
};

SensorCor *corD = new SensorCor(53, 51, 49);
SensorCor *corE = new SensorCor(52, 50, 48);

void setup(){
  delay(2000);
  Serial.begin(9600);
}
  
void loop(){
  corD->verificador();
  if(corD->verificador() == true){
    Serial.print("Verde");
    delay(5000);
  }
//  corE->verificador();
//  Serial.print("BRANCO:");
//  Serial.println(corD->valorBranco);
//  Serial.print("VERMELHO:");
//  Serial.println(corD->valorVermelho);
//  Serial.print("VERDE:");
//  Serial.println(corD->valorVerde);
//  Serial.print("AZUL:");
//  Serial.println(corD->valorAzul);
  Serial.println();
//  delay(500); 
}
