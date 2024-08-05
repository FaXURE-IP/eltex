STATIC_LIB = libcalc.a
DYNAMIC_LIB = libmenu.so

CC = gcc
CFLAGS = -Wall -fPIC

EXECUTABLE05=calc

STATIC_SRC = division.c multiplies.c subtruct.c summation.c
STATIC_OBJ = $(STATIC_SRC:.c=.o)

DYNAMIC_SRC = menu
DYNAMIC_OBJ = $(DYNAMIC_SRC:.o)

all: $(STATIC_LIB) menu.o $(DYNAMIC_LIB) $(EXECUTABLE05)

$(STATIC_LIB): $(STATIC_OBJ)
	ar rcs $@ $^

$(DYNAMIC_LIB): menu.o
	$(CC) -shared -o $@ $^ $(STATIC_LIB)

menu.o: menu.c 
	$(CC) $(CFLAGS) -I../include -c $< -o $@

%.o: %.c
	$(CC) $(CFLAGS) -I../include -c $< -o $@

$(EXECUTABLE05): calc.o
	gcc calc.o -L../el_calculate -lmenu -Wl,-rpath,../el_calculate -o calc

calc.o: calc.c
	gcc calc.c -I../include  -L../el_calculate -lmenu -c -o calc.o 



clean:
	rm -f *.o 
	rm -f *.a
	rm -f *.so
