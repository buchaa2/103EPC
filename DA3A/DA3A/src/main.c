
#define F_CPU 16000000UL
#include <avr/io.h>
#include <stdlib.h>
#include <math.h>
#include <avr/interrupt.h>
#include <stdio.h>

#define BAUDRATE 9600
#define BAUD_PRESCALLER (((F_CPU / (BAUDRATE * 16UL))) - 1)

void USART_init(void); //initialize USART
void USART_send(unsigned char data); //send through USART
unsigned char USART_receive(void); //receive through USART
void USART_putstring(char* StringPtr); //scan through string


char String[] = "Hello_World"; //string i chose
char newline[] = "\n"; //make output pretty
float nu = 9.87; //number i chose


char buffer[5]; //buffer for random int number
char nu_buffer[4]; //buffer for float number
uint8_t OVF_COUNT = 0; //initialize the overflow count for interrupt
uint8_t OVF_LIMIT = 250; //set the over flow limit for 1 sec delay

int main(void)
{
	USART_init(); //initialize USART
	
	TCCR0A = 0x00; //set normal operation
	TCCR0B |= (1 << CS02); //set prescalar to 256
	TCNT0 = 6; 
	TIMSK0 |= (1 << TOIE0);
	sei(); //set interrupt
	
    while (1) 
    { //wait for interrupt
    }
}


void USART_init(void)
{
	UBRR0H = (uint8_t)(BAUD_PRESCALLER >> 8);
	UBRR0L = (uint8_t)(BAUD_PRESCALLER);
	UCSR0B = (1 << RXEN0) | (1 << TXEN0);
	UCSR0C = (3 << UCSZ00);
}
void USART_send(unsigned char data)
{
	while(!(UCSR0A & (1 << UDRE0)));
	UDR0 = data;
}
unsigned char USART_receive(void)
{
	while(!(UCSR0A & (1 << RXC0)));
	return UDR0;
}
void reverse(char *str, int len)
{
	int i=0, j=len-1, temp;
	while (i<j)
	{
		temp = str[i];
		str[i] = str[j];
		str[j] = temp;
		i++; j--;
	}
}
void USART_putstring(char* StringPtr)
{
	while(*StringPtr != 0x00)
	{
		USART_send(*StringPtr);
		StringPtr++;
	}
}
int intToStr(int x, char str[], int d)
{
	int i = 0;
	while (x)
	{
		str[i++] = (x%10) + '0';
		x = x/10;
	}
	
	while (i < d)
	str[i++] = '0';
	
	reverse(str, i);
	str[i] = '\0';
	return i;
}

// Converts a floating point number to string.
void ftoa(float n, char *res, int afterpoint)
{
	// Extract integer part
	int ipart = (int)n;
	
	// Extract floating part
	float fpart = n - (float)ipart;
	
	// convert integer part to string
	int i = intToStr(ipart, res, 0);
	
	// check for display option after point
	if (afterpoint != 0)
	{
		res[i] = '.';  // add dot
		
		// Get the value of decimal
		fpart = fpart * pow(10, afterpoint);
		
		intToStr((int)fpart, res + i + 1, afterpoint);
	}
}
ISR (TIMER0_OVF_vect)
{
	OVF_COUNT++;
	if (OVF_COUNT == OVF_LIMIT) //check to see if the limit equals count
	{
		USART_putstring(String); //print string to the terminal window
		USART_putstring(newline); //go to next line
		itoa(rand(), buffer, 10); //convert random int number (base 10) to a string
		USART_putstring(buffer); //print string
		USART_putstring(newline); //go to next line
		ftoa(nu, nu_buffer, 2); //convert float number to a string
		USART_putstring(nu_buffer); //print string
		USART_putstring(newline); //go to next line
		USART_putstring(newline); //go to next line
		OVF_COUNT = 0; //reset overflow counter
	}
	TCNT0 = 6; //reset
}