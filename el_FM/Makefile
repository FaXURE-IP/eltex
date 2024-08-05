CC = gcc
CFLAGS = -Wall -Wextra -I ../include

SRC = $(wildcard *.c)
OBJ = $(SRC:.c=.o)
LIB = libFM.a

EXECUTABLE04 = FM

.PHONY: all clean

all: $(LIB) $(EXECUTABLE04)

$(LIB): $(OBJ)
	ar rcs -lncursesw $@ $^

%.o: %.c
	$(CC) $(CFLAGS) -c -o $@ $<

$(EXECUTABLE04): FM.o
	gcc FM.o  -L../el_FM -lFM -lncursesw -o FM

FM.o: FM.c
	gcc FM.c -I../include -c -o FM.o 

clean:
	rm -f *.o $(LIB)