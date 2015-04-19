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
	pwm_init();		//initialize PWM channels
		
	//set R, G, B channels to 10% DC
	OCR0A = 0x19;	//note: 0x19 = FF * 10%
	OCR1A = 0x19;
	OCR2A = 0x19;
	
    while(1)
    {
		//note: 0xE6 = FF * 90% 
		while(OCR0A < 0xE6){	//r fade in
			OCR0A += 0xF;
			_delay_ms(50);
		}

		while(OCR1A < 0xE6){	//g fade in
			OCR1A += 0xF;
			_delay_ms(50);
		}

		while(OCR2A < 0xE6){	//b fade in
			OCR2A += 0xF;
			_delay_ms(50);
		}

		while(OCR0A > 0x19){	// r fade out
			OCR0A -= 0xF;
			_delay_ms(50);
		}
		
		while(OCR2A > 0x19){	// b fade out
			OCR2A -= 0xF;
			_delay_ms(50);
		}
		
		while(OCR0A < 0xE6){	// r and b fade in
			OCR0A += 0xF;
			OCR2A += 0xF;
			_delay_ms(50);
		}
		
		while(OCR1A > 0x19){	//g fade out
			OCR1A -= 0xF;
			_delay_ms(50);
		}
		
		while(OCR0A > 0x19){	// r fade out
			OCR0A -= 0xF;
			_delay_ms(50);
		}
		
		while(OCR2A > 0x19){	// b fade out
			OCR2A -= 0xF;
			_delay_ms(50);
		}
	}
}

void pwm_init(){
	//3 PWM channel outputs
	DDRD |= (1<<PD6); //R OC0A
	DDRB |= (1<<PB1); //G 0C1A
	DDRB |= (1<<PB3); //B 0C2A
	
	//fast pwm mode
	TCCR0A |= (1<<WGM01) | (1<<WGM00);	//timer 0
	TCCR2A |= (1<<WGM21) | (1<<WGM20);	//timer 2
	TCCR1A |= (1<<WGM10);				//timer 1
	TCCR1B |= (1<<WGM12);				//timer 1
	
	//clear mode
	TCCR0A |= (1<<COM0A1);
	TCCR1A |= (1<<COM1A1);
	TCCR2A |= (1<<COM2A1);
	
	//set clock, no prescaler
	TCCR0B |= (1<<CS00);	
	TCCR1B |= (1<<CS10);	
	TCCR2B |= (1<<CS20);
}