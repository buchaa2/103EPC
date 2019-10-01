
#define F_CPU 16000000UL

#include <avr/io.h>
#include <util/delay.h>

int main(void)
{
	DDRC &= (0<<3);		// make port c an input or in our case a switch
	PORTC |= (1<<3);	// set port c to be high
	DDRB |= (1<<2);		// set port b to be the LED
	
	while(1){
		if(!(PINC & (1<<PINC2))){	// if pinC is low loop 
			PORTB &= ~(1<<2);		// turn LED on
			_delay_ms(1333);		// delay 1.333 sec
		}
		else{						// else pinC is high
			PORTB |= (1<<2);		// turn LED off
		}
	}// loop
	return 0;
}
