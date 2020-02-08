## INSTALL
~~~
- avr-gcc
- avr-gcc-c++
- avr-libc
- avr-gdb
- avrdude
- simavr
- radare2         <-     Aun no lo documento
~~~

## BUILD
~~~
$ avr-gcc -Wall -g -Os -mmcu=atmega328p -o demo.elf hello.c
~~~

### ENTRYPOINT -> Vector Reset

## headers/Intermix source code with disassembly
~~~
$ avr-objdump -h -S demo.elf
~~~

## the .data section 
### ref: https://www.microchip.com/webdoc/AVRLibcReferenceManual/mem_sections_1sec_dot_data.html
~~~
REF:
microchip.com/webdoc/AVRLibcReferenceManual/mem_sections_1sec_dot_noinit.html#mem_sections_1harvard_arch

Because of the Harvard architecture of the AVR devices, you must manually add 0x800000 to the address
you pass to the linker as the start of the section. Otherwise, the linker thinks you want to put the
.noinit section into the .text section instead of .data/.bss and will complain.


- Harvard architecture 

--------------------                      -------------------                      ------------------
|		   |       address bus    |                 |     address bus      |                |
|  Program Memory  |    <--------------   |                 |   -------------->    |   Data Memory  |
|	(ROM)      |       data bus       |      (CPU)      |      data bus        |      (RAM)     |
|		   |    -------------->   |                 |   <--------------    |                |
--------------------                      -------------------                      ------------------
~~~

## AVR Memory Usage
~~~
$ avr-size -C demo.elf
~~~

## Generating Intel Hex Files
### REF: https://www.microchip.com/webdoc/AVRLibcReferenceManual/group__demo__project_1demo_ihex.html
~~~
$ avr-objcopy -j .text -j .data -O ihex demo.elf demo.hex
~~~

## hex disassembly
~~~
$ avr-objdump --no-show-raw-insn -m avr --prefix-addresses -D demo.hex > demo.asm
~~~

## simavr mode debug demo.elf
~~~
$ simavr -g -m atmega328p -f 16000000 demo.elf
~~~

## simavr mode debug demo.hex
~~~
$ simavr -g -m atmega328p -f 16000000 demo.hex
~~~

## connect debug with avr-gdb
~~~
$ avr-gdb
(gdb) file blink.elf                  |   cargar datos del binario
(gdb) target remote 0.0.0.0:1234      |   conectarse al servidor gdb
(gdb) load                            |   no lo se definir xD...
(gdb) info files                      |   informacion del binario
(gdb) info registers                  |   ver registros
(gdb) break main.c:loop               |   agregar breakpoint en loop
(gdb) break main.c:26                 |   agregar breakpoint en la linea 26
(gdb) info breakpoints                |   ver todos los breakpoints
(gdb) clear main.c:loop               |   elimina breakpoint
(gdb) print PORTB                     |   imprimir valor de la variable PORTB
(gdb) set variable PORTB = 0xff       |   asignar valor a la variable PORTB
(gdb) print PORTB                     |   print dec
(gdb) print/x PORTB                   |   print hex
(gdb) print/t PORTB                   |   print bin
(gdb) list main.c:loop                |   ver el codigo fuente de la funcion loop
(gdb) step                            |   ejecutar una instruccion
(gdb) continue                        |   ejecutar hasta encontrar un breakpoint
(gdb) info frame                      |   luego lo defino, pero es importante
(gdb) info variables                  |   ver las variables
~~~

## connect debug with radare2
~~~
$ r2 -a avr -d gdb://0.0.0.0:1234
~~~

## dump programmer stk500
~~~
$ avrdude -F -v -p m328p -c stk500 -P /dev/ttyUSB0 -D -U flash:r:reverse.hex:i
~~~

## dump programmer arduino
~~~
$ avrdude -F -v -p m328p -c arduino -P /dev/ttyUSB0 -D -U flash:r:reverse.hex:i
~~~

## upload data arduino uno
~~~
$ avrdude -p m328p -c arduino -P /dev/ttyACM0 -b 115200 -U flash:w:demo.hex
~~~
