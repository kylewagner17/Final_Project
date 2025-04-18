#ifndef IR_SENSOR_H
#define IR_SENSOR_H

#include <stdint.h>

void IR_Init(void);
void ADC0_SetChannel(uint8_t channel);
uint16_t ADC0_Read(void);
float IR_Distance_cm(uint16_t adc_value);

extern float distanceL, distanceR;
extern uint16_t adcL, adcR;

#endif
