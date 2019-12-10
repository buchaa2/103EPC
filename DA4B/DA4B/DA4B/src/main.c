#define F_CPU 16000000UL
#include <avr/io.h>
#include <avr/interrupt.h>
#include <util/delay.h>

volatile unsigned int ADCVal;
void ADC_init();
void timer_init();


int main(void){
	// initialize ADC
	ADC_init();

	//set timer
	TCCR1B |=(1<<WGM12)|(1<<CS11); //CTC MODE prescaler = 8

	// set DDR's
	DDRB = 0xFF; //all outputs
	DDRC = 0;	//all input
	PORTB = 0;	//start at 0
	
	while(1){
		ADCSRA|=(1<<ADSC);
		while ((ADCSRA&(1<<ADIF))==0);	//wait until done converting
		ADCVal = ADC & 0x03FF;			//take only 10 bits
		OCR1A = 10*ADCVal;				//scale for proper delay
		
		//procedure for PB0
		PORTB |= (1<<PB0);
		while(!(TIFR1 & (1<<OCF1A))); //wait until timer flag is set. repeat
		TIFR1 |= (1<<OCF1A);
		PORTB &= ~(1<<PB0);
		while(!(TIFR1 & (1<<OCF1A)));
		TIFR1 |= (1<<OCF1A);
		
		//procedure for PB1
		PORTB |= (1<<PB1);
		while(!(TIFR1 & (1<<OCF1A)));
		TIFR1 |= (1<<OCF1A);
		PORTB &= ~(1<<PB1);
		while(!(TIFR1 & (1<<OCF1A)));
		TIFR1 |= (1<<OCF1A);
		
		//procedure for PB2
		PORTB |= (1<<PB2);
		while(!(TIFR1 & (1<<OCF1A)));
		TIFR1 |= (1<<OCF1A);
		PORTB &= ~(1<<PB2);
		while(!(TIFR1 & (1<<OCF1A)));
		TIFR1 |= (1<<OCF1A);
		
		//procedure for PB3
		PORTB |= (1<<PB3);
		while(!(TIFR1 & (1<<OCF1A)));
		TIFR1 |= (1<<OCF1A);
		PORTB &= ~(1<<PB3);
		while(!(TIFR1 & (1<<OCF1A)));
		TIFR1 |= (1<<OCF1A);
		
	}
}

void ADC_init(void){
	DIDR0 = 0x1;
	ADMUX = (1<<REFS0); //input is PC0
	
	ADCSRA |=(1<<ADEN)|(1<<ADPS2)|(1<<ADPS1)|(1<<ADPS0);
	ADCSRB = 0x0;
}
