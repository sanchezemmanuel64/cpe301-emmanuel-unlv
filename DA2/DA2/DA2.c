/*
 * DA2.c
 * Author: Emmanuel
 */ 

#include <avr/io.h>
#include <avr/interrupt.h>
#define F_CPU 16000000UL //set clock frequency to 16MHz for compiler

void delayms(int ms); //delay function prototype

int main(void)
{
	DDRB |= (1<<PB5);	//set portb.5 as output
	DDRB |= (1<<PB4);   //toggles every 5 rising edges
	
	DDRC |= 0x0F; //make lower 4 pins of port C outputs
	DDRD |= 0x0F; //make lower 4 pins of port D outputs
	DDRD &= ~(1<<PD4); //set portd.4 as input
	
	TCCR0A = 0; //timer 0 in normal mode
	TCCR0B = 7; //external clock on t0 (portb.4)
	TCNT0 = 0;	//initialize counter0 to 0
	
	PCICR |= (1<<PCIE0); //needed for using PCINT5 (PB5)
	PCIFR |= (1<<PCIF0); //needed for using PCINT5 (PB5)
	PCMSK0 |= (1<<PCINT5); //using PCINT5 because interrupt will be caused by PB5
	sei(); //enable interrupts
		
	while(1) //generate waveform of 50% DC with 0.5 sec period
    {		
		PORTB &= ~(1<<PB5); //portb.5 off
		delayms(250); //delay for 250 ms
		
		PORTB |= (1<<PB5); //port.5 on
			PORTD = TCNT0>>4; //move upper nibble of counter into portd
			PORTC = TCNT0 & 0x0F; //move lower nibble of counter into portc
		delayms(250); //delay for 250 ms
    }
	
}

void delayms(int ms)
{
	double tcnt = 65536 - (((double)ms)/1000)*((double)F_CPU/64); //set TCNT = 2^16 - delay/clock period/64

	TCNT1H = ((int)tcnt)>>8; //move upper byte of TCNT
	TCNT1L = ((int)tcnt) & 0x00FF; //move lower byte of TCNT
	
	TCCR1A = 0; //timer 1 in normal mode
	TCCR1B = 3; //prescaler = 64
	
	while(!(TIFR1 & (1<<TOV1))) //loop until timer overflow happens
	{
	}
	
	TCCR1B = 0; //stop timer 1
	TIFR1 = 1; //clear overflow flag
	
	return;
}

ISR (PCINT0_vect) //ISR for PCINT5 (PORTB.5)
{
	if((PORTB & (1<<PB5)) == 0) //if pin changed from high to low, do nothing
	{
	}
	
	else //pin changed from low to high
	{
		static int counter = 0; //used to detect every 5th rising edge
		counter++; 
		
		if(counter%5 == 0) //every 5 rising edges, pb.4 toggles
			PORTB ^= (1<<PB4); //toggle pb.4
	}
	
	return;
}