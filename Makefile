# Default target: build executable
all: a4

# Rule for building executable with the required flags
a4:
	gcc -Wall -std=c99 -pedantic main.c graph_functions.c -o a4

# Clean up executable
clean:
	rm -f a4