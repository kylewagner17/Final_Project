
#ifndef ULTRASONIC_SENSOR_H
#define ULTRASONIC_SENSOR_H
#include "tm4c123gh6pm.h"
void Ultrasonic_Init(void);
float Measure_Distance(void);
void delayMicroseconds(uint32_t us);

#endif
