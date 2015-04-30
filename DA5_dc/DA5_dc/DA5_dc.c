/*
Emmanuel Sanchez
CPE 301 - DA5: DC motor control using potentiometer
 */ 

#define F_CPU 16000000UL //clock = 16 MHz
#include <avr/io.h>
#include <util/delay.h> //needed for delays

void delayms(int); //delay function. Uses milliseconds

int main(void)
{
	
	DDRD |= (1<<PD6); //OC0A output PWM
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
					
		PORTD |= (1<<PD6);			//turn on motor
		delayms(motor_speed * 30);	//delay is determined by pot
		
		PORTD &= ~(1<<PD6);			//turn off motor
		delayms((1 - motor_speed) * 30); //time off = (1 - time on)
		
    }
}

void delayms(int n) //delay function
{
	int i;
	for(i=0; i<n; i++)	//loop for n milliseconds
		_delay_ms(1);
};
