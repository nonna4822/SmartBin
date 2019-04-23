#define RW PORTC0
#define RS PORTC1
#define E  PORTC2
#define CTRL_DDR  DDRC
#define CTRL_PORT PORTC
#define DATA_DDR  DDRD
#define DATA_PORT PORTD

#define KEYPAD_DDR  DDRB
#define KEYPAD_PORT PORTB
#define KEYPAD_PIN PINB

#include <stdio.h>
#include <string.h>
#include <math.h>
#include <avr/io.h>
#define F_CPU 8000000UL
#include <util/delay.h>
#include <avr/interrupt.h>

#define Trigger_pin PORTB1 /* Trigger pin */

int TimerOverflow = 0;

ISR(TIMER1_OVF_vect)
{
	TimerOverflow++; /* Increment Timer Overflow count */
}

void sendCommand(unsigned char command)
{
	CTRL_PORT &= ~(1 << RW) & ~(1 << RS);  // set RS to 0 to send command
	DATA_PORT = command;
	CTRL_PORT |= (1 << E); // send short pulse to finish transfer
	_delay_ms(1);
	CTRL_PORT &= ~(1 << E);
	_delay_ms(1);
}

void sendData(unsigned char data)
{
	CTRL_PORT &= ~(1 << RW);
	CTRL_PORT |= (1 << RS);	// set RS to 1 to send data
	DATA_PORT = data;
	CTRL_PORT |= (1 << E); // send short pulse to finish transfer
	_delay_ms(1);
	CTRL_PORT &= ~(1 << E);
	_delay_ms(1);
	CTRL_PORT &= ~(1 << RS);
}

int main(void)
{
	double distance;
	long count;
	
	// initialize LCD
	CTRL_DDR |= (1 << RW) | (1 << RS) | (1 << E);
	CTRL_PORT &= ~(1 << RW) & ~(1 << RS) & ~(1 << E);
	DATA_DDR = 0xFF;
	DATA_PORT = 0;
	
	DDRB |= (1<<PORTB1);
	PORTB |= (1<<PORTB0);

	sei();                /* Enable global interrupt */
	TIMSK1 = (1 << TOIE1); /* Enable Timer1 overflow interrupts */
	TCCR1A = 0;           /* Set all bit to zero Normal operation */
	
	sendCommand(0x38);
	sendCommand(0x0E);
	sendCommand(0x01);  // clear screen
	sendCommand(0x06);	// shift cursor to right
	sendCommand(0x80);	// set cursor to first character on line 1
	
	while (1)
	{
		sendCommand(0x01);
		sendCommand(0x80);
		
		int i=0;
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
		
		if(distance < 1){
			PORTB |= (1<<PB2);
		}else{
			PORTB &= ~(1<<PB2);
		}
		
		char buffer[20];
		dtostrf(distance, 2, 2, buffer);
		strcat(buffer," cm");
		for (i=0; i<strlen(buffer); i++)
		{
			sendData(buffer[i]);
		}
		
		_delay_ms(250);
	}
}
