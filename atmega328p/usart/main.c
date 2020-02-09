/*
 * Creado por Vinicio Valbuena
 *
 * REF:
 * http://ww1.microchip.com/downloads/en/DeviceDoc/ATmega48A-PA-88A-PA-168A-PA-328-P-DS-DS40002061A.pdf
 * https://www.arduino.cc/en/Hacking/PinMapping168
 */
#define F_CPU 16000000UL
#define BAUD  9600

#include <stdlib.h>
#include <avr/io.h>
#include <avr/interrupt.h>
#include <util/delay.h>
#include <util/setbaud.h>

void usart_init()
{

	UBRR0 = 0;

	// 21.8.4  UCSR0C - USART MSPIM Control and Status Register 0 C
	// MODE Master SPI
	UCSR0C = (1<<UCSZ01)|(1<<UCSZ00);

	// 20.11.3 UCSR0B - USART Control and Status Register 0 B

	// Enable receiver and transmitter.
	UCSR0B = (1<<RXEN0)|(1<<TXEN0);


	// 20.11.5 UBRR0L and UBRR0H - USART Baud Rate Registers
	UBRR0H = UBRRH_VALUE;
	UBRR0L = UBRRL_VALUE;

}

// 20.11.2 UCSR0A - USART Control and Status Register 0 A
void usart_transmit(unsigned char c)
{
	// Wait for empty transmit buffer
	while ( !( UCSR0A & (1<<UDRE0)) );

	// Send character c
	UDR0 = c;
}

unsigned char usart_receive()
{
	// Wait for data to be received
	while ( !(UCSR0A & (1<<RXC0)) );

	return UDR0;
}

void init()
{
	// enable Global Interrupt
	sei();

	// set timer 1 prescale factor to 64
	TCCR1B = 0x2;
}

void setup()
{
	usart_init();

	const unsigned char message[] = "Creado por Vinicio Valbuena\n";

	_delay_ms(5000);

	uint8_t i;
	size_t l = sizeof(message);

	for (i = 0; i<l; ++i)
	{
		usart_transmit(message[i]);
	}
}

void loop()
{
	usart_transmit(
		usart_receive()
	);
}

int main()
{
	init();
	setup();

	for (;;)
	{
		loop();
	}

	return 0;
}
