MEMORY
{
	FLASH  (rx)   : ORIGIN = 0x000000, LENGTH = 32k
	SRAM   (rw!x) : ORIGIN = 0x800100, LENGTH = 2k
	EEPROM (rw!x) : ORIGIN = 0x810000, LENGTH = 1k
}
SECTIONS
{
	.text   :
	{
		*(.text)
		. = ALIGN(2); /* align address 16 bits */
		*(.text.*)
		. = ALIGN(2);
	}  > FLASH
	.data          :
	{
		*(.data)
		*(.data*)
		*(.rodata)  /* We need to include .rodata here if gcc is used */
		*(.rodata*) /* with -fdata-sections.  */
		. = ALIGN(2);
	}  > SRAM AT> FLASH
}
