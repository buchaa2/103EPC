#define F_CPU 16000000L
#include <avr/io.h>
#include <util/delay.h>
volatile unsigned int ADCVal;

int main(void) {
	//Set DDR's
	DDRB = 0xFF; // Set port B as output

	//Set ADC
	DIDR0 = 0x1;
	ADMUX = (1<<REFS0); //input pc0
	
	ADCSRA |=(1<<ADEN)|(1<<ADPS2)|(1<<ADPS1)|(1<<ADPS0);
	ADCSRB = 0x0;
	
	TCCR1B = (1<<WGM13)|(1<<WGM12)|(1<<CS11)|(1<<CS10); //fast PWM mode and prescaler = 64
	TCCR1A = (1<<COM1A1)|(1<<WGM11); // Non-inverting PWM
	ICR1 = 2500; // Top of counter

	//Main body loop
	while (1)
	{
		ADCSRA |= (1 << ADSC); //start ADC conversion
		while((!ADCSRA) &(1<<ADIF)); // Wait for conversion
		OCR1A = ADC/2 ; //Set pwm with ADCValue
		
	}
}
