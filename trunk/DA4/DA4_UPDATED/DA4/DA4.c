/*
Emmanuel Sanchez
CPE 301 - DA4 RGB LED changing colors using PWM
 */ 

#define F_CPU 16000000UL //define clock frequency
#include <avr/io.h>
#include <util/delay.h> //needed for delay

void pwm_init();	//initialize PWM channels

int main(void)
{	
	pwm_init();		//initialize PWM channels
		
	//set R, G, B channels to 10% DC
	OCR0A = 0x19;	//note: 0x19 = FF * 10%
	OCR1A = 0x19;	//note: 0x19 = FF * 10%
	OCR2A = 0x19;	//note: 0x19 = FF * 10%
	
    while(1)
    {
		//note: 0xE6 = 90% DC
		while(OCR0A < 0xE6){	//r fade in
			OCR0A += 0xF;		//increment DC
			_delay_ms(50);
		}

		//note: 0xE6 = 90% DC
		while(OCR1A < 0xE6){	//g fade in
			OCR1A += 0xF;		//increment DC
			_delay_ms(50);
		}

		//note: 0xE6 = 90% DC
		while(OCR2A < 0xE6){	//b fade in
			OCR2A += 0xF;		//increment DC
			_delay_ms(50);
		}

		//note: 0x19 = 10% DC
		while(OCR0A > 0x19){	// r fade out
			OCR0A -= 0xF;		//decrement DC
			_delay_ms(50);
		}
		
		//note: 0x19 = 10% DC
		while(OCR2A > 0x19){	// b fade out
			OCR2A -= 0xF;		//decrement DC
			_delay_ms(50);	
		}
		
		//note: 0xE6 = 90% DC
		while(OCR0A < 0xE6){	// r and b fade in
			OCR0A += 0xF;		//increment red DC
			OCR2A += 0xF;		//increment blue DC
			_delay_ms(50);
		}
		
		//note: 0x19 = 10% DC
		while(OCR1A > 0x19){	//g fade out
			OCR1A -= 0xF;		//decrement DC
			_delay_ms(50);
		}
		
		//note: 0x19 = 10% DC
		while(OCR0A > 0x19){	// r fade out
			OCR0A -= 0xF;		//decrement DC
			_delay_ms(50);
		}
		
		//note: 0x19 = 10% DC
		while(OCR2A > 0x19){	// b fade out
			OCR2A -= 0xF;		//decrement DC
			_delay_ms(50);
		}
	}
}

//initialize PWMs
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
	TCCR0A |= (1<<COM0A1);	//timer 0
	TCCR1A |= (1<<COM1A1);	//timer	1
	TCCR2A |= (1<<COM2A1);	//timer 2
	
	//set clock, no prescaler
	TCCR0B |= (1<<CS00);	//timer 0
	TCCR1B |= (1<<CS10);	//timer 1
	TCCR2B |= (1<<CS20);	//timer 2
}