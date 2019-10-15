#include <avr/io.h>
#include <stdio.h>
#include <avr/interrupt.h>

//declare vital variables
int matchCount = 0;
int dc60 = 23; //duty cycle of 60%
int dc40 = 15; //duty cycle of 40%
int flag = 1;  //flag used to toggle between duty cycles
int cycleMatch = 244;	//compare to 244 and clear each time

int main(void)
{
	DDRB |= (1<<2)|(1<<5);	// Set all PORTB out
	PORTB |= (1<<5);		//turn off LED 5
	TCCR0A |= (1<<WGM01);	//set ctc mode on OCRA
	TCCR0B |= (1<<CS00)|(1<<CS02);	// Set pre-scaler to 1024
	TIMSK0 |= (1<<OCIE0A);	//enable compare interrupts
	OCR0A = cycleMatch;
	sei();					//enable interrupts
	
	while(1);				//infinite loop
	
}

ISR(TIMER0_COMPA_vect){
	matchCount ++;
	if (flag == 1 && matchCount == dc60)//if the flag and the matchCount equals the cycle for 60%
	{
		PORTB &= ~(1<<2);	//toggle LED
		matchCount = 0;	//reset overflow counter
		flag = 0;		//set the flag for the second portion of the wave.
	}
	else if (flag == 0 && matchCount == dc40)//if the flag and the matchCount equals the cycles for 40%
	{
		PORTB |= (1<<2);	//toggle LED
		matchCount = 0;	//reset overflow counter
		flag = 1;
	}
}
