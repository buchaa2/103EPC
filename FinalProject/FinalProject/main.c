
#ifndef F_CPU
#define F_CPU 16000000UL
#endif

#include <avr/io.h>
#include <stdio.h>
#include <avr/interrupt.h>
#include <util/delay.h>
#include <stdlib.h>
#include <stdint.h>

#include "APDS9960_def.h"
#include "i2c_master.h"

#define F_CPU 16000000UL
#define BAUD 115200
#define FOSC 16000000
#define UBRREQ FOSC/8/BAUD -1

#define APDS9960_WRITE 0x72
#define APDS9960_READ 0x73

void UART_init (void);
void APDS_init (void);

int UART_putchar( char c, FILE *stream);
FILE str_uart = FDEV_SETUP_STREAM(UART_putchar, NULL , _FDEV_SETUP_WRITE);
void startreading(void);

uint16_t red;
uint16_t green;
uint16_t blue;

int main( void )
{
	UART_init(); // This initializes UART values
	APDS_init(); // This initializes APDS9960
	i2c_init(); // This initializes I2C
	stdout = &str_uart;
	
	red = 0;
	green = 0;
	blue = 0;
	
	//
	_delay_ms(2000);
	printf("AT\r\n");
	
	// Set AP’s info connected by ESP8266. (AP + Station Mode)
	_delay_ms(5000);
	printf("AT+CWMODE=3\r\n");
	
	// Internet Connection
	_delay_ms(5000);
	printf("AT+CWJAP=\"ASUS\",\"abulator\"\r\n");	// Wifi password and SSID

	while(1) // This constantly sends values to the cloud
	{	
		// Enables Single Connection
		_delay_ms(5000);
		printf("AT+CIPMUX=0\r\n");
		
		// Start the connection to cloud
		_delay_ms(5000);
		printf("AT+CIPSTART=\"TCP\",\"184.106.153.149\",80\r\n");
		
		
		// Send values red, green, blue to field 1, field 2, and field 3
		_delay_ms(5000);
		startreading();
		printf("AT+CIPSEND=104\r\n");
		printf("GET /update?key=ZDG1BP942G9NVEWD&field1=", red, green, blue);
		_delay_ms(3000);
		
		//
	}
}

void startreading(){

	uint8_t redH, redL;
	uint8_t greenH, greenL;
	uint8_t blueH, blueL;
	
	// RED
	i2c_readReg(APDS9960_WRITE, APDS9960_RDATAH, &redH, 1);
	i2c_readReg(APDS9960_WRITE, APDS9960_RDATAL, &redL, 1);
	
	// GREEN
	i2c_readReg(APDS9960_WRITE, APDS9960_GDATAH, &greenH, 1);
	i2c_readReg(APDS9960_WRITE, APDS9960_GDATAL, &greenL, 1);
	
	// BLUE
	i2c_readReg(APDS9960_WRITE, APDS9960_BDATAH, &blueH, 1);
	i2c_readReg(APDS9960_WRITE, APDS9960_BDATAL, &blueL, 1);
	
	red = (redH << 8) | redL;
	green = (greenH << 8) | greenL;
	blue = (blueH << 8) | blueL;
	
	// The Threshold limit
	if (red > 255)
	red = 255;
	if (green > 255)
	green = 255;
	if (blue > 255)
	blue = 255;
}

void APDS_init(void){
	uint8_t setup;
	
	i2c_readReg(APDS9960_WRITE, APDS9960_ID, &setup,1);
	if(setup != APDS9960_ID_1) while(1);
	setup = 1 << 1 | 1<<0 | 1<<3 | 1<<4;
	
	i2c_writeReg(APDS9960_WRITE, APDS9960_ENABLE, &setup, 1);
	setup = DEFAULT_ATIME;
	
	i2c_writeReg(APDS9960_WRITE, APDS9960_ATIME, &setup, 1);
	setup = DEFAULT_WTIME;
	
	i2c_writeReg(APDS9960_WRITE, APDS9960_WTIME, &setup, 1);
	setup = DEFAULT_PROX_PPULSE;
	
	i2c_writeReg(APDS9960_WRITE, APDS9960_PPULSE, &setup, 1);
	setup = DEFAULT_POFFSET_UR;
	
	i2c_writeReg(APDS9960_WRITE, APDS9960_POFFSET_UR, &setup, 1);
	setup = DEFAULT_POFFSET_DL;
	
	i2c_writeReg(APDS9960_WRITE, APDS9960_POFFSET_DL, &setup, 1);
	setup = DEFAULT_CONFIG1;
	
	i2c_writeReg(APDS9960_WRITE, APDS9960_CONFIG1, &setup, 1);
	setup = DEFAULT_PERS;
	
	i2c_writeReg(APDS9960_WRITE, APDS9960_PERS, &setup, 1);
	setup = DEFAULT_CONFIG2;
	
	i2c_writeReg(APDS9960_WRITE, APDS9960_CONFIG2, &setup, 1);
	setup = DEFAULT_CONFIG3;
	
	i2c_writeReg(APDS9960_WRITE, APDS9960_CONFIG3, &setup, 1);
}

void USART_putstring(char *StringPtr)
{
	while ((*StringPtr != '\0')){			// Keep looping until the end of the line 
		while (!(UCSR0A & (1 << UDRE0)));   // Keep looping until UDRE0 goes high
		UDR0 = *StringPtr;					
		StringPtr++; 
	}
}

void UART_init(void)
{
	//Sets the baud rate
	uint16_t baud_rate = UBRREQ;
	UBRR0H = baud_rate >> 8;
	UBRR0L = baud_rate & 0xFF;
	
	//This enable receiver and the transmitter
	UCSR0B = ( 1 <<RXEN0)|( 1 <<TXEN0);
	
	// Set frame format: 8data, 1stop bit
	UCSR0C = (3 <<UCSZ00);
}

int UART_putchar(char c, FILE *stream)
{
	//waits until buffer is empty
	while ( !( UCSR0A & ( 1 <<UDRE0)) );
	
	//Put data into the buffer
	UDR0 = c;
	return 0;
}

