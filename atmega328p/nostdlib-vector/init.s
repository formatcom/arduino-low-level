/*
	vector de interrupciones basados en la version de gcc/libc,
	tiene la limitación de que obligatoriamente la función handler
	se debe llamar __vector_1 para poder funcionar.

	el __vector_1 se mapea a nivel de linker.

	NOTA: Es una version muy simplificada de avr-libc/crt1/gcrt1.S
	sin validación, ...
*/

	.macro vector name
		JMP \name
	.endm

	/*
		"ax"
		a -> alloc es decir reservar espacio en memoria
		x -> ejecutable

		progbits -> te dejo leerlo a ti mismo
		REF: http://man7.org/linux/man-pages/man5/elf.5.html
		sh_type
	*/
	.section .vectors, "ax", @progbits
	.global __vectors

__vectors:
	JMP    __init
	vector __vector_1

	.text
	.global __init

__init:
	CLI
	CALL main
