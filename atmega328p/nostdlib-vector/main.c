/*
 * Creado por Vinicio Valbuena
 *
 * REF:
 * http://ww1.microchip.com/downloads/en/DeviceDoc/ATmega48A-PA-88A-PA-168A-PA-328-P-DS-DS40002061A.pdf
 * https://www.arduino.cc/en/Hacking/PinMapping168
 */
#define F_CPU 16000000UL

#include <avr/io.h>


void setup()
{
	DDRB = 0xff;

	DDRD = 0xff ^ (1<<PD2);

	PORTD = 1<<PD2;

	PORTB = 1<<PB5;

	// Configurar flanco de subida
	EICRA = (1<<ISC01)|(1<<ISC00);

	// Establecer mascara de bit para INT0
	EIMSK = 1<<INT0;

	// REF: 13.2.3 EIFR – External Interrupt Flag Register
	// No activar interrupcion al habilitar las interrupciones globales
	EIFR = 1<<INTF0;

	// Activar interrupciones globales
	SREG = 1<<7;
}

void __vector_1(void)
{
	PORTB ^= 1<<PB5;

	/* 7.3.1 SREG – AVR Status Register

	   El hardware borra el bit I después de que se haya producido una
	   interrupción.

	*/

	SREG = 1<<7; // esto es quivalente a RETI en asm.
}

int main()
{
	setup();

	for (;;);

	return 0;
}
