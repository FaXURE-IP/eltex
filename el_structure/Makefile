CC = gcc -g 
CFLAGS = -Wall -Wextra -I . -I ../include

SRC = $(wildcard *.c)
OBJ = $(SRC:.c=.o)
LIB = libstructure.a

.PHONY: all clean

all: $(LIB)

$(LIB): $(OBJ)
	ar rcs $@ $^

%.o: %.c
	$(CC) $(CFLAGS) -c -o $@ $<

clean:
	rm -f *.o $(LIB)