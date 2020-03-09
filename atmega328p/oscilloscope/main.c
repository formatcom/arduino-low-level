/*
 * Creado por Vinicio Valbuena
 *
 * REF:
 * http://ww1.microchip.com/downloads/en/DeviceDoc/ATmega48A-PA-88A-PA-168A-PA-328-P-DS-DS40002061A.pdf
 * https://www.arduino.cc/en/Hacking/PinMapping168
 */
#define F_CPU 16000000UL
#define BAUD  9600

#include <avr/io.h>
#include <avr/interrupt.h>
#include <util/setbaud.h>

// REF: https://gcc.gnu.org/onlinedocs/gcc/AVR-Function-Attributes.html
// REF: https://www.arduino.cc/reference/en/language/functions/analog-io/analogreference/
// Arduino uno AREF -> 5V

uint8_t c; // Lo usaremos para guardar los datos del adc

void usart_init()
{
	// MODE Asynchronous
	// REF: Table 20-11. UCSZn Bits Settings
	// Se ajusta a 8 caracteres
	UCSR0C = (1<<UCSZ01)|(1<<UCSZ00);


	// 20.11.3 UCSR0B - USART Control and Status Register 0 B
	// Enable transmitter.
	UCSR0B = (1<<TXEN0);


	// 20.11.5 UBRR0L and UBRR0H - USART Baud Rate Registers
	UBRR0H = UBRRH_VALUE;
	UBRR0L = UBRRL_VALUE;

}

// 20.11.2 UCSR0A - USART Control and Status Register 0 A
void usart_transmit(uint8_t c)
{
	// Wait for empty transmit buffer
	while ( !( UCSR0A & (1<<UDRE0)) );

	// REF: Figure 20-1. USART Block Diagram
	// REF: 20.11.1 UDRn – USART I/O Data Register
	// UDR son en verdad dos registros de IO separados
	// cuando se escribe se envia al TXB
	//
	// Send character c
	UDR0 = c;
}

void adc_init()
{
	// atmega328p solo tiene un convertidor ADC multiplexado
	// Seleccionamos el canal del multiplexor
	// REF: Table 24-4. Input Channel Selections
	ADMUX = (1<<REFS0);

	// REF: 24.9.3 ADCL and ADCH – The ADC Data Register
	// Lo coloco ajustado a la derecha para trabajar solo con 8 bit
	ADMUX |= (1<<ADLAR);

	// REF: Table 24-6. ADC Auto Trigger Source Selections
	ADCSRB = (0<<ADTS2) | (0<<ADTS1) | (0<<ADTS0);

	// REF: Table 24-5. ADC Prescaler Selections
	// Frecuencia del ADC = Frecuencia del CPU / PREESCALAR
	// 16000000 // 16 MHz / 128 = 125 KHz
	// Frecuencia de Muestreo = Frecuencia del ADC / Ciclos que toma completar la conversion
	// Frecuencia de Muestreo = 125 KHz / 13 = 9.6KHz
	ADCSRA = (1<<ADPS2) | (1<<ADPS1) | (1<<ADPS0);

	// Decidi no utilizar Auto Triggered conversions basandome en esta tabla.
	// REF: Table 24-1. ADC Conversion Time

	// Se habilita el ADC
	ADCSRA |= (1<<ADEN);
}

uint8_t adc_read8()
{
	// REF: 24.9.3 ADCL and ADCH – The ADC Data Register
	// Si el resultado se deja ajustado a la derecha y no se requiere mas de 8 bits
	// de precision, es suficiente con leer ADCH. De lo contrario, ADCL debe leerse
	// primero, luego ADCH, para asegurar que el contenido de los registros de datos
	// corresponda a la misma conversion.
	//
	// Una vez que se lee ADCL, se bloquea el acceso de ADC a los registros de datos.
	// Esto significa que si se ha leido ADCL y se completa una conversion antes de
	// leer ADCH, ningunos de los registros se actualiza y se pierde el resultado de
	// la conversion. Cuando se lee ADCH, se vuelve a habilitar el ADC a los registros
	// ADCH y ADCL.

	// Esperar a que termine la conversion
	while ( ADCSRA & (1<<ADSC) );

	// REF: /usr/avr/include/avr/iom328p.h
	// #define ADC     _SFR_MEM16(0x78)
	// word ADC -> ADCL y ADCH

	return ADCH;
}


void setup()
{
	usart_init();
	adc_init();

	// Iniciar Conversion ADC
	ADCSRA |= (1<<ADSC);
}

static inline void loop()
{
	// Leemos el resultado de la conversion una vez esta se complete
	c = adc_read8();

	// Iniciar una nueva conversion, para que se trabaje al mismo tiempo
	// que se este haciendo la transferencia por USART.
	ADCSRA |= (1<<ADSC);

	// Lo enviamos por USART
	usart_transmit(c);
}

int main()
{
	setup();

	for (;;) loop();

	return 0;
}
