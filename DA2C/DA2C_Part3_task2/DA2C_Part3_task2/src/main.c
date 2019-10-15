#include <avr/io.h>
#include <stdio.h>
#include <avr/interrupt.h>

//set global variables
int flag = 0;
int matchCount = 0;
int cycleMatch = 244;


int main(void){
	//initialize registers
	DDRB |= (1<<2)|(1<<5);		//set portb 2 to output
	PORTB |= (1<<2)|(1<<5);		//set pb2 to high
	DDRC &= (0<<2);		//set ddrc to input
	PORTC |= (0<<2);		//set portc2 to low
	
	//Set modes
	TCCR0A |= (1<<WGM01);	//set ctc mode on OCRA
	TCCR0B |= (1<<CS00)|(1<<CS02);	// Set pre-scaler to 1024
	TIMSK0 |= (1<<OCIE0A);	//enable compare interrupts
	OCR0A = cycleMatch;

	sei();			//enable interrupt
	
	while (1) {
		if (!(PINC & (1 << PINC2))) //check for button press
		{
			flag = 1;		//if pressed then turn on led
		}
		else {
			PORTB |= (1<<2);		//if not pressed then keep led off
			flag = 0;
		}
	}
	return 0;
}

ISR(TIMER0_COMPA_vect){
	
	int ovFlowCount = 0;
	int matchCheck = 0;
	int delayCount = 85;		//amount of overflows to reach desired delay

	if (flag == 1)
	{
		flag = 0;	//toggle flag
		PORTB &= ~(1<<2);
		TCNT0 = 0;				//start counter at 0
		while(matchCount <= delayCount )
		{
			matchCheck = TIFR0 & 0x02;	//variable to check if compare is met
			if (matchCheck == 0x02){
				matchCount ++;		//increase match count
				TIFR0 |= (1<<OCF0A); //reset sreg flag
			}
		}
		matchCount = 0;		//reset match count
		PORTB |= (1<<2); //turn off LED
		
	}
}
