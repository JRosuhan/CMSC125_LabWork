CC = gcc
CFLAGS = -Wall -Wextra -std=c11

all: mysh

mysh: main.c
	$(CC) $(CFLAGS) main.c -o mysh

clean:
	rm -f mysh
