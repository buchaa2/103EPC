#define F_CPU 16000000UL
#include <avr/io.h>


int main(void)
{
	DDRC &= (0<<3);		// connect PORTC.3 to switch as input
	PORTC |= (1<<3);	// enable pull-up
	DDRB = 0xFF;		// set PORTB.2 for output (LED)
	TCCR0A = 0;			// normal operation
	TCCR0B |= (1<<CS02) | (1<<CS00); // prescaler = 1024
	int count = 0;		// overflow counter
	
	while (1) {
		if(!(PINC & (1<<PINC3))) {	// check if pin C 3 is low
			PORTB =0x00;		// set LED on
			
			count = 0;				// reset counter
			TCNT0 = 0x00;			// reset timer
			while(count < 82) {		// loop until 1.25 sec delay met
				if(TCNT0 == 0xFF){	// if timer overflows
					count++;		// increment counter
					TCNT0 = 0x00;	// reset timer
				}
			}
		}
		else {
			PORTB = 0xFF;		// set LED off
		}
	}
	return 0;
}
