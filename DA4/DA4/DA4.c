/*
Emmanuel Sanchez
CPE 301 - DA4
 */ 

#define F_CPU 16000000UL
#include <avr/io.h>
#include <util/delay.h>

void pwm_init();	//initialize PWM channels
void setDC10();		//set R, G, B channels to 10% DC

int main(void)
{
	//pwm_init();		//initialize PWM channels
	//setDC10();		//set R, G, B channels to 10% DC
	
	DDRB |= (1<<PB3);
	PORTB |= (1<<PB3);
    while(1)
    {
		

	
    }
}

void pwm_init(){
	//3 PWM channel outputs
	DDRD |= 1<<PD6; //R
	DDRD |= 1<<PD5; //G
	DDRB |= 1<<PB3; //B
	
	//set operation to fast PWM	
	TCCR0B |= (1<<WGM02);
	TCCR0A |= (1<<WGM01 | 1<<WGM00);
	TCCR2B |= (1<<WGM22);
	TCCR2A |= (1<<WGM21 | 1<<WGM20);

	//set clock 
	TCCR0B |= (1<<CS00); //no prescaler
	TCCR2B |= (1<<CS00); //no prescaler
	
	//set inverting mode
	TCCR0A |= (1<<COM0A1 | 1<<COM0A0);
	TCCR0B |= (1<<COM0B1 | 1<<COM0B0);
	TCCR2A |= (1<<COM2A1 | 1<<COM2A0);
	
}

void setDC10(){
	//set DC 10% (%10 of 0xFF is 0x19)
	OCR0A = 0x19; //R
	OCR0B = 0x19; //G
	OCR2A = 0x19; //B
}
