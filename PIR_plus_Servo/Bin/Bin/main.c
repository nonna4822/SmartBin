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
	DDRB |= (1<<PORTB1);	/* Make OC1A pin as output */
	TCNT1 = 0;		/* Set timer1 count zero */
	ICR1 = 2499;		/* Set TOP count for timer1 in ICR1 register */
	//TCCR1A|=(1<<COM1A1)|(1<<WGM11);        //NON Inverted PWM
	//TCCR1B|=(1<<WGM13)|(1<<WGM12)|(1<<CS11); //PRESCALER=8 MODE 14(FAST PWM)

	
	/* Set Fast PWM, TOP in ICR1, Clear OC1A on compare match, clk/64 */
	TCCR1A = (1<<WGM11)|(1<<COM1A1);
	TCCR1B = (1<<WGM12)|(1<<WGM13)|(1<<CS11);
	
	/* Replace with your application code */
	while (1)
	{
		OCR1A = 0;
		if(PINB & (1<<PORTB) ){
			if(PIND & (1<<PIND2)){ //close bin
				OCR1A = 175;
				_delay_ms(100);
			}else {
				OCR1A = 300;	//open bin
				_delay_ms(100);
			}
			
		}
	}
}

