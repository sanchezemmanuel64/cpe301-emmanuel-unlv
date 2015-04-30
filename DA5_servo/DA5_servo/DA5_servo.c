/*
Emmanuel Sanchez
CPE 301 - DA5: servo motor control using potentiometer
 */ 

#define F_CPU 16000000UL //clock = 16 MHz
#include <avr/io.h>

int main()
{
	DDRB |= (1<<PB1); //PWM pin (OC1A)
	DDRC &= ~(1<<PC0); //potentiometer analog input
	
	//configure timer1
	TCCR1A |= (1<<COM1A1) | (1<<WGM11); //fast PWM (mode 14), non-inverting
	TCCR1B |= (1<<WGM13) | (1<<WGM12) | (1<<CS11) | (1<<CS10); //prescaler = 64
	
	ICR1 = 4999; //fPWM = 50 Hz (period = 20ms)
	
	//configure ADC
	ADCSRA = 0x87;	//ADC enable, no interrupts, div factor 128
	ADMUX = 0x60;	//Reference voltage = AVCC with external capacitor at AREF pin, left justified

	uint8_t pot;	//holds value of pot from analog channel
	float servo_pos; //holds value to determine servo position
	
	while(1)
	{
		//get ADC value
		ADCSRA |= (1<<ADSC); //start conversion
		while((ADCSRA &(1<<ADIF)) == 0); //wait for conversion to finish
		pot = ADCH<<1;			//take value from upper byte of ADC
		
		servo_pos = pot * 5 / 1024.0;	//scale the potentiometer value
			
		//determine servo position	
		//0 degrees on my servo is when OCR1A is 150.
		//180 degrees is when OCR1A is 535.
		servo_pos =  (servo_pos * 385) + 150; //equation to determine position. Min = 150, Max = 535
		
		OCR1A = (int)servo_pos; //update servo position
	}
}