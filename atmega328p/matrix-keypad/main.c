/*
 * Creado por Vinicio Valbuena
 *
 * REF:
 * http://ww1.microchip.com/downloads/en/DeviceDoc/ATmega48A-PA-88A-PA-168A-PA-328-P-DS-DS40002061A.pdf
 * https://www.arduino.cc/en/Hacking/PinMapping168
 */
#define F_CPU 16000000UL

#include <avr/io.h>

// Solo reconocer la fila 2

void setup()
{
	// filas como entrada
	// columnas como salida
	DDRD = 0xf0;

	// fila 2 la colocamos a 1
	PORTD = 0x0f;

}

int main()
{
	setup();

	for (;;);

	return 0;
}
