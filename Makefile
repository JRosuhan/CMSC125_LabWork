# Compiler used
CC = gcc

# Compiler flags:
# -Wall: show warnings
# -Wextra: show extra warnings for safer code
CFLAGS = -Wall -Wextra

# Build the final executable (mysh)
# Links all object files together
mysh: main.o parser.o executor.o
	$(CC) $(CFLAGS) -o mysh main.o parser.o executor.o

# Compile main source file into object file
main.o: main.c mysh.h
	$(CC) $(CFLAGS) -c main.c

# Compile parser module
parser.o: parser.c mysh.h
	$(CC) $(CFLAGS) -c parser.c

# Compile executor module
executor.o: executor.c mysh.h
	$(CC) $(CFLAGS) -c executor.c

# Clean build files (object files + executable)
clean:
	rm -f *.o mysh