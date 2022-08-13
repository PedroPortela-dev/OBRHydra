#include <HCSR04.h>

#define velmotorD 3
#define md1 4
#define md2 5

#define velmotorE 11
#define me1 12
#define me2 13

#define p_trigger1 3
#define p_trigger2 5
#define p_echo1 2
#define p_echo2 4

UltraSonicDistanceSensor sensorDistance1(p_trigger1,p_echo1);
UltraSonicDistanceSensor sensorDistance2(p_trigger2,p_echo2);

void setup() {
  Serial.begin(9600);

}

void loop() {
  Serial.print("DISTANCE 1: ");
  Serial.println(sensorDistance1.measureDistanceCm());
  Serial.print("DISTANCE 2: ");
  Serial.println(sensorDistance2.measureDistanceCm());
  Serial.println("-------------------------------------------------------");
  delay(500);
}
