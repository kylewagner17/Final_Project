#include "tm4c123gh6pm.h"
#include "Ultrasonic.h"

#define TRIG_PIN  (1U << 2) //GIOPA2
#define ECHO_PIN  (1U << 3) //GPIOA3
#define CLOCK_FREQ 16000000.0f
#define SOUND_SPEED_IN 1130.0f

// variable prototyping
uint32_t time;
float time_sec;
uint32_t cycles;
uint32_t start, end;
uint32_t timeout, waitCount;

void delayMicroseconds(uint32_t us) {
    cycles = (CLOCK_FREQ / 1000000) * us;
    while (cycles--) {
        __asm("nop");
    }
}

//initializing Port A, PA2 = Output, PA3 = Input
void Ultrasonic_Init(void) {
	SYSCTL->RCGCGPIO |= (1U << 0);
	while ((SYSCTL->PRGPIO & (1U << 0)) == 0);
	GPIOA->DIR |= TRIG_PIN;
	GPIOA->DEN |= TRIG_PIN;
	GPIOA->DIR &= ~ECHO_PIN;
	GPIOA->DEN |= ECHO_PIN;

	//Set a basic timer not tied to SYSTICK
	SYSCTL->RCGCTIMER |= (1U << 1);
	while ((SYSCTL->PRTIMER & (1U << 1)) == 0);
	TIMER1->CTL &= ~(1U << 0);
	TIMER1->CFG = 0x00;
	TIMER1->TAMR = 0x02;
	TIMER1->TAILR = 0xFFFFFFFF;
	TIMER1->CTL |= 0x01;
}

//Measures the time it takes for the signal sent out by the sensor to return byt setting the GPIOA data register to the value returned in the echo pin, once for the start time 
static uint32_t Measure_Echo_Time(void) {
	//set a timeout value to keep  the sensor from getting stuck in a loop
	timeout = 160000;
	waitCount = 0;
	TIMER1->CTL &= ~0x01;
	TIMER1->TAV = 0xFFFFFFFF;
	TIMER1->CTL |= 0x01;

	GPIOA->DATA |= TRIG_PIN;
	delayMicroseconds(10);
	GPIOA->DATA &= ~TRIG_PIN;

	while ((GPIOA->DATA & ECHO_PIN) == 0)
		if (++waitCount > timeout) return 0;
	start = TIMER1->TAR;

	waitCount = 0;
	while (GPIOA->DATA & ECHO_PIN)
		if (++waitCount > timeout) return 0;
	end = TIMER1->TAR;

	//ternary conditional operator ?: means (condition) ? (value if true) : (value if false);
	return (start >= end) ? (start - end) : (start + (0xFFFFFFFF - end));
}

//calculates the distance of the signal returning to the ultrasonic sensor based on the time it takes, gives a value that is easier to understand than the raw value
float Measure_Distance(void) {
    time = Measure_Echo_Time();
    if (time == 0) return 999.0f;
    time_sec = time / CLOCK_FREQ;
    return (time_sec * SOUND_SPEED_IN) / 2.0f;
}
