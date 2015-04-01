//Emmanuel Sanchez 
//CPE 301 - DA3
//USART, ADC, temp sensor

#define F_CPU 16000000UL //16 MHz clock
#include <avr/io.h>
#include <avr/interrupt.h>

void usart_init (void) //initialize USART
{
	UCSR0B = (1<<TXEN0); //transmit enable
	UCSR0C = ((1<<UCSZ01)|(1<<UCSZ00)); //asynch mode
	UBRR0L = 0x67; //baud rate 9600 at 16 MHz
}
void usart_tx_string(char *data) //takes a string and sends it serially
{
	while(*data != '\0'){ //send chars until NULL is found
		while(!(UCSR0A & (1<<UDRE0))); //wait for UDRE0 to be 1
		UDR0 = *data;	//send char serially
		data++; //point to next char
	}
}
int main(void)
{
	DDRC &= ~(1<<PORTC0);	//PC0 is analog input
		
	usart_init(); //initialize USART
	ADCSRA = 0x87; //set ADEN, ADC prescaler 128
	ADMUX = 0xC0; //select channel 0 (PC0)
	sei(); //enable interrupts (needed for timer overflow interrupt)
	
	//configure timer 1 to interrupt every second
	TCNT1 = 65536 - ((double)F_CPU/256); //overflow in 1 sec
	TCCR1A = 0; //normal mode
	TCCR1B = 4; //prescaler = 256
	TIMSK1	|= (1<<TOIE1); //interrupt on overflow
	
	while(1); //wait for interrupts
	
	return 0;
}

ISR(TIMER1_OVF_vect) //timer1 overflow ISR
{	
	TCCR1B = 0; //stop timer 1
	TIFR1 = 1; //clear overflow flag

	int adc_temp;	//stores ADC temporarily
	float adc_tempf; //float for calculations
	int adc_tempi; //integer part
	int adc_tempd; //decimal part
	
	//read ADC
	ADCSRA |= (1<<ADSC); //start conversion
	while((ADCSRA &(1<<ADIF)) == 0); //wait for conversion to finish
	adc_temp = ADC;	//save ADC value
		
	adc_tempf = (float)adc_temp * (1.1 / 1024) / 0.01; //ADC value * resolution / scale factor of LM34 (10mV/deg F)
	adc_tempi = (int)adc_tempf; //integer part
	adc_tempf = adc_tempf - adc_tempi; //subtract integer part
	adc_tempd = (int)(adc_tempf * 10); //decimal part

	char intpart[10]; //stores temperature to be displayed
	sprintf(intpart, "%d", adc_tempi); //integer part to string
	strcat(intpart, "."); //concatenate dot
	
	char decpart[4]; //stores decimal part
	sprintf(decpart, "%d\r\n", adc_tempd); //decimal part to different string
	strcat(intpart, decpart); //concatenate integer and decimal parts
	
	usart_tx_string(intpart); //send temperature serially
	
	//re-configure timer 1 to interrupt every second
	TCNT1 = 65536 - ((double)F_CPU/256); //overflow in 1 sec
	TCCR1A = 0; //normal mode
	TCCR1B = 4; //prescaler = 256
		
	return;
}