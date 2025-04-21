#include <stdint.h>
#include "tm4c123gh6pm.h"
#include "ultrasonic.h"
#include "ir.h"
#include "motor_driver.h"

/*
Used for testing purposes, uses these to check if your sensors are working and reading values properly
#define ULTRA_LED (1U << 1)  // PF1
#define IR_L_LED  (1U << 2)  // PF2
#define IR_R_LED  (1U << 3)  // PF3
*/

//prototyping for variables
float ultra;
int i;
uint32_t sum;

/*
initialization for the testing leds, not compatible with the code for the motor driver, only use this to test the sensors
void LED_Init(void) {
    SYSCTL->RCGCGPIO |= (1U << 5);
    while ((SYSCTL->PRGPIO & (1U << 5)) == 0);
    GPIOF->DIR |= ULTRA_LED | IR_L_LED | IR_R_LED;
    GPIOF->DEN |= ULTRA_LED | IR_L_LED | IR_R_LED;
}
*/

void obstacle_avoidance_algorithm(void){
//Roomba style algorithm
			//Checks for ultrasonic sensor distance, then checks raw values on IR sensors if anything is in front of them and then turns away depending on which is sensor is sensing
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

int main(void) {
	//initializations as main function starts
	//LED_Init();
	Ultrasonic_Init();
	IR_Init();
	Motor_Init();

		while (1) {
			// Measure Ultrasonic distance
			ultra = Measure_Distance();
			delayMicroseconds(5000);//5 ms delay before reading IR to limit electrical noise
			
			obstacle_avoidance_algorithm();
			
    }
}
