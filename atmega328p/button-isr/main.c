/*
 * Creado por Vinicio Valbuena
 *
 * REF:
 * http://ww1.microchip.com/downloads/en/DeviceDoc/ATmega48A-PA-88A-PA-168A-PA-328-P-DS-DS40002061A.pdf
 * https://www.arduino.cc/en/Hacking/PinMapping168
 */
#define F_CPU 16000000UL

#include <avr/io.h>
#include <avr/interrupt.h>


void __vector_1 (void) __attribute__ ((signal));

void setup()
{

	DDRB = 0xff;

	DDRD = 0xff ^ (1<<PD2);

	PORTD = 1<<PD2;

	PORTB = 1<<PB5;

	// Desactivar interrupciones globales
	cli();

	// Configurar flanco de subida
	EICRA = (1<<ISC01)|(1<<ISC00);

	// Establecer mascara de bit para INT0
	EIMSK = 1<<INT0;

	// No activar interrupcion al habilitar las interrupciones globales
	EIFR = 1<<INTF0;

	// Activar interrupciones globales
	sei();
}

void __vector_1(void)
{
	PORTB ^= 1<<PB5;
}

void loop()
{
}

int main()
{
	setup();

	for (;;)
	{
		loop();
	}

	return 0;
}
