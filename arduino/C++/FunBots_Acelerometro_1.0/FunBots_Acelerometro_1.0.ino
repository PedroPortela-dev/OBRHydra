// Inclusão das Bibliotecas
#include<Wire.h>

const int MPU = 0x68;

float AccX, AccY, AccZ, Temp, GyrX, GyrY, GyrZ, UltimoValorGyrZ, DerivadaGyrZ, MinDerivadaGyrZ, MaxDerivadaGyrZ, MediaGyrZ, MaxMediaGyrZ;
float ultimosValoresGyrZ[20];
int i;

void setup() {
  Serial.begin(9600);

  // Inicializa o MPU-6050
  Wire.begin();
  Wire.beginTransmission(MPU);
  Wire.write(0x6B);
  Wire.write(0);
  Wire.endTransmission(true);

  // Configura Giroscópio para fundo de escala desejado
  /*
    Wire.write(0b00000000); // fundo de escala em +/-250°/s
    Wire.write(0b00001000); // fundo de escala em +/-500°/s
    Wire.write(0b00010000); // fundo de escala em +/-1000°/s
    Wire.write(0b00011000); // fundo de escala em +/-2000°/s
  */
  Wire.beginTransmission(MPU);
  Wire.write(0x1B);
  Wire.write(0x00000000);  // Trocar esse comando para fundo de escala desejado conforme acima
  Wire.endTransmission();

  // Configura Acelerometro para fundo de escala desejado
  /*
      Wire.write(0b00000000); // fundo de escala em +/-2g
      Wire.write(0b00001000); // fundo de escala em +/-4g
      Wire.write(0b00010000); // fundo de escala em +/-8g
      Wire.write(0b00011000); // fundo de escala em +/-16g
  */
  Wire.beginTransmission(MPU);
  Wire.write(0x1C);
  Wire.write(0b00000000);  // Trocar esse comando para fundo de escala desejado conforme acima
  Wire.endTransmission();

  pinMode(LED_BUILTIN, OUTPUT);
}

void loop() {
  // Comandos para iniciar transmissão de dados
  Wire.beginTransmission(MPU);
  Wire.write(0x3B);
  Wire.endTransmission(false);
  Wire.requestFrom(MPU, 14, true); // Solicita os dados ao sensor

  // Armazena o valor dos sensores nas variaveis correspondentes
  AccX = Wire.read() << 8 | Wire.read(); //0x3B (ACCEL_XOUT_H) & 0x3C (ACCEL_XOUT_L)
  AccY = Wire.read() << 8 | Wire.read(); //0x3D (ACCEL_YOUT_H) & 0x3E (ACCEL_YOUT_L)
  AccZ = Wire.read() << 8 | Wire.read(); //0x3F (ACCEL_ZOUT_H) & 0x40 (ACCEL_ZOUT_L)
  Temp = Wire.read() << 8 | Wire.read(); //0x41 (TEMP_OUT_H) & 0x42 (TEMP_OUT_L)
  GyrX = Wire.read() << 8 | Wire.read(); //0x43 (GYRO_XOUT_H) & 0x44 (GYRO_XOUT_L)
  GyrY = Wire.read() << 8 | Wire.read(); //0x45 (GYRO_YOUT_H) & 0x46 (GYRO_YOUT_L)
  GyrZ = Wire.read() << 8 | Wire.read(); //0x47 (GYRO_ZOUT_H) & 0x48 (GYRO_ZOUT_L)
  AccX /= 16384;
  AccY /= 16384;
  AccZ /= 16384;
  GyrX /= 131;
  GyrY /= 131;
  GyrZ /= 131;

  // Imprime na Serial os valores obtidos
  /* Alterar divisão conforme fundo de escala escolhido:
      Acelerômetro
      +/-2g = 16384
      +/-4g = 8192
      +/-8g = 4096
      +/-16g = 2048

      Giroscópio
      +/-250°/s = 131
      +/-500°/s = 65.6
      +/-1000°/s = 32.8
      +/-2000°/s = 16.4
  */
  DerivadaGyrZ = (UltimoValorGyrZ - GyrZ)/0.01;
  UltimoValorGyrZ = GyrZ;
  
  MaxDerivadaGyrZ = (DerivadaGyrZ > MaxDerivadaGyrZ)? DerivadaGyrZ:MaxDerivadaGyrZ;
  MinDerivadaGyrZ = (DerivadaGyrZ < MinDerivadaGyrZ)? DerivadaGyrZ:MinDerivadaGyrZ;
  if(MaxDerivadaGyrZ > 5000){
    digitalWrite(LED_BUILTIN, HIGH);   // turn the LED on (HIGH is the voltage level)
    delay(5000);
    digitalWrite(LED_BUILTIN, LOW);
    MaxDerivadaGyrZ = 0; 
  }
  if(MinDerivadaGyrZ < -5000){
    for(int i = 0; i < 5; i++){
      digitalWrite(LED_BUILTIN, HIGH);   // turn the LED on (HIGH is the voltage level)
      delay(1000);
      digitalWrite(LED_BUILTIN, LOW);
      delay(1000);
    }
    MinDerivadaGyrZ = 0; 
  }

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
    Serial.print("1");
    digitalWrite(LED_BUILTIN, HIGH); 
  }else{
    Serial.print("0");
    digitalWrite(LED_BUILTIN, LOW);
  }
  
  Serial.print("MediaGyrZ: ");
  Serial.println(MediaGyrZ);
  delay(10);
}
