#define F_CPU 16000000UL
#include <avr/io.h>
#include <stdlib.h>
#include <math.h>
#include <avr/interrupt.h>
#include <stdio.h>
#include <util/delay.h>
#include <math.h>
#define BAUDRATE 9600
#define BAUD_PRESCALLER (((F_CPU / (BAUDRATE * 16UL))) - 1)

void USART_init(void); //initialize USART
void USART_transmit(unsigned char data); //send through USART
unsigned char USART_receive(void); //receive through USART
void USART_putstring(char* StringPtr); //scan through string
void ADC_init (void); //function to initialize ADC

volatile unsigned int ADC_temp;
char outs[20];

uint8_t OVFLOWCOUNT = 0; //initialize the overflow count for interrupt
uint8_t OVFLOWLIMIT = 250; //set the over flow limit for 1 sec delay


int main(void)
{
	USART_init(); //initialize USART
	ADC_init (); //initialize ADC
	USART_putstring("Connected\r\n");
TCCR0A = 0x00; //set normal operation
	TCCR0B |= (1 << CS02); //set prescalar to 256
	TCNT0 = 6; 
	TIMSK0 |= (1 << TOIE0);
	sei(); //set interrupt

	
	while (1)
	{ //sit here waiting for interrupt
	}
}
void USART_init(void)
{
	UBRR0H = (uint8_t)(BAUD_PRESCALLER >> 8);
	UBRR0L = (uint8_t)(BAUD_PRESCALLER);
	UCSR0B = (1 << RXEN0) | (1 << TXEN0);
	UCSR0C = (3 << UCSZ00);
}

unsigned char USART_receive(void)
{
	while(!(UCSR0A & (1 << RXC0)));
	return UDR0;
}

void USART_transmit(unsigned char data)
{
	while(!(UCSR0A & (1 << UDRE0)));
	UDR0 = data;
}

void USART_putstring(char* StringPtr)
{
	while(*StringPtr != 0x00)
	{
		USART_transmit(*StringPtr);
		StringPtr++;
	}
}

void ADC_init (void)
{
	ADMUX = (0<<REFS1)|					
	(1<<REFS0)|							
	(0<<ADLAR)|							
	(1<<MUX2)|							
	(0<<MUX1)|	// ADC5 PC5
	(0<<MUX0);
	ADCSRA = (1<<ADEN)|					
	(0<<ADSC)|							
	(0<<ADATE)|							
	(0<<ADIF)|							
	(0<<ADIE)|							
	(0<<ADPS2)|	// ADC Prescaler 
	(1<<ADPS1)|
	(1<<ADPS0);
	
}
ISR (TIMER0_OVF_vect)
{
	OVFLOWCOUNT++; 
	if (OVFLOWCOUNT == OVFLOWLIMIT) 
	{
		ADCSRA|=(1<<ADSC);
		while((ADCSRA&(1<<ADIF))==0);
		
		ADCSRA |= (1<<ADIF);
//take value from ADC and transmit it
		int var = ADCL;
		var = var | (ADCH<<8);
		var = (var/1024.0) * 5000/10;
		var = (var*2)+32; 
		var = var % 100;
		USART_transmit((var/10)+'0');
		var = var % 10;
		USART_transmit((var)+'0');
		USART_transmit('\n');
		OVFLOWCOUNT = 0; 
	}
	TCNT0 = 6; 
}
