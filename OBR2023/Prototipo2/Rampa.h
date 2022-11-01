#include<Wire.h>
#include "Variaveis.h"
#include "Objetos.h"

void verificacaoMPU(){
  MPUloop();
  if(Subida){
    Serial.print("Subida");
    // if(distD < 10 && distD > 1 && distE < 10 && distE > 1){
    //   Rampa = true;
    // }
    POWER = 220;
  }
  // else if(Descendo && !Rampa){
  //   POWER = 0;
  // }
  // else if(!Subida && Rampa){
  //   Sala3 = true;
  // }
  // else{
  //   POWER = 70;
  // }
}

void MPUsetup() {
  Serial.begin(9600);

  Wire.begin();
  Wire.beginTransmission(MPU);
  Wire.write(0x6B);
  Wire.write(0);
  Wire.endTransmission(true);

  Wire.beginTransmission(MPU);
  Wire.write(0x1B);
  Wire.write(0x00000000);
  Wire.endTransmission();

  Wire.beginTransmission(MPU);
  Wire.write(0x1C);
  Wire.write(0b00000000);
  Wire.endTransmission();
}

void MPUloop() {
  
  Wire.beginTransmission(MPU);
  Wire.write(0x3B);
  Wire.endTransmission(false);
  Wire.requestFrom(MPU, 14, true);

  AccX = Wire.read() << 8 | Wire.read();
  AccY = Wire.read() << 8 | Wire.read();
  AccZ = Wire.read() << 8 | Wire.read();
  Temp = Wire.read() << 8 | Wire.read();
  GyrX = Wire.read() << 8 | Wire.read(); 
  GyrY = Wire.read() << 8 | Wire.read(); 
  GyrZ = Wire.read() << 8 | Wire.read(); 
  AccX /= 16384;
  AccY /= 16384;
  AccZ /= 16384;
  GyrX /= 131;
  GyrY /= 131;
  GyrZ /= 131;

  DerivadaGyrZ = (UltimoValorGyrZ - GyrZ)/0.01;
  UltimoValorGyrZ = GyrZ;

  MaxDerivadaGyrZ = (DerivadaGyrZ > MaxDerivadaGyrZ)? DerivadaGyrZ:MaxDerivadaGyrZ;
  if(MaxDerivadaGyrZ > 5000){
    Subida = true;
    MaxDerivadaGyrZ = 0; 
  }
  // Serial.print("MaxDerivadaGyrZ");
  // Serial.println(MaxDerivadaGyrZ);

  ultimosValoresGyrZ[i] = GyrZ;
  if(i < 20){
    i++;
  }else{
    i = 0;
  }

  for(int j = 0; j < 20; j++){
    MediaGyrZ += ultimosValoresGyrZ[j];
  }
  MediaGyrZ /= 20;

  if(abs(MediaGyrZ) < -10){
    Descendo = true; 
  }else{
    Descendo = false;
    if(Descendo){
      Subida = false;
    }
  }
  delay(10);
}