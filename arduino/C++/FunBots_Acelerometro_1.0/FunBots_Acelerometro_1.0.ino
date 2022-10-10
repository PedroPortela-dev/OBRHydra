// Inclusão das Bibliotecas
#include<Wire.h>
#include<math.h>

const int MPU = 0x68;

float AccX, AccY, AccZ, Temp, GyrX, GyrY, GyrZ;
float AccX_Media, AccY_Media, AccZ_Media,  GyrX_Media, GyrY_Media, GyrZ_Media;
float VlcX, VlcY, VlcZ, AngX, AngY, AngZ, distX, distY, distZ;

unsigned long timer, i, DeltaT;

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
  Wire.write(0b00000000);  // Trocar esse comando para fundo de escala desejado conforme acima
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

  timer = micros();
  DeltaT = (micros()-timer)/(double)1000000;

  do{
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

    AccX_Media+=AccX;
    AccY_Media+=AccY;
    AccZ_Media+=AccZ;
    GyrX_Media+=GyrX;
    GyrY_Media+=GyrY;
    GyrZ_Media+=GyrZ;

    i++;
  }while(DeltaT < 5);

  AccX_Media/=i;
  AccY_Media/=i;
  AccZ_Media/=i;
  GyrX_Media/=i;
  GyrY_Media/=i;
  GyrZ_Media/=i;

  timer = micros();
}

void loop() {

  DeltaT = (micros()-timer)/(double)1000000;
  
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

  AccX-=AccX_Media;
  AccY-=AccY_Media;
  AccZ-=AccZ_Media;
  GyrX-=GyrX_Media;
  GyrY-=GyrY_Media;
  GyrZ-=GyrZ_Media;

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

  AccX /= 16384;
  AccY /= 16384;
  AccZ /= 16384;

  AccX *= 9.8;
  AccY *= 9.8;
  AccZ *= 9.8;

  GyrX /= 131;
  GyrY /= 131;
  GyrZ /= 131;

//  distX += VlcX*DeltaT + AccX*pow(DeltaT, 2)/2;
//  distY += VlcY*DeltaT + AccX*pow(DeltaT, 2)/2;
//  distZ += VlcZ*DeltaT + AccX*pow(DeltaT, 2)/2;

  VlcX += AccX*DeltaT;
  VlcY += AccY*DeltaT;
  VlcZ += AccZ*DeltaT;

  AngX += GyrX*DeltaT;
  AngY += GyrY*DeltaT;
  AngZ += GyrZ*DeltaT;

  timer = micros();
  
  Serial.print("AngX:");
  Serial.print(AngX);
  Serial.print("\t");
  Serial.print("AngY:");
  Serial.print(AngY);
  Serial.print("\t");
  Serial.print("AngZ:");
  Serial.print(AngZ);
  Serial.print("\t");

  Serial.print("VlcX:");
  Serial.print(VlcX);
  Serial.print("\t");
  Serial.print("VlcY:");
  Serial.print(VlcY);
  Serial.print("\t");
  Serial.print("VlcZ:");
  Serial.print(VlcZ);
  
  Serial.println();
}
