//Emmanuel Sanchez 
//CPE 301 - DA3
//USART, ADC, temp sensor

#define F_CPU 16000000UL
#include <avr/io.h>
#include <avr/interrupt.h>

void usart_init (void)
{
	UCSR0B = (1<<TXEN0); //transmit enable
	UCSR0C = ((1<<UCSZ01)|(1<<UCSZ00)); //asynch mode
	UBRR0L = 0x67; //baud rate 9600 at 16 MHz
}
void usart_tx_string(char *data) //takes a char and sends it serially
{
	while(*data != '\0'){
		while(!(UCSR0A & (1<<UDRE0))); //wait for UDRE0 to be 1
		UDR0 = *data;	//send char serially
		data++;
	}
}
int main(void)
{
	DDRC &= ~(1<<PORTC0);	//PORTC.0 is analog input
		
	usart_init();
	ADCSRA = 0x87; //set ADEN, ADC prescaler 128
	//ADCSRB = 0x6; //autotrigger timer 1 overflow
	ADMUX = 0; //select channel 0 (PC0)
	
	while(1){
	//configure timer 1 to interrupt every second
	TCNT1 = 65536 - ((double)F_CPU/256); //overflow in 1 sec
	TCCR1A = 0; //normal mode
	TCCR1B = 4; //prescaler = 256
	TIMSK1	|= (1<<TOIE1); //interrupt on overflow
	sei();
	}
	return 0;
}

ISR(TIMER1_OVF_vect)
{	
	TCCR1B = 0; //stop timer 1
	TIFR1 = 1; //clear overflow flag

	int adc_temp;
	ADCSRA |= (1<<ADSC);
	while((ADCSRA &(1<<ADIF)) == 0);
	adc_temp = ADC;
		
	char outs[20];
	snprintf(outs,sizeof(outs),"%3d\r\n", adc_temp);
	usart_tx_string(outs);
		
	return;
}