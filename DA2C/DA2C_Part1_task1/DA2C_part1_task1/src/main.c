#define F_CPU 16000000UL
#include <avr/io.h>


int main(void)
{
	DDRB =0xFF;		// set PORTB registers as outputs for output (LED)
	DDRC =0x00;		// set PORTC registers as inputs  for input switches
	PORTB =0xFF;		// turn all the LEDs in B on
	TCCR0A = 0;			// normal operation
	TCCR0B |= (1<<CS02) | (1<<CS00); // prescaler = 1024
	int count = 0;		// overflow counter
	
	
	while (1) {
		count = 0;					// reset counter
		TCNT0 = 0x00;				// reset timer
		while (count < 15){			// loop until 40% DC is met
			if(TCNT0 == 0xFF){		// if timer overflows
				count++;			// increment counter
				TCNT0 = 0x00;		// reset timer
			}
		}
		PORTB =0x00 ;			// toggle LED off
		count = 0;					// reset counter
		TCNT0 = 0x00;				// reset timer
		while (count < 23){			// loop until remaining 60% DC is met
			if(TCNT0 == 0xFF){		// if timer overflows
				count++;			// increment counter
				TCNT0 = 0x00;		// reset timer
			}
		}
		PORTB =0xFF;			// toggle LED on
	}
}

