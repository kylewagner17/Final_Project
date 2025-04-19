#include <stdint.h>
#include "tm4c123gh6pm.h"
#include "ultrasonic.h"
#include "ir.h"
#include "motor_driver.h"

/*
#define ULTRA_LED (1U << 1)  // PF1
#define IR_L_LED  (1U << 2)  // PF2
#define IR_R_LED  (1U << 3)  // PF3
*/

float ultra;

/*
void LED_Init(void) {
    SYSCTL->RCGCGPIO |= (1U << 5);
    while ((SYSCTL->PRGPIO & (1U << 5)) == 0);
    GPIOF->DIR |= ULTRA_LED | IR_L_LED | IR_R_LED;
    GPIOF->DEN |= ULTRA_LED | IR_L_LED | IR_R_LED;
}
*/

int main(void) {
    //LED_Init();
    Ultrasonic_Init();
    IR_Init();
	Motor_Init();

    while (1) {
			
        // Measure Ultrasonic distance
        ultra = Measure_Distance();
        delayMicroseconds(100000);
        if (ultra < 1.0f)
            Motor_Backward();
        else
            Motor_Forward();
/*
        // Measure IR distances
        ADC0_SetChannel(0); adcL = ADC0_Read();
        ADC0_SetChannel(1); adcR = ADC0_Read();

        distanceL = (float)adcL;
        distanceR = (float)adcR;

        if (distanceL < 7.0f)
				Motor_RotateLeft();
        else
            Motor_Forward();

        if (distanceR < 7.0f)
            Motor_RotateRight();
        else
            Motor_Forward(); */
    }
}
