#ifndef SENSORCOR_H_INCLUDED
#define SENSORCOR_H_INCLUDED


#define sensForaD !digitalRead(8)
#define sensForaE !digitalRead(10)
#define sensDentroD digitalRead(9)
#define sensDentroE digitalRead(11)

#define PRETO 1
#define BRANCO 0
int POWER = 70;
#define POWER1 200
#define DELAY 200
#define DELAY1 200
#define DELAY2 1700
// #define P_NEUTRA m_despejo.write(0)
// #define P_Despejo m_despejo.write(90)

unsigned long timer, timer2;

String corD, corE;
float distB, distC,distD, distE;

const int MPU = 0x68;
float AccX, AccY, AccZ, Temp, GyrX, GyrY, GyrZ, UltimoValorGyrZ, DerivadaGyrZ, MinDerivadaGyrZ, MaxDerivadaGyrZ, MediaGyrZ, MaxMediaGyrZ;
float ultimosValoresGyrZ[20];
int i = 0, j = 0;
bool Subida = false, Descendo = false, Rampa = false, Sala3 = false;

float distVitima;
int lado = 1;
String entrada = ".";
boolean despejei = false;
int despejos =0;
bool voltar = false;
float distparedeD = 0;
float distparedeE = 0;
bool entrei = false;
int comecar = 0;

#endif