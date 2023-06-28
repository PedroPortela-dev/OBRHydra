// Inclusão das Bibliotecas
#include<Wire.h>

#define g 9.80665;

const int MPU = 0x68;

double AccX, AccY, AccZ, Temp, GyrX, GyrY, GyrZ;
double AccX_Media, AccY_Media, AccZ_Media,  GyrX_Media, GyrY_Media, GyrZ_Media;
double VlcX, VlcY, VlcZ, AngX, AngY, AngZ, distX, distY, distZ;
double deltat;
double maxGyrX, maxGyrY, maxGyrZ, maxAccX, maxAccY, maxAccZ; 

unsigned long timer, i;

void setup() {
  Serial.begin(9600);
  configMPU();
  setMedia();
  timer = micros();
}

void loop() {
  
  setMPU();
  printMPU();
}

double map(double valor, double AtualInf, double AtualSup, double DestInf, double DestSup){
  double a = (DestSup - DestInf)/(AtualSup - AtualInf);
  return a*(valor-AtualInf) + DestInf;
}

void configMPU(){
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
  Wire.write(0b00001000);  // Trocar esse comando para fundo de escala desejado conforme acima
  Wire.endTransmission();
}

void MPURead(){
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
}

void setMedia(){
  timer = micros();

  do{

    MPURead();
    
    GyrX_Media+=GyrX;
    GyrY_Media+=GyrY;
    GyrZ_Media+=GyrZ;

    i++;
    
    deltat = (micros()-timer)/1000000.0;
    Serial.println(deltat);
  }while(deltat < 5);

  GyrX_Media/=i;
  GyrY_Media/=i;
  GyrZ_Media/=i;
}

void setMPU(){
  MPURead();
  
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

  GyrX = map(GyrX, -32768.0, 32767.0, -250.0, 250.0);
  GyrY = map(GyrY, -32768.0, 32767.0, -250.0, 250.0);
  GyrZ = map(GyrZ, -32768.0, 32767.0, -250.0, 250.0);

  GyrX = (abs(GyrX) < 0.25)? 0: GyrX;
  GyrY = (abs(GyrY) < 0.25)? 0: GyrY;
  GyrZ = (abs(GyrZ) < 0.25)? 0: GyrZ;

  deltat = (micros()-timer)/1000000.0;

  AngX += GyrX*deltat;
  AngY += GyrY*deltat;
  AngZ += GyrZ*deltat;

  maxGyrX = max(maxGyrX, GyrX);
  maxGyrY = max(maxGyrY, GyrY);
  maxGyrZ = max(maxGyrZ, GyrZ);

  timer = micros();
}

void printMPU(){
  Serial.print("Deltat:");
  Serial.print((micros()-timer));
  Serial.print("\t");

//  Serial.print("AccX:");
//  Serial.print(AccX);
//  Serial.print("\t");
//  Serial.print("AccY:");
//  Serial.print(AccY);
//  Serial.print("\t");
//  Serial.print("AccZ:");
//  Serial.print(AccZ);
//  Serial.print("\t");
//
//  Serial.print("GyrX:");
//  Serial.print(GyrX);
//  Serial.print("\t");
//  Serial.print("GyrY:");
//  Serial.print(GyrY);
//  Serial.print("\t");
  Serial.print("GyrZ:");
  Serial.print(GyrZ);
  Serial.print("\t");

//  Serial.print("AngX:");
//  Serial.print(AngX);
//  Serial.print("\t");
//  Serial.print("AngY:");
//  Serial.print(AngY);
//  Serial.print("\t");
  Serial.print("AngZ:");
  Serial.print(AngZ);
  Serial.print("\t");
//
//  Serial.print("VlcX:");
//  Serial.print(VlcX);
//  Serial.print("\t");
//  Serial.print("VlcY:");
//  Serial.print(VlcY);
//  Serial.print("\t");
//  Serial.print("VlcZ:");
//  Serial.print(VlcZ);
//  Serial.print("\t");

//  Serial.print("distX:");
//  Serial.print(distX);
//  Serial.print("\t");
//  Serial.print("distY:");
//  Serial.print(distY);
//  Serial.print("\t");
//  Serial.print("distZ:");
//  Serial.print(distZ);

//  Serial.print("maxGyrX");
//  Serial.print(maxGyrX);
//  Serial.print("\t");
//  Serial.print("maxGyrY");
//  Serial.print(maxGyrY);
//  Serial.print("\t");
  Serial.print("maxGyrZ");
  Serial.print(maxGyrZ);
  Serial.print("\t");
//
//  Serial.print("maxAccX");
//  Serial.print(maxAccX);
//  Serial.print("\t");
//  Serial.print("maxAccY");
//  Serial.print(maxAccY);
//  Serial.print("\t");
//  Serial.print("maxAccZ");
//  Serial.print(maxAccZ);
//  Serial.print("\t");

  Serial.println();
}
