CC = gcc
CFLAGS = -Wall -Werror
TARGETS = 1 2

all: $(TARGETS)

1: 1.o
	$(CC) $(CFLAGS) 1.o -o 1

2: 2.o
	$(CC) $(CFLAGS) 2.o -o 2

%.o: %.c
	$(CC) $(CFLAGS) -c $< -o $@

clean:
	rm -f $(TARGETS) *.o
	rm -f 1
	rm -f 2

