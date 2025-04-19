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
int i;

/*
void LED_Init(void) {
    SYSCTL->RCGCGPIO |= (1U << 5);
    while ((SYSCTL->PRGPIO & (1U << 5)) == 0);
    GPIOF->DIR |= ULTRA_LED | IR_L_LED | IR_R_LED;
    GPIOF->DEN |= ULTRA_LED | IR_L_LED | IR_R_LED;
}
*/
float get_average_ir(uint8_t channel) {
    uint32_t sum = 0;
    for (i = 0; i < 4; i++) {
        ADC0_SetChannel(channel);
        sum += ADC0_Read();
        delayMicroseconds(500); // Small delay between samples
    }
    return (float)(sum / 4);
}

int main(void) {
	//LED_Init();
	Ultrasonic_Init();
	IR_Init();
	Motor_Init();

    while (1) {
			
			// Measure Ultrasonic distance
			ultra = Measure_Distance();
			delayMicroseconds(5000);//5 ms delay before reading IR to limit electrical noise
			
			//Roomba style algorithm
			if (ultra < 1.0f){
				Motor_Brake();
				ADC0_SetChannel(0); adcL = ADC0_Read();
				ADC0_SetChannel(1); adcR = ADC0_Read();
				delayMicroseconds(100000);
				if (adcL < 1000){
					Motor_RotateRight();
					delayMicroseconds(200000);
				}
				else if (adcR < 1000){
					Motor_RotateLeft();
					delayMicroseconds(200000);
				}
				else if (adcR < 1000 && adcL < 1000){
					Motor_RotateLeft();
					delayMicroseconds(200000);
				}
				else {
					Motor_RotateRight();
					delayMicroseconds(200000);
				}
			}
			else{
				Motor_Forward();
			}
    }
}
