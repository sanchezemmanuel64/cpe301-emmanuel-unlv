/*
MAIN
*/

#define F_CPU 16000000UL
#define BAUD 9600

#define ACCEL_SENSITIVITY 8192.0 // Sensitivity = -2 to 2 G at 16Bit -> 2G = 32768 && 0.5G = 8192
#define GYRO_SENSITIVITY 65.536
#define PI 3.14159265359		//pi

#include <inttypes.h>
#include <stdint.h>
#include <avr/io.h>
#include <util/delay.h>
#include <util/setbaud.h>
#include <avr/interrupt.h>
#include <math.h>
#include "mpu6050.h"
#include "mpu6050_reg.h"
#include "i2c.h"
#include "uart.h"

void timer_setup();
void get_time(double* dt);
volatile double count;
const double unit_t = 8/16000000;

int main(void){

sei();  
uart_init();
i2c_init();

DDRB |= _BV(5);

uint8_t ret;

int16_t accel_buff[3], gyro_buff[3];
double accelX, accelY, accelZ;
double gyroX, gyroY, gyroZ;
double dt;
float pitch;	//angle around x axis
float roll;		//angle around y axis
float pitch_accel, roll_accel;
float clicks = 0;
int clickprevx = 0;
int clickprevy = 0;

// initialize & test MPU5060 availability
ret = i2c_start(MPU6050_ADDRESS+I2C_WRITE);
if(~ret){
	PORTB |= _BV(5);
	_delay_ms(200);
	PORTB &= ~(_BV(5));
}

mpu6050_init();
timer_setup();

mpu6050_read_accel_ALL(accel_buff);

while(1){
	get_time(&dt);
	mpu6050_read_accel_ALL(accel_buff);
	mpu6050_read_gyro_ALL(gyro_buff);
	
	// acceleration (m/s^2)
	accelX = accel_buff[0]*9.8*2/32768;
	accelY = accel_buff[1]*9.8*2/32768;
	accelZ = accel_buff[2]*9.8*2/32768;
	
	// gyro rate (degrees/s)
	gyroX = gyro_buff[0]*(180/PI)*1000/32768;
	gyroY = gyro_buff[1]*(180/PI)*1000/32768;
	gyroZ = gyro_buff[2]*(180/PI)*1000/32768;
	
	//uses complementary filter as found on http://www.pieter-jan.com/
		
	//integrate the gyro data: int(angularSpeed) = angle
	pitch = pitch + ((float)gyro_buff[0] / GYRO_SENSITIVITY) * dt;
	roll = roll - ((float)gyro_buff[1] / GYRO_SENSITIVITY) * dt; 
	
	//compensate for drift with accelerometer data
	int approx_force_magnitude = abs(accel_buff[0]) + abs(accel_buff[1]) + abs(accel_buff[2]);
	
	if(approx_force_magnitude > 8192 && approx_force_magnitude < 32768)
	{
		//pitch
		pitch_accel = atan2f((float)accel_buff[1], (float)accel_buff[2]) * 180 / PI; //in degrees
		pitch = pitch * 0.98 + pitch_accel * 0.02;
		if((pitch > 25 || pitch <-25) && clickprevx == 0)
		{
			clicks += 1;
			clickprevx = 1;
		}
		
		else if (!(pitch > 25 || pitch <-25)) clickprevx = 0;
		
		//roll
		roll_accel = atan2f((float)accel_buff[0], (float)accel_buff[2]) * 180 / PI; //in degrees
		roll = roll * 0.98 + roll_accel * 0.02; 
		if((roll > 25 || roll <-25) && clickprevy == 0)
		{ 
			clicks += 1;
			clickprevy = 1;
		}
		
		else if(!(roll > 25 || roll <-25)) clickprevy = 0;
	}
	
	//The android application used for graphing the serial data
	//from bluetooth requires a special format. Each line must start with 
	//"E" and all values must be separated by commas.
	//Format: "Evalue1,value2,value3\n"
	
	_delay_ms(10);
	uart_putchar('E');
	uart_putdouble(gyroZ);
	uart_putstring(",");
	uart_putdouble(pitch);
	uart_putstring(",");
	uart_putdouble(roll);
	uart_putchar('\n');
	uart_putstring("                                                                   MOVEMENTS: ");
	uart_putdouble(clicks);
	uart_putstring("                      ");
	uart_putchar('\n');
	//uart_putchar('\r');
		
		
}

}//end of main



void timer_setup(){
	TCCR1A = 0x00;
	TIMSK1 |= _BV(TOIE1);
	TCCR1B |= _BV(CS11);  
	TCCR1B &= ~( _BV(CS12)  | _BV(CS10)); // prescaler=8

}



void get_time(double * dt){
	cli();
		uint8_t l = TCNT1L;
		uint8_t h = TCNT1H;
		uint16_t step = h<<8 | l;
		*dt = (double)step*5e-7 + count*0.032768;
		count = 0;
	sei();
}


// timer 1 overflow interrupt handler
SIGNAL(TIMER1_OVF_vect){
	count += 1;
	//TCNT1H = 0x00;
	//TCNT1L = 0x00;

}




