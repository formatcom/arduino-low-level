/*
 * Creado por Vinicio Valbuena
 *
 * REF:
 * http://ww1.microchip.com/downloads/en/DeviceDoc/ATmega48A-PA-88A-PA-168A-PA-328-P-DS-DS40002061A.pdf
 * https://www.arduino.cc/en/Hacking/PinMapping168
 */
#define F_CPU 16000000UL

#include <avr/io.h>
#include <util/delay.h>


void setup()
{
	DDRB = 0xff;
}

static inline void loop()
{
	PORTB ^= 1<<PORTB5;
	_delay_ms(1000);
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
