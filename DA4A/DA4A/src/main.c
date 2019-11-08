#define F_CPU 16000000UL
#include <avr/io.h>
#include <avr/interrupt.h>
#include <util/delay.h>


volatile unsigned int ADC_Val;			// variable for the value of the ADC
int flag = 0;							// used in the interrupt to toggle 

int main(void){
	DDRB |= (1<<PB1)|(1<<PB5);		//PB1 and PB5 are outputs
	DDRC = 0;						//port c is an input
	PORTB = 0;						//port b is set to low
	PORTC |= (1<<PC1);				//pc1 is set to high
	
	TCCR1A |= (1<<COM1A1)|(1<<COM1B1)|(1<<WGM11);	// we are usingFast PWM, Non-inverted mode
	TCCR1B |= (1<<WGM13)|(1<<WGM12)|(1<<CS11);		// Pre-scaler is set to 8
	ICR1 = 9999;									// Top of timer1 set to 9999
	
	PCICR = (1<<PCIE1);		//Enable PCINT
	PCMSK1 = (1<<PCINT9);   //Set interrupt for PC1
	
	ADMUX = (1<<REFS0);												// the input for ADC is pc0
	ADCSRA |= (1<<ADEN) | (1<<ADPS2) | (1<<ADPS1) | (1<<ADPS0);		// enable ADC, system clock used for ADC
	ADCSRB = 0x0;													// free running mode
	DIDR0 = 0x1;													// Filter out any digital signals
	
	sei();			//Enable interrupt
	
	while (1);		//wait for interrupt
}


//ISR for PC1
ISR(PCINT1_vect){
	_delay_ms(500);	
	if(!(PINC & (1 << PINC1))){
		while(!(PINC & (1 << PINC1))){
			if(flag == 1){
				PORTB |= (1 << PORTB1) | (1 << PORTB5); //turn on OC1A and LED off
				ADCSRA |= (1 << ADSC);				// start conversion
				while((ADCSRA&(1<<ADIF))==0){		// wait for conversion to finish
					ADC_Val = ADC & 0x03FF;		// we only need first 10 bits
					OCR1A = 10*ADC_Val;				// Calculate duty cycle
				}
			}
			else if(flag == 0){
				OCR1A = 0;					//reset
				PORTB &= ~(1 << PORTB1);
				PORTB &= ~(1 << PORTB5);	//turn off OC1A and LED on
			}
		}
		flag ^= 1;			//toggle
	}
}
