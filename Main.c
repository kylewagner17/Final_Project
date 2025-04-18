#include <stdint.h>
#include "tm4c123gh6pm.h"
#include "ultrasonic.h"
#include "ir.h"

#define ULTRA_LED (1U << 1)  // PF1
#define IR_L_LED  (1U << 2)  // PF2
#define IR_R_LED  (1U << 3)  // PF3

void LED_Init(void) {
    SYSCTL->RCGCGPIO |= (1U << 5);
    while ((SYSCTL->PRGPIO & (1U << 5)) == 0);
    GPIOF->DIR |= ULTRA_LED | IR_L_LED | IR_R_LED;
    GPIOF->DEN |= ULTRA_LED | IR_L_LED | IR_R_LED;
}

int main(void) {
    LED_Init();
    Ultrasonic_Init();
    IR_Init();

    while (1) {
        // Measure Ultrasonic distance
        float ultra = Measure_Distance();
        delayMicroseconds(100000);
        if (ultra < 1.0f)
            GPIOF->DATA |= ULTRA_LED;
        else
            GPIOF->DATA &= ~ULTRA_LED;

        // Measure IR distances
        ADC0_SetChannel(0); adcL = ADC0_Read();
        ADC0_SetChannel(1); adcR = ADC0_Read();

        distanceL = IR_Distance_cm(adcL);
        distanceR = IR_Distance_cm(adcR);

        if (distanceL < 7.0f)
            GPIOF->DATA |= IR_L_LED;
        else
            GPIOF->DATA &= ~IR_L_LED;

        if (distanceR < 7.0f)
            GPIOF->DATA |= IR_R_LED;
        else
            GPIOF->DATA &= ~IR_R_LED;
    }
}
