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

	// Asignamos el PORTD2 como entrada.
	DDRD = 0xff ^ (1<<PD2);

	// Habilitamos la resistencia pull-up
	PORTD = 1<<PD2;

	// Encendemos el LED
	PORTB = 1<<PB5;
}

static inline void loop()
{
	if (~PIND & (1<<PD2))
	{
		while(~PIND & (1<<PD2));
		PORTB ^= 1<<PB5;
	}
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
