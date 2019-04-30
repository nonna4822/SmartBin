/*
 * Bin.c
 *
 * Created: 4/8/2019 3:14:45 PM
 * Author : elesr
 */ 

#define F_CPU 1000000UL
#include <avr/io.h>
#include <util/delay.h>


int main(void)
{
	DDRD |= (1<<PORTD6);	/* Make OC0A pin as output */
	TCNT0 = 0;		/* Set timer0 count zero */
	TCCR0A|=(1<<COM0A1)|(1<<WGM01)|(1<<WGM00);        //NON Inverted PWM
	TCCR0B|=(1<<CS02); //PRESCALER=256 MODE 14(FAST PWM)
	/* Replace with your application code */
	while (1)
	{
		OCR0A = 0;
		if(PINC & (1<<PORTC0) ){
				OCR0A = 0;	//0
				_delay_ms(1000);
				OCR0A = 125; //90
				_delay_ms(1000);
		}else{
				OCR0A = 0;	
				_delay_ms(1000);
		}
	}
}