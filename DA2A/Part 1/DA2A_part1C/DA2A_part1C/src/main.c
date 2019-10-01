
#define F_CPU 16000000UL

#include <avr/io.h>
#include <util/delay.h>


int main(void)
{
	DDRB |= (1<<3);		// set PORTB.3 for output
	
	while (1)
	{
		_delay_ms(250);		// delay for 40% DC
		
		PORTB &= ~(1<<3);	// make port B LED high
		
		_delay_ms(375);		// the rest of the .650ms delay
		
		PORTB |= (1<<3);	// make port B LED low
	}
}
