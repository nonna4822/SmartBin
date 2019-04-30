/*
 * Bin.c
 *
 * Created: 4/8/2019 3:14:45 PM
 * Author : elesr
 */ 

#define F_CPU 1000000UL
#include <avr/io.h>
#include <util/delay.h>
#include <avr/interrupt.h>
#define Trigger_pin PORTB1 /* Trigger pin */

int TimerOverflow = 0;

ISR(TIMER1_OVF_vect)
{
	TimerOverflow++; /* Increment Timer Overflow count */
}

int main(void)
{
	/*ultrasonic */
	double distance;
	long count;
	
	DDRB |= (1<<PORTB1) || (1<<PORTB2);
	PORTB |= (1<<PORTB0);

	sei();                /* Enable global interrupt */
	TIMSK1 = (1 << TOIE1); /* Enable Timer1 overflow interrupts */
	TCCR1A = 0;           /* Set all bit to zero Normal operation */
	
	/*servo-motor */
	DDRC |= 0x00;
	DDRD |= (1<<PORTD6);	/* Make OC0A pin as output */
	TCNT0 = 0;		/* Set timer0 count zero */
	TCCR0A|=(1<<COM0A1)|(1<<WGM01)|(1<<WGM00);        //NON Inverted PWM
	TCCR0B|=(1<<CS01); //PRESCALER=256 MODE 14(FAST PWM)
	/* Replace with your application code */
	
	while (1)
	{
		/* ultrasonic */
		DDRB |= (1<<PB2);
		
		/* Give 10us trigger pulse on trig. pin to HC-SR04 */
		PORTB |= (1 << Trigger_pin);
		_delay_us(10);
		PORTB &= (~(1 << Trigger_pin));

		TCNT1 = 0;         /* Clear Timer counter */
		TCCR1B = 0x41;     /* Capture on rising edge, No prescaler*/
		TIFR1 = 1 << ICF1; /* Clear ICP flag (Input Capture flag) */
		TIFR1 = 1 << TOV1; /* Clear Timer Overflow flag */

		/*Calculate width of Echo by Input Capture (ICP) */

		while ((TIFR1 & (1 << ICF1)) == 0);              /* Wait for rising edge */
		TCNT1 = 0;         /* Clear Timer counter */
		TCCR1B = 0x01;     /* Capture on falling edge, No prescaler */
		TIFR1 = 1 << ICF1; /* Clear ICP flag (Input Capture flag) */
		TIFR1 = 1 << TOV1; /* Clear Timer Overflow flag */
		TimerOverflow = 0; /* Clear Timer overflow count */

		while ((TIFR1 & (1 << ICF1)) == 0);  /* Wait for falling edge */
		count = ICR1 + (65535 * TimerOverflow); /* Take count */
		/* 8MHz Timer freq, sound speed =343 m/s */
		distance = (double)count / 466.47;
		if(distance < 5){
			PORTB |= (1<<PORTB2);
		}else{
			PORTB &= ~(1<<PORTB2);
		}
		
		
	}
}