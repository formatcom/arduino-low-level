/*
 * Creado por Vinicio Valbuena
 *
 * REF:
 * http://ww1.microchip.com/downloads/en/DeviceDoc/ATmega48A-PA-88A-PA-168A-PA-328-P-DS-DS40002061A.pdf
 * https://www.arduino.cc/en/Hacking/PinMapping168
 */
#define F_CPU 16000000UL

#include <avr/io.h>

#define STATE_INITIAL   1<<0
#define STATE_PRESSED   1<<1
#define STATE_RELEASED  1<<2

uint8_t state = STATE_INITIAL;

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

void detectButton()
{
	if (~PIND & (1<<PD2))
		state = STATE_PRESSED;
	else if(state & STATE_PRESSED)
		state = STATE_RELEASED;
}

void updateLed()
{
	if (state & STATE_RELEASED)
	{
		PORTB ^= 1<<PB5;
		state = STATE_INITIAL;
	}
}

void loop()
{
	detectButton();
	updateLed();
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
