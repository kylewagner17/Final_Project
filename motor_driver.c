#include "tm4c123gh6pm.h"
#include "motor_driver.h"

#define GPIO_PORTD_CR  (*((volatile uint32_t *)0x40007524))

void Motor_Init(void) {
    volatile unsigned long delay;
    SYSCTL->RCGCGPIO |= 0x28;         // Enable clock for Ports D and F
    delay = SYSCTL->RCGCGPIO;          // Allow time for clock to stabilize

    // Port D Initialization (IN1-4)
    GPIOD->LOCK = 0x4C4F434B;          // Unlock PD0
    GPIO_PORTD_CR |= 0x0F;             // Allow changes to PD0–PD3
    GPIOD->DIR |= 0x0F;                // Set PD0–PD3 as output
    GPIOD->DEN |= 0x0F;                // Enable digital I/O on PD0–PD3

    // Port F Initialization (ENA and ENB for PWM)
    SYSCTL->RCGCPWM |= 0x02;            // Activate PWM1
    SYSCTL->RCC |= 0x00100000;          // Use PWM divider
    SYSCTL->RCC = (SYSCTL->RCC & ~0x000E0000) | 0x00040000; // Set PWM clock to /2

    GPIOF->AFSEL |= 0x0C;               // Enable alternate function on PF2, PF3
    GPIOF->PCTL &= ~0x0000FF00;         // Clear PF2, PF3 PCTL
    GPIOF->PCTL |= 0x00005500;          // Configure PF2, PF3 for M1PWM6, M1PWM7
    GPIOF->DIR |= 0x0C;                 // Set PF2, PF3 as outputs
    GPIOF->DEN |= 0x0C;                 // Enable digital I/O on PF2, PF3
    GPIOF->DATA |= 0x0C;                // Set PF2, PF3 high
    GPIOF->AMSEL &= ~0x0C;              // Disable analog functionality on PF2, PF3

    PWM1->_3_CTL = 0x00;                // Disable PWM Generator 3
    PWM1->_3_GENA = 0x0000008C;          // Set PF2 PWM output behavior
    PWM1->_3_GENB = 0x0000080C;          // Set PF3 PWM output behavior
    PWM1->_3_LOAD = 7999;               // Set period for 1 kHz PWM
    PWM1->_3_CMPA = 4000;               // 50% duty cycle for M1PWM6 (PF2)
    PWM1->_3_CMPB = 4400;               // 55% duty cycle for M1PWM7 (PF3) depending on your motors you may need to make a slight difference in duty cycle due to one motor moving at a higher speed than the other
    PWM1->_3_CTL |= 0x01;               // Enable PWM Generator 3
    PWM1->ENABLE |= 0xC0;               // Enable M1PWM6 and M1PWM7
}

//Rotates both motors forward
void Motor_Forward(void) {
    GPIOD->DATA = 0x05; // IN1 = 1, IN2 = 0, IN3 = 1, IN4 = 0
}

//Rotates both motors backward
void Motor_Backward(void) {

    GPIOD->DATA = 0x0A; // IN1 = 0, IN2 = 1, IN3 = 0, IN4 = 1
}

//Rotates right motor forward and left motor backwards for a zero point left turn
void Motor_RotateLeft(void) {
    GPIOD->DATA = 0x06; // IN1 = 0, IN2 = 1, IN3 = 1, IN4 = 0
}

//Rotates left motor forward and right motor backwards for a zero point right turn
void Motor_RotateRight(void) {
    GPIOD->DATA = 0x09; // IN1 = 1, IN2 = 0, IN3 = 0, IN4 = 1
}

// Stops the motor rotating in either direction, momentum can continue to push the device forward as the tires will still rotate
void Motor_Stop(void) {
    GPIOD->DATA = 0x00; // IN1 = 0, IN2 = 0, IN3 = 0, IN4 = 0
}

// Actively holds the motor/tires from rotating, tires should not rotate due to momentum
void Motor_Brake(void) {
	GPIOD->DATA = 0x0F;   // IN1 = 1, IN2 = 1, IN3 = 1, IN4 = 1
}

