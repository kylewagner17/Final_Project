#include "tm4c123gh6pm.h"
#include "IR.h"

float distanceL, distanceR;
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


