
#include <avr/io.h>
#include <stdio.h>
#include <avr/interrupt.h>

//declare vital variables
int ovFlowCount = 0;
int dc60 = 23; //overflow count to reach is 23
int dc40 = 15; //overflow count to reach is 15
int flag = 1;


int main(void)
{
	DDRB |= (1<<2)|(1<<5);		// Set all PORTB out
	PORTB |= (1<<5);			// Set PORTB
	TCCR0B |= (1<<CS00)|(1<<CS02);	// Set pre-scaler to 1024
	TIMSK0 |= (1<<TOIE0); //enable timer interrupts
	sei(); //enable interrupts
	TCNT0 = 0; //initialize clock
	
	while(1);
	
}

ISR(TIMER0_OVF_vect){
		ovFlowCount ++;
		if (flag == 1 && ovFlowCount == dc40){
			PORTB &= ~(1<<2);
			ovFlowCount = 0;	//reset overflow counter
			flag = 0;
		}
		else if (flag == 0 && ovFlowCount == dc60){
			PORTB |= (1<<2);
			ovFlowCount = 0;	//reset overflow counter
			flag = 1;
		}
}
