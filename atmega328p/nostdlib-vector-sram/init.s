
	.equ ptr_vector_int0, 0x800100
	.global ptr_vector_int0

	.macro vector ptr_vector
		LDS   r31, \ptr_vector+1
		LDS   r30, \ptr_vector
		IJMP
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
	JMP    __vector_1

	.text
	.global __init

__init:
	CLI
	CALL main

__vector_1: vector ptr_vector_int0


