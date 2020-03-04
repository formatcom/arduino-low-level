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

// REF: https://gcc.gnu.org/onlinedocs/gcc/AVR-Function-Attributes.html
// REF: https://www.arduino.cc/reference/en/language/functions/analog-io/analogreference/
// Arduino uno AREF -> 5V

volatile uint16_t wADC; // para hacer backup del ADC y luego enviarlo por el USART

void ADC_vect(void)
{

	// REF: /usr/avr/include/avr/iom328p.h
	// #define ADC     _SFR_MEM16(0x78)
	// word ADC -> ADCL y ADCH
	wADC = ADC;

	// Habilitar las interrupciones globales
	sei();
}

void ADCInit()
{
	// atmega328p solo tiene un convertidor ADC multiplexado
	// Seleccionamos el canal del multiplexor
	// REF: Table 24-4. Input Channel Selections
	ADMUX = (0<<MUX3) | (0<<MUX2) | (0<<MUX1) | (0<<MUX0);

	// REF: Table 24-6. ADC Auto Trigger Source Selections
	// Free Running mode
	ADCSRB = (0<<ADTS2) | (0<<ADTS1) | (0<<ADTS0);

	// REF: Table 24-5. ADC Prescaler Selections
	// Frecuencia del ADC = Frecuencia del CPU / PREESCALAR
	// 16000000 // 16 MHz / 128 = 125 KHz
	// Frecuencia de Muestreo = Frecuencia del ADC / Ciclos que toma completar la conversion
	// Frecuencia de Muestreo = 125 KHz / 13 = 9.6KHz
	ADCSRA = (1<<ADPS2) | (1<<ADPS1) | (1<<ADPS0);

	// Se habilita el ADC, la interrupcion y  ADC Auto Trigger Enable
	ADCSRA |= (1<<ADEN) | (1<<ADIE) | (1<<ADATE);
}

void setup()
{
	// Desactivar interrupciones globales
	cli();

	ADCInit();

	// Habilitar las interrupciones globales
	sei();
}

int main()
{
	setup();

	for (;;);

	return 0;
}
