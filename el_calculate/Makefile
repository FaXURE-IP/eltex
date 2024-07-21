STATIC_LIB = libcalc.a
DYNAMIC_LIB = libmenu.so

CC = gcc
CFLAGS = -Wall -fPIC

STATIC_SRC = division.c multiplies.c subtruct.c summation.c
STATIC_OBJ = $(STATIC_SRC:.c=.o)

DYNAMIC_SRC = menu
DYNAMIC_OBJ = $(DYNAMIC_SRC:.o)

all: $(STATIC_LIB) menu.o $(DYNAMIC_LIB)

$(STATIC_LIB): $(STATIC_OBJ)
	ar rcs $@ $^

$(DYNAMIC_LIB): menu.o
	$(CC) -shared -o $@ $^ $(STATIC_LIB)

menu.o: menu.c 
	$(CC) $(CFLAGS) -I../include -c $< -o $@

%.o: %.c
	$(CC) $(CFLAGS) -I../include -c $< -o $@

clean:
	rm -f $(STATIC_OBJ) $(DYNAMIC_OBJ) $(STATIC_LIB) $(DYNAMIC_LIB)
