#include "hcsr04.h"
#include <avr/interrupt.h>
#include <util/atomic.h>

static struct KALMAN k = {.previous_X=1, .previous_P=1, .K=1};

void HCSR04_init(void){
	HCSR04_DDR |= (1 << HCSR04_TRIG_PIN);
	HCSR04_DDR &= ~(1 << HCSR04_ECHO_PIN);
}
volatile uint32_t timer_counter;
ISR(TIMER1_COMPA_vect){
	timer_counter++;
}
uint32_t getTimer(void){
	uint32_t prev = 0;
	ATOMIC_BLOCK(ATOMIC_RESTORESTATE){
		prev = timer_counter;
	}
	return prev;
}
__attribute__((noinline))uint16_t HCSR04_measure(void){
	uint32_t loop_counter=0;
	uint32_t prev_time = getTimer();
	while(ECHO_PIN_CONTROL()){
		if((getTimer() - prev_time) > 300) return 0;
	}
	TRIG_PULSE();
	asm volatile("NOP");
	prev_time = getTimer();
	while(!ECHO_PIN_CONTROL()){
		
		if((getTimer() - prev_time) > 300) return 0;
	}
	while(ECHO_PIN_CONTROL()){
		loop_counter++;
		if(loop_counter > 30000){
			loop_counter=0;
			break;
		}
	}
	 asm volatile("NOP");
	uint32_t command_num = (loop_counter*15)+19;
	uint16_t dist = (uint16_t)(command_num*ConstantForDist);
	dist = kalman_calculate(&k, dist, HCSR04_NOISE_VARIANCE, 10);
	return dist;
}
