NAME=usart

CC=avr-gcc

CFLAGS=-Wall -g -Os -mmcu=atmega328p

SRCS = main.c

PORT ?= /dev/ttyACM0

all: $(NAME).hex

$(NAME).elf:
	$(CC) -o $@ $(SRCS) $(CFLAGS)

$(NAME).hex: $(NAME).elf
	avr-objcopy -j .text -j .data -O ihex $< $@

upload: $(NAME).hex
	@echo --- UPLOAD DATA
	sudo avrdude -p m328p -c arduino -P $(PORT) -b 115200 -U flash:w:$<

clean:
	$(RM) $(NAME).elf $(NAME).hex
