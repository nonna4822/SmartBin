/*
 * Bin.c
 *
 * Created: 4/8/2019 3:14:45 PM
 * Author : elesr
 */ 

#ifndef F_CPU
#define F_CPU 8000000UL
#endif
#include <avr/io.h>
#include <util/delay.h>


int main(void)
{
	DDRB = 0x00;
	DDRD = 0xff;
    /* Replace with your application code */
    while (1) 
    {
		if(PINB & (1<<PORTB)){
			PORTD = 0x46;
			_delay_ms(1000);
			//PORTD = 0x8f;
			//_delay_ms(1000);
			//PORTD = 0x89;
			//_delay_ms(1000);
			PORTD = 0x00;
		}
		
    }
}

