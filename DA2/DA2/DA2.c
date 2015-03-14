/*
 * DA2.c
 * Author: Emmanuel
 */ 
#include <avr/io.h>
#define F_CPU 16000000UL //set clock frequency to 16MHz for compiler

void delayms(int ms); //delay function prototype

int main(void)
{
	DDRB |= (1<<PB5);	//set portb.5 as output
	DDRD |= 0x0F; //make lower 4 pins of port C outputs
	DDRB |= 0x0F; //make lower 4 pins of port C outputs
	DDRD &= ~(1<<PD4); //set portd.4 as input
	
	TCCR0A = 0; //normal mode
	TCCR0B = 7; //external clock on t0 (portb.4)
	TCNT0 = 0;	//initialize counter0 to 0
		
	while(1)
    {
		PORTB &= ~(1<<PB5); //portb.5 off
		delayms(250); //delay for 250 ms
		
		PORTB |= (1<<PB5); //port.5 on
			PORTB = TCNT0>>4;
			PORTD = TCNT0 & 0x0F;
		delayms(250); //delay for 250 ms
    }
	
}

void delayms(int ms)
{
	double tcnt = 65536 - (((double)ms)/1000)*((double)F_CPU/64); //2^16 - delay/clock period

	TCNT1H = ((int)tcnt)>>8;
	TCNT1L = ((int)tcnt) & 0x00FF;
	
	TCCR1A = 0; //WGM normal mode
	TCCR1B = 3; //normal mode, prescaler = 64
	
	while(!(TIFR1 & (1<<TOV1))) //loop until timer overflow happens
	{
	}
	
	TCCR1B = 0; //stop timer 1
	TIFR1 = 1; //clear overflow flag
	
	return;
}