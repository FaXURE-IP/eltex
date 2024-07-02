CC = gcc
CFLAGS = -Wall -Wextra -I ../include

SRC = $(wildcard *.c)
OBJ = $(SRC:.c=.o)
LIB = libbits.a

.PHONY: all clean

all: $(LIB)

$(LIB): $(OBJ)
	ar rcs $@ $^

%.o: %.c
	$(CC) $(CFLAGS) -c -o $@ $<

clean:
	rm -f *.o $(LIB)