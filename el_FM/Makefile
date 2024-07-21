CC = gcc
CFLAGS = -Wall -Wextra -I ../include

SRC = $(wildcard *.c)
OBJ = $(SRC:.c=.o)
LIB = libFM.a

.PHONY: all clean

all: $(LIB)

$(LIB): $(OBJ)
	ar rcs -lncursesw $@ $^

%.o: %.c
	$(CC) $(CFLAGS) -c -o $@ $<

clean:
	rm -f *.o $(LIB)