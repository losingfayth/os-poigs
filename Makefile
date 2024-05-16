# Makefile to compile #POIGS# program
## Not for brownie points, but because I put #'s in the source file name
## because I thought it would be funny and now it's annoying to compile

# Compiler settings
CC = gcc
CFLAGS = -Wall
OUTPUT = poigs

# Source files
SOURCES = art.c command.c list.c utils.c
PRIMARY_SOURCE = \#poigs\#.c

# Default rule to build the program
all: $(OUTPUT)

$(OUTPUT): $(PRIMARY_SOURCE) $(SOURCES)
	$(CC) $(CFLAGS) '$(PRIMARY_SOURCE)' $(SOURCES) -o $(OUTPUT)

# Clean rule to clean up compiled files
clean:
	rm -f $(OUTPUT)

.PHONY: all clean