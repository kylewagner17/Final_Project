#include "tm4c123gh6pm.h"
#include "IR.h"

float distanceL, distanceR;
float voltage;
float d;
uint16_t result;
uint16_t adcL, adcR;

void IR_Init() {
    SYSCTL->RCGCADC |= 0x01;
    SYSCTL->RCGCGPIO |= 0x10;
    while ((SYSCTL->PRGPIO & 0x10) == 0) {}

    GPIOE->AFSEL |= 0x0C;
    GPIOE->DEN &= ~0x0C;
    GPIOE->AMSEL |= 0x0C;

    ADC0->ACTSS &= ~0x08;
    ADC0->EMUX &= ~0xF000;
    ADC0->SSCTL3 = 0x06;
    ADC0->ACTSS |= 0x08;
}

void ADC0_SetChannel(uint8_t channel) {
    ADC0->ACTSS &= ~0x08;
    ADC0->SSMUX3 = channel;
    ADC0->ACTSS |= 0x08;
}

uint16_t ADC0_Read(void) {
    ADC0->PSSI = 0x08;
    while ((ADC0->RIS & 0x08) == 0);
    result = ADC0->SSFIFO3 & 0xFFF;
    ADC0->ISC = 0x08;
    return result;
}

float IR_Distance_cm(uint16_t adc_value) {
    voltage = (adc_value * 3.3f) / 4095.0f;
    if (voltage < 0.4f) voltage = 0.4f;
    if (voltage > 3.0f) voltage = 3.0f;
    d = 27.86f / (voltage - 0.1f);
    if (d < 0.0f) d = 0.0f;
    if (d > 80.0f) d = 80.0f;
    return d;
}
