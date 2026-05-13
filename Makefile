CC = gcc
CFLAGS = -Wall -Wextra

mysh: main.o parser.o executor.o
	$(CC) $(CFLAGS) -o mysh main.o parser.o executor.o

main.o: main.c mysh.h
	$(CC) $(CFLAGS) -c main.c

parser.o: parser.c mysh.h
	$(CC) $(CFLAGS) -c parser.c

executor.o: executor.c mysh.h
	$(CC) $(CFLAGS) -c executor.c

clean:
	rm -f *.o mysh