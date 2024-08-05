CC = gcc
CFLAGS = -Wall -Wextra -I ../include

SRC = $(wildcard *.c)
OBJ = $(SRC:.c=.o)
LIB = libbits.a


EXECUTABLE01 = bits 

all: $(LIB) bits

$(LIB): $(OBJ)
	ar rcs $@ $^

%.o: %.c
	$(CC) $(CFLAGS) -c -o $@ $<

$(EXECUTABLE01): bits.o
	gcc bits.o  -L../el_bits -lbits -o bits

bits.o: bits.c
	gcc bits.c -I../include -c -o bits.o 

clean:
	rm -f *.o $(LIB)