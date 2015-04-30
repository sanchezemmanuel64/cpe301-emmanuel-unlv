/*
Emmanuel Sanchez
CPE 301 - DA5: stepper motor speed control using potentiometer
 */ 

#define F_CPU 16000000UL //XTAL = 16MHZ
#include <avr/io.h>
#include <util/delay.h> 

void delayms(int); //delay function


int main()
{
	DDRD = 0x0F; //set lower nibble of PORTD as output
	DDRC &= ~(1<<PC0); //potentiometer analog input
	
	//configure ADC
	ADCSRA = 0x87;	//ADC enable, no interrupts, div factor 128
	ADMUX = 0x60;	//Reference voltage = AVCC with external capacitor at AREF pin, left justified
	
	uint8_t pot;	//holds value of pot from analog channel
	float motor_speed; //used to determine delays
	
	while(1)
	{
		ADCSRA |= (1<<ADSC); //start conversion
		while((ADCSRA &(1<<ADIF)) == 0); //wait for conversion to finish
		pot = ADCH;			//take value from upper byte of ADC
		
		motor_speed =  pot * 5.0 / 1024.0;	//scale the potentiometer value
		
		//sequence for stepper motor
		PORTD = 0X66;
		delayms(motor_speed * 200);	//delay is proportional to pot value
			
		PORTD = 0x33;
		delayms(motor_speed * 200); //delay is proportional to pot value
		
		PORTD = 0x99;
		delayms(motor_speed * 200); //delay is proportional to pot value
			
		PORTD = 0xCC;
		delayms(motor_speed * 200); //delay is proportional to pot value
	}
	return 0;
}


void delayms(int n) //delay function
{
	int i;
	for(i=0; i<n; i++) //loop for n milliseconds
		_delay_ms(1);
}