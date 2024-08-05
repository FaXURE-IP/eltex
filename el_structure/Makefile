CC = gcc -g 
CFLAGS = -Wall -Wextra -I . -I ../include

SRC = $(wildcard *.c)
OBJ = $(SRC:.c=.o)
LIB = libstructure.a

.PHONY: all clean


EXECUTABLE03 = structure

all: $(LIB) $(EXECUTABLE03)

$(LIB): $(OBJ)
	ar rcs $@ $^

%.o: %.c
	$(CC) $(CFLAGS) -c -o $@ $<

$(EXECUTABLE03): structure.o
	gcc structure.o  -L../el_structure -lstructure -o structure

structure.o: structure.c
	gcc structure.c -I../include -c -o structure.o 

clean:
	rm -f *.o $(LIB)