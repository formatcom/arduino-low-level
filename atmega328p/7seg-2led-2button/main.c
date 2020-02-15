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
#include <util/delay.h>

#define SEG_A      1<<PC0
#define SEG_B      1<<PC1
#define SEG_C      1<<PC2
#define SEG_D      1<<PC3
#define SEG_E      1<<PC4
#define SEG_G      1<<PC5
#define SEG_ZERO   0x3f

const int8_t symbol[] = {
	SEG_C | SEG_D | SEG_E | SEG_G,
	SEG_B | SEG_C,
	SEG_ZERO ^ SEG_C,
	SEG_ZERO ^ SEG_E,
};

uint8_t dir = 1;
uint8_t count = 0;

void setup()
{

	DDRB = 0xff;
	DDRC = 0x3f;

	DDRD = 0xff ^ ((1<<PD3)|(1<<PD2));

	PORTD = (1<<PD3)|(1<<PD2);

	PORTB = 1<<PB4;

	PORTC = ~symbol[1];

	// Desactivar interrupciones globales
	cli();

	// Configurar flanco de subida
	EICRA = (1<<ISC11)|(1<<ISC10)|(1<<ISC01)|(1<<ISC00);

	// Establecer mascara de bit para INT0
	EIMSK = (1<<INT1)|(1<<INT0);

	// No activar interrupcion al habilitar las interrupciones globales
	EIFR = (1<<INTF1)|(1<<INTF0);

	// Activar interrupciones globales
	sei();
}

ISR(INT0_vect)
{
	PORTB ^= (1<<PB5) | (1<<PB4);
	dir *= -1;
}

ISR(INT1_vect, ISR_ALIASOF(INT0_vect));

void loop()
{
	PORTC = ~symbol[count];
	_delay_ms(1500);
	count = (count + dir) % 4;
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
