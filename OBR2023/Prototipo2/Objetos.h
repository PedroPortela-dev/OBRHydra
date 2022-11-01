#include <HCSR04.h>
#include <Servo.h>
#include "Motor.h"
#include "Driver.h"
#include "SensorCor.h"

Motor *md = new Motor(2,3,4, false);
Motor *me = new Motor(5,6,7, false, 22);
Motor *coleta = new Motor(A15, A14, A13, false);
// Motor *braço = new Motor(A15, A14, A13, false);
// Servo m_despejo;
Driver *drive = new Driver(md,me);
SensorCor *SensorCorD = new SensorCor(53, 51, 49);
SensorCor *SensorCorE = new SensorCor(52, 50, 48);
UltraSonicDistanceSensor distanceSensorDown(A12, A11);
UltraSonicDistanceSensor distanceSensorUp(A6, A5);
UltraSonicDistanceSensor distanceSensorLeft(A10, A9);
UltraSonicDistanceSensor distanceSensorRight(A8, A7);