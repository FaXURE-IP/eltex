CC = gcc
CFLAGS = -Wall -Werror
TARGETS = 1

all: $(TARGETS)

1: 1.o
	$(CC) $(CFLAGS) 1.o -o 1

%.o: %.c
	$(CC) $(CFLAGS) -c $< -o $@

clean:
	rm -f $(TARGETS) *.o
	rm -f 1

