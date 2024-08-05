CC = gcc
CFLAGS = -Wall -Wextra -I ../include

SRC = $(wildcard *.c)
OBJ = $(SRC:.c=.o)
LIB = libmas.a

.PHONY: all clean


EXECUTABLE02 = mas


all: $(LIB) $(EXECUTABLE02)

$(LIB): $(OBJ)
	ar rcs $@ $^

%.o: %.c
	$(CC) $(CFLAGS) -c -o $@ $<

$(EXECUTABLE02): mas.o
	gcc mas.o  -L../el_mas -lmas -o mas

mas.o: mas.c 
	gcc mas.c -I../include -c -o mas.o 

clean:
	rm -f *.o $(LIB)