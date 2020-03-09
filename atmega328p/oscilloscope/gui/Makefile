NAME=oscilloscope

CC=gcc

CFLAGS=-Wall -lGL -lglut

SRCS = main.c

all: $(NAME)

$(NAME):
	$(CC) -o $@ $(SRCS) $(CFLAGS)

clean:
	$(RM) $(NAME)
