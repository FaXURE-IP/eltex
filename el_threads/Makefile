CC = gcc
CFLAGS = -Wall -pthread -Werror
TARGETS = 1 2 3

all: $(TARGETS)

1: 1.o
	$(CC) $(CFLAGS) 1.o -o 1

2: 2.o
	$(CC) $(CFLAGS) 2.o -o 2

3: 3.o
	$(CC) $(CFLAGS) 3.o -o 3
%.o: %.c
	$(CC) $(CFLAGS) -c $< -o $@

clean:
	rm -f $(TARGETS) *.o
	rm -f 1
	rm -f 2
	rm -f 3
	rm -f test

